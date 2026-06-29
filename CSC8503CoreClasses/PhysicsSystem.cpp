#include "PhysicsSystem.h"
#include "PhysicsObject.h"
#include "GameObject.h"
#include "CollisionDetection.h"
#include "Quaternion.h"
#include "Constraint.h"
#include "Debug.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;

PhysicsSystem::PhysicsSystem(GameWorld& g) : gameWorld(g)
{
    applyGravity = false;
    useBroadPhase = false;
    dTOffset = 0.0f;
    globalDamping = 0.96f;

    SetGravity(Vector3(0.0f, -9.8f, 0.0f));
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::SetGravity(const Vector3& g)
{
    gravity = g;
}

void PhysicsSystem::Clear()
{
    allCollisions.clear();
}

bool useSimpleContainer = false;
int constraintIterationCount = 20;

const int   idealHZ = 240;
const float idealDT = 1.0f / idealHZ;

int realHZ = idealHZ;
float realDT = idealDT;

void PhysicsSystem::Update(float dt)
{
    // Debug controls
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::B)) {
        useBroadPhase = !useBroadPhase;
        std::cout << "Setting broadphase to " << useBroadPhase << std::endl;
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::N)) {
        useSimpleContainer = !useSimpleContainer;
        std::cout << "Setting broad container to " << useSimpleContainer << std::endl;
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::I)) {
        constraintIterationCount--;
        std::cout << "Setting constraint iterations to " << constraintIterationCount << std::endl;
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::O)) {
        constraintIterationCount++;
        std::cout << "Setting constraint iterations to " << constraintIterationCount << std::endl;
    }

    dTOffset += dt;

    GameTimer timer;
    timer.GetTimeDeltaSeconds();

    if (useBroadPhase) {
        UpdateObjectAABBs();
    }

    // Fixed timestep loop
    while (dTOffset > realDT)
    {
        IntegrateAccel(realDT);

        if (useBroadPhase)
        {
            BroadPhase();
            NarrowPhase();
        }
        else
        {
            BasicCollisionDetection();
        }

        // Solve constraints
        float constraintDt = realDT / (float)constraintIterationCount;

        for (int i = 0; i < constraintIterationCount; ++i)
        {
            UpdateConstraints(constraintDt);
        }

        // Update transforms from velocities
        IntegrateVelocity(realDT);

        dTOffset -= realDT;
    }

    ClearForces();
    UpdateCollisionList();

    timer.Tick();
    float updateTime = timer.GetTimeDeltaSeconds();

    // Auto-adjust physics tick rate to maintain FPS
    if (updateTime > realDT) {
        realHZ /= 2;
        realDT *= 2;
        std::cout << "Dropping iteration count due to long physics update (now "
            << realHZ << ")\n";
    }
    else if (dt * 2 < realDT) {
        int prev = realHZ;
        realHZ *= 2;
        realDT /= 2;

        if (realHZ > idealHZ) {
            realHZ = idealHZ;
            realDT = idealDT;
        }
        if (prev != realHZ) {
            std::cout << "Increasing iteration count due to fast physics update (now "
                << realHZ << ")\n";
        }
    }
}

/*
===============================================================================
                             COLLISION LIFETIME MGMT
===============================================================================
*/

void PhysicsSystem::UpdateCollisionList()
{
    for (auto it = allCollisions.begin(); it != allCollisions.end(); )
    {
        if ((*it).framesLeft == numCollisionFrames)
        {
            it->a->OnCollisionBegin(it->b);
            it->b->OnCollisionBegin(it->a);

            if (collisionCallback)
                collisionCallback(it->a, it->b, *it);
        }

        CollisionDetection::CollisionInfo& info =
            const_cast<CollisionDetection::CollisionInfo&>(*it);

        info.framesLeft--;

        if (info.framesLeft < 0)
        {
            info.a->OnCollisionEnd(info.b);
            info.b->OnCollisionEnd(info.a);
            it = allCollisions.erase(it);
        }
        else {
            ++it;
        }
    }
}

void PhysicsSystem::UpdateObjectAABBs()
{
    gameWorld.OperateOnContents(
        [](GameObject* g) {
            g->UpdateBroadphaseAABB();
        }
    );
}

void PhysicsSystem::BasicCollisionDetection()
{
    // (Empty in tutorial skeleton)
}

/*
===============================================================================
                                BROADPHASE + NARROWPHASE
===============================================================================
*/

void PhysicsSystem::BroadPhase()
{
    broadphaseCollisions.clear();

    std::vector<GameObject*> objects;

    gameWorld.OperateOnContents([&](GameObject* g) {
        objects.push_back(g);
        });

    for (int i = 0; i < objects.size(); i++)
    {
        for (int j = i + 1; j < objects.size(); j++)
        {
            GameObject* a = objects[i];
            GameObject* b = objects[j];

            CollisionDetection::CollisionInfo info;
            info.a = a;
            info.b = b;

            broadphaseCollisions.insert(info);
        }
    }
}

void PhysicsSystem::NarrowPhase()
{
    for (auto& bpInfo : broadphaseCollisions)
    {
        CollisionDetection::CollisionInfo info;
        info.a = bpInfo.a;
        info.b = bpInfo.b;

        if (CollisionDetection::ObjectIntersection(info.a, info.b, info))
        {
            allCollisions.insert(info);
        }
    }
}

/*
===============================================================================
                               INTEGRATION !!!
===============================================================================
*/

void PhysicsSystem::IntegrateAccel(float dt)
{
    gameWorld.OperateOnContents([&](GameObject* o)
        {
            PhysicsObject* phys = o->GetPhysicsObject();
            if (!phys) return;

            float invMass = phys->GetInverseMass();
            if (invMass <= 0.0f) return; // static object

            // --- Linear Acceleration ---
            Vector3 acceleration = phys->GetForce() * invMass;

            if (applyGravity)
                acceleration += gravity;

            Vector3 vel = phys->GetLinearVelocity();
            vel += acceleration * dt;
            phys->SetLinearVelocity(vel);

            // --- Angular Acceleration ---
            Vector3 angVel = phys->GetAngularVelocity();
            Vector3 torque = phys->GetTorque();
            Vector3 angAccel = phys->GetInertiaTensor() * torque;

            angVel += angAccel * dt;
            phys->SetAngularVelocity(angVel);
        });
}

void PhysicsSystem::IntegrateVelocity(float dt)
{
    gameWorld.OperateOnContents([&](GameObject* o)
        {
            PhysicsObject* phys = o->GetPhysicsObject();
            if (!phys) return;

            float invMass = phys->GetInverseMass();
            if (invMass <= 0.0f) return;

            Transform& trans = o->GetTransform();

            // --- Linear motion ---
            Vector3 pos = trans.GetPosition();
            Vector3 vel = phys->GetLinearVelocity();

            pos += vel * dt;
            trans.SetPosition(pos);

            // --- Angular motion ---
           /* Vector3 angVel = phys->GetAngularVelocity();

    if (Vector::Dot(angVel, angVel) > 0.0f)
            {
                Quaternion q = trans.GetOrientation();
                Quaternion dq(angVel * dt, 0.0f);
                q = q + dq * q * 0.5f;
                q.Normalise();
                trans.SetOrientation(q);
            }*/


            Vector3 angVel = phys->GetAngularVelocity();

            // --- FIXED ANGULAR INTEGRATION ---
            float speedSq = Vector::Dot(angVel, angVel);
            if (speedSq > 0.000001f)
            {
                float speed = sqrt(speedSq);
                Vector3 axis = angVel / speed;
                float angle = speed * dt * 0.5f;

                float sinHalf = sin(angle);
                float cosHalf = cos(angle);

                // Construct delta rotation (your engine supports this constructor)
                Quaternion delta(axis.x * sinHalf,
                    axis.y * sinHalf,
                    axis.z * sinHalf,
                    cosHalf);

                Quaternion q = trans.GetOrientation();
                q = delta * q;
                q.Normalise();

                trans.SetOrientation(q);
            }


            // --- Apply Damping ---
            vel *= globalDamping;
            phys->SetLinearVelocity(vel);

            angVel *= globalDamping;
            phys->SetAngularVelocity(angVel);
        });
}

void PhysicsSystem::ClearForces()
{
    gameWorld.OperateOnContents(
        [](GameObject* o) {
            o->GetPhysicsObject()->ClearForces();
        }
    );
}

void PhysicsSystem::UpdateConstraints(float dt)
{
    std::vector<Constraint*>::const_iterator first;
    std::vector<Constraint*>::const_iterator last;

    gameWorld.GetConstraintIterators(first, last);

    for (auto i = first; i != last; ++i)
    {
        (*i)->UpdateConstraint(dt);
    }
}

