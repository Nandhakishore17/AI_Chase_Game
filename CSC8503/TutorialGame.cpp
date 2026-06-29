#include "Pathfinding.h"
#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsSystem.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "CollisionDetection.h"

#include "Window.h"              
#include "Keyboard.h"
#include "Mouse.h"
#include "Vector.h"             
#include "Quaternion.h"          
#include "Matrix.h"              
#include "Debug.h"
#include "KeyboardMouseController.h"
#include "GameTechRendererInterface.h"
#include <cfloat>


using namespace NCL;
using namespace CSC8503;


TutorialGame::TutorialGame(GameWorld& gameWorld, GameTechRendererInterface& gameRenderer, PhysicsSystem& gamePhysics)
    : world(gameWorld)
    , renderer(gameRenderer)
    , physics(gamePhysics)
{
    controller = new KeyboardMouseController(
        *Window::GetWindow()->GetKeyboard(),
        *Window::GetWindow()->GetMouse());

    world.GetMainCamera().SetController(*controller);

    
    controller->MapAxis(0, "Sidestep");   
    
    controller->MapAxis(2, "Forward");    

    controller->MapAxis(3, "XLook");      
    controller->MapAxis(4, "YLook");     

    useGravity = false;
    inSelectionMode = false;
    forceMagnitude = 10.0f;

    playerObject = nullptr;
    fragileItem = nullptr;
    pressureButton = nullptr;
    puzzleDoor = nullptr;
    deliveryZone = nullptr;
    

    itemCarried = false;
    itemBroken = false;
    grappling = false;

    breakImpulseThreshold = 30.0f;
    score = 0;
    deliveredItems = 0;
    totalItems = 1;

    enemySpeed = 5.0f;
    enemyChaseSpeed = 10.0f;

    cubeMesh = renderer.LoadMesh("cube.msh");
    sphereMesh = renderer.LoadMesh("sphere.msh");
    catMesh = renderer.LoadMesh("ORIGAMI_Chat.msh");
    kittenMesh = renderer.LoadMesh("Kitten.msh");
    enemyMesh = renderer.LoadMesh("Keeper.msh");
    bonusMesh = renderer.LoadMesh("19463_Kitten_Head_v1.msh");
    capsuleMesh = renderer.LoadMesh("capsule.msh");

    
    defaultTex = renderer.LoadTexture("Default.png");
    checkerTex = renderer.LoadTexture("checkerboard.png");
    glassTex = renderer.LoadTexture("stainedglass.tga");

    
    checkerMaterial.type = MaterialType::Opaque;
    checkerMaterial.diffuseTex = checkerTex;

    glassMaterial.type = MaterialType::Transparent;
    glassMaterial.diffuseTex = glassTex;

    notexMaterial.type = MaterialType::Opaque;
    notexMaterial.diffuseTex = nullptr;

    InitCamera();
    inMenu = true;
}

TutorialGame::~TutorialGame() {

    for (auto n : navigationNodes) {
        delete n;
    }

    navigationNodes.clear();
}



void TutorialGame::InitCamera() {
    auto& cam = world.GetMainCamera();

    cam.SetNearPlane(0.1f);
    cam.SetFarPlane(500.0f);

    cam.SetPitch(-10.0f);
    cam.SetYaw(0.0f);        

    cam.SetPosition(Vector3(0, 15, 80));  
}




void TutorialGame::InitWorld() {
    world.ClearAndErase();
    physics.Clear();

    
    physics.SetGravity(Vector3(0, -9.8f, 0));
    

    physics.SetCollisionCallback(
        [this](GameObject* a, GameObject* b, const CollisionDetection::CollisionInfo& info)
        {
            if (a == fragileItem) CheckItemBreak(a, info);
            if (b == fragileItem) CheckItemBreak(b, info);
        }
    );

    
    InitCourierLevel();
    BuildNavigationGraph();

   
}



void TutorialGame::StartGame(int enemyCount) {

    selectedEnemyCount = enemyCount;
inMenu = false;

enemies.clear();
bonuses.clear();

selectionObject = nullptr;
playerObject = nullptr;
fragileItem = nullptr;
deliveryZone = nullptr;
pressureButton = nullptr;
puzzleDoor = nullptr;

world.ClearAndErase();
physics.Clear();

    
    itemCarried = false;
    itemBroken = false;
    score = 0;
    deliveredItems = 0;

    endReason = "";
    gameWon = false;
    gameLost = false;
    gameEnded = false;

    InitCourierLevel();
    BuildNavigationGraph();
    SpawnBonuses();

   
    if (enemyCount > 0) {

        for (auto& e : enemies) {
            e.path.clear();
        }

        enemies.clear();

      
        if (enemyCount >= 1) {

            EnemyData e{};
            e.state = EnemyState::Patrol;
            e.patrolIndex = 0;
            e.currentPathIndex = 0;
            e.waitingAtNode = false;

            e.enemyObject =
                AddEnemyToWorld(Vector3(10, 3, -10));

            e.forward = Vector3(0, 0, 1);

            enemies.push_back(e);
        }

        if (enemyCount >= 2) {

            EnemyData e{};
            e.state = EnemyState::Patrol;
            e.patrolIndex = 0;
            e.currentPathIndex = 0;
            e.waitingAtNode = false;

            e.enemyObject =
                AddEnemyToWorld(Vector3(-10, 3, -10));

            e.forward = Vector3(0, 0, 1);

            enemies.push_back(e);
        }

        if (enemyCount >= 3) {

            EnemyData e{};
            e.state = EnemyState::Patrol;
            e.patrolIndex = 0;
            e.currentPathIndex = 0;
            e.waitingAtNode = false;

            e.enemyObject =
                AddEnemyToWorld(Vector3(15, 3, -15));

            e.forward = Vector3(0, 0, 1);

            enemies.push_back(e);
        }

        InitEnemyAI();
    }
}





void TutorialGame::InitCourierLevel() {
    AddFloorToWorld(Vector3(0, -2, 0));

    

    float H = 2.5f;
    
    float outerT = 2.0f;
    float innerT = 1.0f;

    
    AddCubeToWorld(Vector3(0, H, -40), Vector3(40, H, outerT), 0);
    AddCubeToWorld(Vector3(0, H, 40), Vector3(40, H, outerT), 0);
    AddCubeToWorld(Vector3(-40, H, 0), Vector3(outerT, H, 40), 0);
    AddCubeToWorld(Vector3(40, H, 0), Vector3(outerT, H, 40), 0);

   
    AddCubeToWorld(Vector3(-16, H, 37), Vector3(6, H, outerT), 0);
    AddCubeToWorld(Vector3(16, H, 37), Vector3(6, H, outerT), 0);

    
    AddCubeToWorld(Vector3(-18, H, -30), Vector3(8, H, innerT), 0);
    AddCubeToWorld(Vector3(18, H, -30), Vector3(8, H, innerT), 0);

    AddCubeToWorld(Vector3(-18, H, -15), Vector3(8, H, innerT), 0);
    AddCubeToWorld(Vector3(18, H, -15), Vector3(8, H, innerT), 0);

    
    AddCubeToWorld(Vector3(-30, H, -14), Vector3(innerT, H, 8), 0);
    AddCubeToWorld(Vector3(-30, H, 14), Vector3(innerT, H, 8), 0);

    AddCubeToWorld(Vector3(-15, H, -14), Vector3(innerT, H, 8), 0);
    AddCubeToWorld(Vector3(-15, H, 14), Vector3(innerT, H, 8), 0);

    
    AddCubeToWorld(Vector3(15, H, -14), Vector3(innerT, H, 8), 0);
    AddCubeToWorld(Vector3(15, H, 14), Vector3(innerT, H, 8), 0);

    AddCubeToWorld(Vector3(30, H, -14), Vector3(innerT, H, 8), 0);
    AddCubeToWorld(Vector3(30, H, 14), Vector3(innerT, H, 8), 0);

    
    AddCubeToWorld(Vector3(0, H, -18), Vector3(innerT, H, 2), 0);
    AddCubeToWorld(Vector3(0, H, 18), Vector3(innerT, H, 2), 0);

    AddCubeToWorld(Vector3(-22, H, 0), Vector3(4, H, innerT), 0);
    AddCubeToWorld(Vector3(22, H, 0), Vector3(4, H, innerT), 0);


    
    AddCubeToWorld(Vector3(-24, H, -8), Vector3(5, H, innerT), 0);
    AddCubeToWorld(Vector3(-24, H, 8), Vector3(5, H, innerT), 0);

    AddCubeToWorld(Vector3(24, H, -8), Vector3(5, H, innerT), 0);
    AddCubeToWorld(Vector3(24, H, 8), Vector3(5, H, innerT), 0);


    
    AddCubeToWorld(Vector3(-18, H, 26), Vector3(innerT, H, 4), 0);
    AddCubeToWorld(Vector3(18, H, 26), Vector3(innerT, H, 4), 0);

    
    AddCubeToWorld(Vector3(-18, H, -26), Vector3(innerT, H, 4), 0);
    AddCubeToWorld(Vector3(18, H, -26), Vector3(innerT, H, 4), 0);


    
    AddCubeToWorld(Vector3(-8, H, 22), Vector3(6, H, innerT), 0);
    AddCubeToWorld(Vector3(8, H, 22), Vector3(6, H, innerT), 0);

    AddCubeToWorld(Vector3(-8, H, -22), Vector3(6, H, innerT), 0);
    AddCubeToWorld(Vector3(8, H, -22), Vector3(6, H, innerT), 0);

    
    AddCubeToWorld(Vector3(-16, H, 30), Vector3(innerT, H, 6), 0);
    AddCubeToWorld(Vector3(16, H, 30), Vector3(innerT, H, 6), 0);


    
    pressureButton = AddCubeToWorld(Vector3(-28, 1, -28), Vector3(1, 0.5f, 1), 0);
    pressureButton->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));

    puzzleDoor = AddOBBToWorld(Vector3(0, 5, 8), Vector3(8, 5, 1), 0);
    puzzleDoor->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));

    
    playerObject = AddPlayerToWorld(Vector3(0, 4, 25));

    
    fragileItem = AddFragileItemToWorld(Vector3(28, 2, -28));
    fragileItem->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

    
    deliveryZone = AddDeliveryZone(Vector3(24, 1, 24), Vector3(3, 1, 3));
    AddDeliveryMarker(Vector3(24, 4, 24));

    
}





GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
    GameObject* floor = new GameObject();

    Vector3 size = Vector3(200, 2, 200);

    AABBVolume* volume = new AABBVolume(size);
    floor->SetBoundingVolume(volume);

    floor->GetTransform()
        .SetScale(size * 2.0f)
        .SetPosition(position);

    floor->SetRenderObject(new RenderObject(floor->GetTransform(), cubeMesh, checkerMaterial));
    floor->SetPhysicsObject(new PhysicsObject(floor->GetTransform(), floor->GetBoundingVolume()));

    floor->GetPhysicsObject()->SetInverseMass(0);
    floor->GetPhysicsObject()->InitCubeInertia();

    world.AddGameObject(floor);
    return floor;
}





GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
    GameObject* sphere = new GameObject();

    SphereVolume* volume = new SphereVolume(radius);
    sphere->SetBoundingVolume(volume);

    sphere->GetTransform()
        .SetScale(Vector3(radius, radius, radius))
        .SetPosition(position);

    sphere->SetRenderObject(new RenderObject(sphere->GetTransform(), sphereMesh, checkerMaterial));
    sphere->SetPhysicsObject(new PhysicsObject(sphere->GetTransform(), sphere->GetBoundingVolume()));

    sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
    sphere->GetPhysicsObject()->InitSphereInertia();

    world.AddGameObject(sphere);
    return sphere;
}


GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
    GameObject* cube = new GameObject();

    AABBVolume* volume = new AABBVolume(dimensions);
    cube->SetBoundingVolume(volume);

    cube->GetTransform()
        .SetScale(dimensions * 2.0f)
        .SetPosition(position);

    cube->SetRenderObject(new RenderObject(cube->GetTransform(), cubeMesh, checkerMaterial));
    cube->SetPhysicsObject(new PhysicsObject(cube->GetTransform(), cube->GetBoundingVolume()));

    cube->GetPhysicsObject()->SetInverseMass(inverseMass);
    cube->GetPhysicsObject()->InitCubeInertia();

    world.AddGameObject(cube);
    return cube;
}


GameObject* TutorialGame::AddOBBToWorld(
    const Vector3& pos,
    const Vector3& halfDims,
    float inverseMass,
    float pitch, float yaw, float roll)
{
    GameObject* obj = new GameObject();

    OBBVolume* volume = new OBBVolume(halfDims);
    obj->SetBoundingVolume(volume);

    obj->GetTransform()
        .SetPosition(pos)
        .SetScale(halfDims * 2.0f)
        .SetOrientation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll));

    obj->SetRenderObject(new RenderObject(obj->GetTransform(), cubeMesh, checkerMaterial));
    obj->SetPhysicsObject(new PhysicsObject(obj->GetTransform(), obj->GetBoundingVolume()));

    obj->GetPhysicsObject()->SetInverseMass(inverseMass);
    obj->GetPhysicsObject()->InitCubeInertia();

    world.AddGameObject(obj);
    return obj;
}




GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
    GameObject* player = new GameObject("Player");

    float radius = 1.2f;

    SphereVolume* volume = new SphereVolume(radius);
    player->SetBoundingVolume(volume);

    player->GetTransform()
        .SetScale(Vector3(radius, radius, radius))
        .SetPosition(position);

    RenderObject* ro =
        new RenderObject(
            player->GetTransform(),
            sphereMesh,
            checkerMaterial
        );

    ro->SetColour(Vector4(0, 0, 1, 1));
    player->SetRenderObject(ro);

    PhysicsObject* po =
        new PhysicsObject(
            player->GetTransform(),
            player->GetBoundingVolume()
        );

    po->SetInverseMass(1.0f);
    po->SetElasticity(0.6f);
    po->InitSphereInertia();

    player->SetPhysicsObject(po);

    world.AddGameObject(player);
    return player;
}



GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
    GameObject* enemy = new GameObject("Enemy");

    SphereVolume* volume = new SphereVolume(1.2f);
    enemy->SetBoundingVolume(volume);

    enemy->GetTransform()
        .SetScale(Vector3(2.4f, 2.4f, 2.4f))
        .SetPosition(position);

    RenderObject* ro = new RenderObject(enemy->GetTransform(), enemyMesh, notexMaterial);
    ro->SetColour(Vector4(1, 1, 0, 1));
    enemy->SetRenderObject(ro);

    PhysicsObject* po = new PhysicsObject(enemy->GetTransform(), enemy->GetBoundingVolume());
    po->SetInverseMass(1.0f);
    po->InitSphereInertia();
    po->SetElasticity(0.6f);

    enemy->SetPhysicsObject(po);

    world.AddGameObject(enemy);
    return enemy;
}









GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
    GameObject* bonus = new GameObject();

    SphereVolume* volume = new SphereVolume(0.5f);
    bonus->SetBoundingVolume(volume);

    bonus->GetTransform()
        .SetScale(Vector3(2, 2, 2))
        .SetPosition(position);

    bonus->SetRenderObject(new RenderObject(bonus->GetTransform(), bonusMesh, glassMaterial));
    bonus->SetPhysicsObject(new PhysicsObject(bonus->GetTransform(), bonus->GetBoundingVolume()));

    bonus->GetPhysicsObject()->SetInverseMass(1.0f);
    bonus->GetPhysicsObject()->InitSphereInertia();

    world.AddGameObject(bonus);
    return bonus;
}

void TutorialGame::SpawnBonuses() {

    bonuses.clear();

    if (navigationNodes.empty())
        return;

    for (int i = 0; i < 5; ++i) {

        

        NavigationNode* node = nullptr;

        while (!node) {
            int nodeIndex = rand() % navigationNodes.size();

            Vector3 testPos = navigationNodes[nodeIndex]->position;

            
            if (fabs(testPos.x) > 24 || fabs(testPos.z) > 24)
                continue;

            node = navigationNodes[nodeIndex];
        }

        Vector3 pos = node->position;
        pos.y = 3.0f;

        BonusItem b;

        b.object = AddBonusToWorld(pos);



        int type = rand() % 3;

        if (type == 0) {

            b.type = BonusType::SpeedBoost;

            b.object->GetRenderObject()->SetColour(
                Vector4(0, 1, 0, 1)
            );
        }

        if (type == 1) {

            b.type = BonusType::FreezeEnemy;

            b.object->GetRenderObject()->SetColour(
                Vector4(0, 0, 1, 1)
            );
        }

        if (type == 2) {

            b.type = BonusType::HeavyPlayer;

            b.object->GetRenderObject()->SetColour(
                Vector4(1, 0, 1, 1)
            );
        }

        bonuses.push_back(b);
    }
}


GameObject* TutorialGame::AddFragileItemToWorld(const Vector3& position) {
    
    GameObject* item = new GameObject("FragileItem");

    
    float radius = 1.0f;

    
    SphereVolume* volume = new SphereVolume(radius);
    item->SetBoundingVolume(volume);

    
    item->GetTransform()
        .SetScale(Vector3(1.0f, 1.0f, 1.0f))
        .SetPosition(position);

    
    RenderObject* ro = new RenderObject(item->GetTransform(), sphereMesh, glassMaterial);
    ro->SetColour(Vector4(1, 0, 0, 1));   
    item->SetRenderObject(ro);

    
    PhysicsObject* po = new PhysicsObject(item->GetTransform(), item->GetBoundingVolume());
    item->SetPhysicsObject(po);

    po->SetInverseMass(1.0f);
    po->InitSphereInertia();
    po->SetElasticity(0.6f);
    

    
    world.AddGameObject(item);
    return item;
}










GameObject* TutorialGame::AddDeliveryZone(const Vector3& pos, const Vector3& size) {
    GameObject* zone = new GameObject();

    AABBVolume* volume = new AABBVolume(size);
    zone->SetBoundingVolume(volume);

    zone->GetTransform()
        .SetScale(size * 2.0f)
        .SetPosition(pos);

    RenderObject* ro = new RenderObject(zone->GetTransform(), cubeMesh, glassMaterial);
    ro->SetColour(Vector4(0, 0, 1, 0.25f));
    zone->SetRenderObject(ro);

    PhysicsObject* po = new PhysicsObject(zone->GetTransform(), zone->GetBoundingVolume());
    po->SetInverseMass(0);
    zone->SetPhysicsObject(po);

    world.AddGameObject(zone);
    return zone;
}





GameObject* TutorialGame::AddDeliveryMarker(const Vector3& pos) {
    GameObject* m = new GameObject("DeliveryMarker");

    SphereVolume* volume = new SphereVolume(0.5f);
    m->SetBoundingVolume(volume);

    m->GetTransform()
        .SetScale(Vector3(3.0f, 3.0f, 3.0f))
        .SetPosition(pos);

    RenderObject* ro = new RenderObject(
        m->GetTransform(),
        sphereMesh,        
        notexMaterial      
    );

    ro->SetColour(Vector4(1, 0, 0, 1)); 
    m->SetRenderObject(ro);

    PhysicsObject* po = new PhysicsObject(m->GetTransform(), m->GetBoundingVolume());
    po->SetInverseMass(0); 
    m->SetPhysicsObject(po);

    world.AddGameObject(m);
    return m;
}





void TutorialGame::UpdateFragileItem(float dt) {
    if (!fragileItem || itemBroken || !playerObject) return;

    Vector3 p = playerObject->GetTransform().GetPosition();
    Vector3 i = fragileItem->GetTransform().GetPosition();

    Debug::DrawLine(
        i + Vector3(0, 3, 0),
        i + Vector3(0, 6, 0),
        Debug::RED
    );

    float dist = sqrt((i.x - p.x) * (i.x - p.x) + (i.y - p.y) * (i.y - p.y) + (i.z - p.z) * (i.z - p.z));

    if (!itemCarried && dist < 12.0f && Window::GetKeyboard()->KeyPressed(KeyCodes::F))
        itemCarried = true;

    if (itemCarried && Window::GetKeyboard()->KeyPressed(KeyCodes::G)) {
        itemCarried = false;
        fragileItem->GetPhysicsObject()->SetLinearVelocity(Vector3());
        return;
    }

    if (itemCarried) {
        Vector3 targetPos = p + Vector3(0, 3, 0);

        Vector3 dir = targetPos - i;
        fragileItem->GetPhysicsObject()->SetLinearVelocity(dir * 8.0f);

        i = fragileItem->GetTransform().GetPosition();

        i.x = std::max(-playLimit, std::min(playLimit, i.x));
        i.z = std::max(-playLimit, std::min(playLimit, i.z));

        fragileItem->GetTransform().SetPosition(i);
    }
}

void TutorialGame::UpdateDelivery(float dt) {
    if (!fragileItem || !deliveryZone || itemBroken) return;

    Vector3 a = fragileItem->GetTransform().GetPosition();
    Vector3 b = deliveryZone->GetTransform().GetPosition();

    float dist = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));

    if (dist < 8.0f) {

        deliveredItems++;
        score += 10;
        itemCarried = false;

        fragileItem->GetTransform().SetPosition(Vector3(0, -200, 0));
        fragileItem->GetPhysicsObject()->SetLinearVelocity(Vector3());

        
        if (deliveredItems >= totalItems) {

            gameWon = true;
            gameEnded = true;
            endReason = "Delivery Complete!";
        }
    }
}





void TutorialGame::CheckItemBreak(GameObject* item, const CollisionDetection::CollisionInfo& info) {
    if (!item || itemBroken)
        return;

    PhysicsObject* a = info.a->GetPhysicsObject();
    PhysicsObject* b = info.b->GetPhysicsObject();

    if (!a || !b) return;

    float impact = Vector::Length(
        a->GetLinearVelocity() - b->GetLinearVelocity()
    );

    if (impact > breakImpulseThreshold) {
        itemBroken = true;
        itemCarried = false;

        gameLost = true;
        gameEnded = true;
        endReason = "Fragile Item Destroyed!";

        item->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

        Debug::Print("FRAGILE ITEM BROKEN!", Vector2(40, 50), Debug::RED);
    }
}


void TutorialGame::InitPuzzleObjects() {
    pressureButton = AddCubeToWorld(Vector3(5, 0.5f, 5), Vector3(1, 0.25f, 1), 0);
    pressureButton->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

    puzzleDoor = AddOBBToWorld(Vector3(5, 5, 8), Vector3(3, 5, 1), 0);
    puzzleDoor->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
}




void TutorialGame::UpdatePuzzleLogic(float dt) {
    if (!pressureButton || !puzzleDoor) return;

    bool pressed = false;
    Vector3 btnPos = pressureButton->GetTransform().GetPosition();

    world.OperateOnContents([&](GameObject* o) {
        if (o == pressureButton) return;

        float dist = Vector::Length(o->GetTransform().GetPosition() - btnPos);
        if (dist < 2.0f)
            pressed = true;
        });

    if (pressed && !doorOpen) {
        doorOpen = true;
        puzzleDoor->GetTransform().SetPosition(
            puzzleDoor->GetTransform().GetPosition() + Vector3(0, 12, 0)
        );
    }

    if (!pressed && doorOpen) {
        doorOpen = false;

        
        puzzleDoor->GetTransform().SetPosition(Vector3(0, 5, 8));
    }
}



void TutorialGame::InitEnemyAI() {

    enemyPatrolPoints.clear();

    

    
    enemyPatrolPoints.push_back(
        Vector3(-30, 2, -30)
    );

   
    enemyPatrolPoints.push_back(
        Vector3(30, 2, -30)
    );

    
    enemyPatrolPoints.push_back(
        Vector3(30, 2, 30)
    );

    
    enemyPatrolPoints.push_back(
        Vector3(-30, 2, 30)
    );

    
    enemyPatrolPoints.push_back(
        Vector3(0, 2, 0)
    );
}




void TutorialGame::UpdateEnemyAI(float dt) {

    if (enemies.empty() || !playerObject)
        return;

    pathUpdateTimer += dt;

    for (EnemyData& enemy : enemies) {

        if (!enemy.enemyObject)
            continue;

        if (enemy.waitingAtNode) {

            enemy.patrolWaitTimer -= dt;

            if (enemy.patrolWaitTimer <= 0.0f) {

                enemy.waitingAtNode = false;

                enemy.patrolIndex++;
                enemy.patrolIndex %= enemyPatrolPoints.size();

                enemy.path.clear();
                enemy.currentPathIndex = 0;
            }

            continue;
        }

        Vector3 enemyPos =
            enemy.enemyObject->GetTransform().GetPosition();

        Vector3 playerPos =
            playerObject->GetTransform().GetPosition();

        float playerDist =
            Vector::Length(playerPos - enemyPos);

        
        if (playerDist < 4.5f) {

            gameLost = true;
            gameEnded = true;
            endReason = "Caught by Enemy!";

            playerObject->GetPhysicsObject()
                ->SetLinearVelocity(Vector3());

            enemy.enemyObject->GetPhysicsObject()
                ->SetLinearVelocity(Vector3());

            continue;
        }

        

        EnemyBonusDecision(enemy);

        switch (enemy.state) {

        case EnemyState::Patrol:

            if (CanEnemySeePlayer(enemy)) {

                enemy.state = EnemyState::Chase;
            }

            break;

        case EnemyState::Chase:

            enemy.lastKnownPlayerPos = playerPos;

            if (playerDist > losePlayerDistance) {

                enemy.state = EnemyState::Search;

                enemy.searchTimer = 4.0f;
            }


            break;

        case EnemyState::Search:

            if (playerDist < enemyDetectRange) {

                enemy.state = EnemyState::Chase;
            }

            enemy.searchTimer -= dt;

            if (enemy.searchTimer <= 0.0f) {

                enemy.state = EnemyState::Return;
            }

            break;

        case EnemyState::Return:

            if (CanEnemySeePlayer(enemy)) {
                enemy.state = EnemyState::Chase;
            }

            break;
        }

        

        Vector3 targetPos;

        switch (enemy.state) {

        case EnemyState::Patrol:

            targetPos =
                enemyPatrolPoints[enemy.patrolIndex];

            if (Vector::Length(targetPos - enemyPos) < 3.0f) {

                if (!enemy.waitingAtNode) {
                    enemy.waitingAtNode = true;
                    enemy.patrolWaitTimer = 2.0f;
                }

                enemy.enemyObject->GetPhysicsObject()
                    ->SetLinearVelocity(Vector3());
            }

            break;

        case EnemyState::Chase:

            targetPos = playerPos;
            break;

        case EnemyState::Search:

            targetPos = enemy.lastKnownPlayerPos;

            if (Vector::Length(targetPos - enemyPos) < 3.0f) {

                enemy.state = EnemyState::Return;
            }

            break;

        case EnemyState::Return:

            targetPos =
                enemyPatrolPoints[enemy.patrolIndex];

            if (Vector::Length(targetPos - enemyPos) < 3.0f) {

                enemy.state = EnemyState::Patrol;
            }

            break;
        }

        

        if (pathUpdateTimer >= pathUpdateInterval) {

            NavigationNode* start =
                GetClosestNode(enemyPos);

            NavigationNode* goal =
                GetClosestNode(targetPos);

            if (start && goal) {

                enemy.path =
                    Pathfinding::FindPath(start, goal);

                enemy.currentPathIndex = 0;
            }
        }

        

        if (enemy.path.empty()) {

            Vector3 dir = targetPos - enemyPos;
            dir.y = 0;

            float dist = Vector::Length(dir);

            if (dist > 1.0f) {

                dir = Vector::Normalise(dir);

                enemy.forward = dir;

                float speed = enemySpeed;

                
                if (freezeEnemyActive) {
                    speed = 0.0f;
                }
                else if (enemy.state == EnemyState::Chase) {
                    speed = enemyChaseSpeed;
                }

                enemy.enemyObject
                    ->GetPhysicsObject()
                    ->SetLinearVelocity(dir * speed);
            }

            continue;
        }

        if (enemy.currentPathIndex >= enemy.path.size())
            continue;

        Vector3 nextNode =
            enemy.path[enemy.currentPathIndex]->position;

        Vector3 dir =
            nextNode - enemyPos;

        dir.y = 0;

        float dist = Vector::Length(dir);

        if (dist < 1.2f) {

            enemy.currentPathIndex++;
            continue;
        }

        dir = Vector::Normalise(dir);

        enemy.forward =
            enemy.forward +
            (dir - enemy.forward) * (dt * 4.0f);

        enemy.forward = Vector::Normalise(enemy.forward);

        PhysicsObject* phys =
            enemy.enemyObject->GetPhysicsObject();

        float speed = enemySpeed;

        
        if (freezeEnemyActive) {
            speed = 0.0f;
        }
        else if (enemy.state == EnemyState::Chase) {
            speed =
                enemyChaseSpeed +
                (10.0f / std::max(dist, 1.0f));
        }

        Vector3 avoidance =
            CalculateAvoidanceForce(enemy);
        Vector3 finalDir = (dir * 3.0f) + avoidance;

        finalDir = Vector::Normalise(finalDir);

        Vector3 velocity =
            finalDir * speed;

        if (dist < 6.0f) {
            velocity *= 0.5f;
        }

        velocity.y = phys->GetLinearVelocity().y;

        phys->SetLinearVelocity(velocity);

       

        Vector3 v =
            phys->GetLinearVelocity();

        float maxSpeed = 8.0f;

        if (enemy.state == EnemyState::Chase)
            maxSpeed = 16.0f;

        v.x = std::max(std::min(v.x, maxSpeed), -maxSpeed);
        v.z = std::max(std::min(v.z, maxSpeed), -maxSpeed);

        phys->SetLinearVelocity(v);
        Vector3 ep =
            enemy.enemyObject->GetTransform().GetPosition();

        ep.x = std::max(-playLimit, std::min(playLimit, ep.x));
        ep.z = std::max(-playLimit, std::min(playLimit, ep.z));

        enemy.enemyObject->GetTransform().SetPosition(ep);
}

    
    if (pathUpdateTimer >= pathUpdateInterval) {

        pathUpdateTimer = 0.0f;
    }

}


void TutorialGame::EnemyBonusDecision(EnemyData& enemy) {

    if (!enemy.enemyObject)
        return;

    Vector3 enemyPos =
        enemy.enemyObject->GetTransform().GetPosition();

    float closestBonusDist = FLT_MAX;

    BonusItem* closestBonus = nullptr;

    for (auto& b : bonuses) {

        if (!b.active || !b.object)
            continue;

        float dist =
            Vector::Length(
                b.object->GetTransform().GetPosition()
                - enemyPos
            );

        if (dist < closestBonusDist) {

            closestBonusDist = dist;
            closestBonus = &b;
        }
    }

    if (!closestBonus)
        return;

    Vector3 playerPos =
        playerObject->GetTransform().GetPosition();

    float playerDist =
        Vector::Length(playerPos - enemyPos);

   

    if (closestBonusDist < playerDist * 0.7f) {

        enemy.lastKnownPlayerPos =
            closestBonus->object->GetTransform().GetPosition();

        enemy.state = EnemyState::Search;
    }
}


Vector3 TutorialGame::CalculateAvoidanceForce(EnemyData& enemy) {

    Vector3 avoidance(0, 0, 0);

    if (!enemy.enemyObject)
        return avoidance;

    Vector3 enemyPos =
        enemy.enemyObject->GetTransform().GetPosition();

    world.OperateOnContents([&](GameObject* o) {

        if (!o)
            return;

        if (o == enemy.enemyObject)
            return;

        if (o == playerObject)
            return;

        PhysicsObject* phys = o->GetPhysicsObject();

        if (!phys)
            return;

        Vector3 objPos =
            o->GetTransform().GetPosition();

        Vector3 toEnemy =
            enemyPos - objPos;

        float dist =
            Vector::Length(toEnemy);

        

        float avoidRadius = 5.0f;

        if (dist < avoidRadius && dist > 0.01f) {

            Vector3 pushDir =
                Vector::Normalise(toEnemy);

            float strength =
                (avoidRadius - dist) / avoidRadius;

            avoidance += pushDir * strength;
        }
        });

   
    avoidance.y = 0;

    return avoidance * 8.0f;
}


void TutorialGame::UpdateBonuses(float dt) {

    if (!playerObject)
        return;

    Vector3 playerPos =
        playerObject->GetTransform().GetPosition();

    
    for (auto& b : bonuses) {

        if (!b.active || !b.object)
            continue;

        Vector3 pos =
            b.object->GetTransform().GetPosition();

        float dist =
            Vector::Length(playerPos - pos);

        if (dist < 3.0f) {

            b.active = false;

            b.object->GetTransform().SetPosition(
                Vector3(0, -100, 0)
            );

            

            if (b.type == BonusType::SpeedBoost) {
                speedBoostActive = true;
                bonusTimer = 5.0f;
                playerSpeedMultiplier = 1.8f;
            }

            if (b.type == BonusType::FreezeEnemy) {
                freezeEnemyActive = true;
                bonusTimer = 5.0f;
            }

            if (b.type == BonusType::HeavyPlayer) {
                heavyPlayerActive = true;
                bonusTimer = 5.0f;

                playerObject->GetPhysicsObject()
                    ->SetInverseMass(0.2f);
            }
        }
    }

    
    if (bonusTimer > 0.0f) {
        bonusTimer -= dt;
    }

    
    if (bonusTimer <= 0.0f) {

        if (speedBoostActive) {
            playerSpeedMultiplier = 1.0f;
            speedBoostActive = false;
        }

        if (freezeEnemyActive) {
            enemyChaseSpeed = 20.0f;
            freezeEnemyActive = false;
        }

        if (heavyPlayerActive) {
            playerObject->GetPhysicsObject()
                ->SetInverseMass(1.0f);

            heavyPlayerActive = false;
        }
    }
}


void TutorialGame::UpdateGrapple(float dt) {
    if (!playerObject) return;

    const Keyboard* kb = Window::GetKeyboard();

    if (kb->KeyPressed(KeyCodes::E)) {
        Ray r = CollisionDetection::BuildRayFromMouse(world.GetMainCamera());
        r = Ray(r.GetPosition() + Vector3(0, 4, 0), r.GetDirection());

        RayCollision rc;
        if (world.Raycast(r, rc, true)) {
            if (rc.node != playerObject && rc.node != fragileItem) {
                grappling = true;
                grapplePoint = rc.collidedAt;
            }
        }
    }

    if (kb->KeyPressed(KeyCodes::R))
        grappling = false;

    if (!grappling) return;

    Vector3 p = playerObject->GetTransform().GetPosition();
    Vector3 dir = grapplePoint - p;

    float dist = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (dist < 1.0f) { grappling = false; return; }

    dir = dir * (1.0f / dist);

    playerObject->GetPhysicsObject()->AddForce(dir * 80.0f);
}














void TutorialGame::PlayerMovement(float dt) {
    if (!playerObject) return;

    PhysicsObject* phys = playerObject->GetPhysicsObject();
    if (!phys) return;

    float moveForce = 70.0f * playerSpeedMultiplier;
    const float jumpForce = 250.0f;

    Matrix4 view = world.GetMainCamera().BuildViewMatrix();
    Matrix4 camWorld = Matrix::Inverse(view);

    Vector3 camRight = Vector3(camWorld.GetColumn(0));
    Vector3 camForward = Vector3(camWorld.GetColumn(2)) * -1.0f;

    camRight.y = camForward.y = 0;
    camRight = Vector::Normalise(camRight);
    camForward = Vector::Normalise(camForward);

    

    Vector3 force(0, 0, 0);
    auto* kb = Window::GetKeyboard();

    if (kb->KeyDown(KeyCodes::W)) force += camForward * moveForce;
    if (kb->KeyDown(KeyCodes::S)) force -= camForward * moveForce;
    if (kb->KeyDown(KeyCodes::A)) force -= camRight * moveForce;
    if (kb->KeyDown(KeyCodes::D)) force += camRight * moveForce;

    if (kb->KeyPressed(KeyCodes::I))
        phys->AddForce(Vector3(0, jumpForce, 0));

    if (kb->KeyDown(KeyCodes::K)) {
        Vector3 p = playerObject->GetTransform().GetPosition();
        p.y -= 5.0f * dt;
        playerObject->GetTransform().SetPosition(p);
    }
    
    
    if (Vector::Length(force) > 0.0f) {
        force = Vector::Normalise(force) * moveForce;

        phys->AddForce(force);

        
        Vector3 torque =
            Vector::Cross(
                Vector3(0, 1, 0),
                force
            ) * 0.4f;

        phys->AddTorque(torque);
    }


   
    Vector3 v = phys->GetLinearVelocity();
    float maxSpeed = 6.0f;

    
    v.x = std::max(std::min(v.x, maxSpeed), -maxSpeed);
    v.z = std::max(std::min(v.z, maxSpeed), -maxSpeed);

    
    Vector3 p = playerObject->GetTransform().GetPosition();


    if (fabs(v.x) > 0.01f || fabs(v.z) > 0.01f) {

        Vector3 moveDir =
            Vector::Normalise(
                Vector3(v.x, 0, v.z)
            );

        Ray forwardRay(
            p + Vector3(0, 1.0f, 0),
            moveDir
        );

        RayCollision rc;

        if (world.Raycast(forwardRay, rc, true)) {

            if (rc.node != playerObject &&
                rc.rayDistance < 2.2f)
            {
                v.x = -moveDir.x * 2.5f;
                v.z = -moveDir.z * 2.5f;

                p -= moveDir * 0.5f;

                playerObject->GetTransform().SetPosition(p);
            }
        }
    }

    
    phys->SetLinearVelocity(v);

   
    p.x = std::max(-playLimit, std::min(playLimit, p.x));
    p.z = std::max(-playLimit, std::min(playLimit, p.z));

    playerObject->GetTransform().SetPosition(p);

}








void TutorialGame::DrawHUD() {
    int y = 5;

    
    Debug::Print(
        "Score: " + std::to_string(score),
        Vector2(5, y),
        Debug::YELLOW
    );
    y += 5;

    Debug::Print(
        "Delivered: " +
        std::to_string(deliveredItems) +
        "/" +
        std::to_string(totalItems),
        Vector2(5, y),
        Debug::YELLOW
    );
    y += 5;

    
    if (bonusTimer > 0.0f) {
        Debug::Print(
            "Bonus Time: " +
            std::to_string((int)bonusTimer),
            Vector2(5, y),
            Debug::CYAN
        );
        y += 5;
    }

    
    if (itemCarried) {
        Debug::Print(
            "Deliver to RED MARKER (G = Drop)",
            Vector2(5, y),
            Debug::RED
        );
        y += 5;

        Debug::Print(
            "Carrying Item (G to drop)",
            Vector2(5, y),
            Debug::WHITE
        );
        y += 5;
    }

    
    if (itemBroken) {
        Debug::Print(
            "ITEM BROKEN!",
            Vector2(5, y),
            Debug::RED
        );
        y += 5;
    }

    
    if (grappling) {
        Debug::Print(
            "Grapple Active",
            Vector2(5, y),
            Debug::CYAN
        );
        y += 5;
    }

    
    if (fragileItem && !itemBroken && playerObject) {

        Vector3 playerPos =
            playerObject->GetTransform().GetPosition();

        Vector3 itemPos =
            fragileItem->GetTransform().GetPosition();

        float dist =
            Vector::Length(itemPos - playerPos);

        Debug::Print(
            "Item Dist: " +
            std::to_string(dist),
            Vector2(5, y),
            Debug::WHITE
        );
        y += 5;

        if (deliveryZone) {

            float deliveryDist =
                Vector::Length(
                    fragileItem->GetTransform().GetPosition() -
                    deliveryZone->GetTransform().GetPosition()
                );

            Debug::Print(
                "Delivery Dist: " +
                std::to_string(deliveryDist),
                Vector2(5, y),
                Debug::GREEN
            );
            y += 5;
        }

        
        Vector3 dir =
            Vector::Normalise(itemPos - playerPos);

        Debug::DrawLine(
            playerPos + Vector3(0, 2, 0),
            playerPos + dir * 5.0f,
            Debug::RED
        );
    }

    
    std::string stateText = "NONE";

    if (!enemies.empty()) {

        EnemyState s = enemies[0].state;

        if (s == EnemyState::Patrol)
            stateText = "PATROL";

        if (s == EnemyState::Chase)
            stateText = "CHASE";

        if (s == EnemyState::Search)
            stateText = "SEARCH";

        if (s == EnemyState::Return)
            stateText = "RETURN";
    }

    Debug::Print(
        "Enemy State: " + stateText,
        Vector2(5, y),
        Debug::CYAN
    );
    y += 5;

    
    Debug::Print(
        "Force: " + std::to_string(forceMagnitude),
        Vector2(5, 90),
        Debug::WHITE
    );

    
    if (gameEnded) {

        if (gameWon) {
            Debug::Print(
                "========== YOU WIN ==========",
                Vector2(22, 35),
                Debug::GREEN
            );
        }

        if (gameLost) {
            Debug::Print(
                "========== YOU LOSE ==========",
                Vector2(22, 35),
                Debug::RED
            );
        }

        Debug::Print(
            endReason,
            Vector2(28, 42),
            Debug::WHITE
        );

        Debug::Print(
            "Final Score: " +
            std::to_string(score),
            Vector2(28, 48),
            Debug::YELLOW
        );

        Debug::Print(
            "R = Restart",
            Vector2(30, 55),
            Debug::WHITE
        );

        Debug::Print(
            "M = Return To Menu",
            Vector2(26, 60),
            Debug::CYAN
        );
    }
}



void TutorialGame::DrawMenu() {

    Debug::Print(
        "=== CSC8503 AI CHASE GAME ===",
        Vector2(20, 20),
        Debug::YELLOW
    );

    Debug::Print(
        "1 = Practice Mode",
        Vector2(20, 30),
        Debug::WHITE
    );

    Debug::Print(
        "2 = 1 Enemy",
        Vector2(20, 40),
        Debug::WHITE
    );

    Debug::Print(
        "3 = 2 Enemies",
        Vector2(20, 50),
        Debug::WHITE
    );

    Debug::Print(
        "4 = 3 Enemies",
        Vector2(20, 60),
        Debug::WHITE
    );

    Debug::Print(
        "E = Return To Menu",
        Vector2(20, 70),
        Debug::CYAN
    );
}



bool TutorialGame::SelectObject() {
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::Q)) {
        inSelectionMode = !inSelectionMode;
        Window::GetWindow()->ShowOSPointer(inSelectionMode);
        Window::GetWindow()->LockMouseToWindow(!inSelectionMode);
    }

    if (!inSelectionMode) return false;

    Debug::Print("Selection Mode (Q to exit)", Vector2(5, 85));

    if (Window::GetMouse()->ButtonDown(MouseButtons::Left)) {
        if (selectionObject) {
            selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
            selectionObject = nullptr;
        }

        Ray mouseRay = CollisionDetection::BuildRayFromMouse(world.GetMainCamera());

        
        Vector3 shiftedOrigin = mouseRay.GetPosition() + Vector3(0, 5, 0);
        Ray r(shiftedOrigin, mouseRay.GetDirection());   

        RayCollision rc;


      

        if (world.Raycast(r, rc, true)) {
            selectionObject = (GameObject*)rc.node;
            selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
        }
    }

    return selectionObject != nullptr;

}


void TutorialGame::MoveSelectedObject() {
    Debug::Print("Force: " + std::to_string(forceMagnitude), Vector2(5, 90));

    forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

    if (forceMagnitude < 0)
        forceMagnitude = 0;

    if (!selectionObject) return;

    if (Window::GetMouse()->ButtonPressed(MouseButtons::Right)) {
        Ray r = CollisionDetection::BuildRayFromMouse(world.GetMainCamera());
        RayCollision rc;

        if (world.Raycast(r, rc, true)) {
            if (rc.node == selectionObject) {
                selectionObject->GetPhysicsObject()->AddForceAtPosition(
                    r.GetDirection() * forceMagnitude,
                    rc.collidedAt
                );
            }
        }
    }
}


void TutorialGame::LockedObjectMovement() {
    Matrix4 view = world.GetMainCamera().BuildViewMatrix();
    Matrix4 camWorld = Matrix::Inverse(view);

    Vector3 right = Vector3(camWorld.GetColumn(0));
    Vector3 fwd = Vector::Cross(Vector3(0, 1, 0), right);
    fwd = Vector::Normalise(fwd);

    if (Window::GetKeyboard()->KeyDown(KeyCodes::UP))
        selectionObject->GetPhysicsObject()->AddForce(fwd);

    if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN))
        selectionObject->GetPhysicsObject()->AddForce(-fwd);

    if (Window::GetKeyboard()->KeyDown(KeyCodes::NEXT))
        selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
}


void TutorialGame::DebugObjectMovement() {
    if (!inSelectionMode || !selectionObject)
        return;

    if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT))
        selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));

    if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT))
        selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));

    if (Window::GetKeyboard()->KeyDown(KeyCodes::UP))
        selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));

    if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN))
        selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
}



void TutorialGame::UpdateGame(float dt) {

  

    if (gameEnded &&
        Window::GetKeyboard()->KeyPressed(KeyCodes::R)) {

        gameWon = false;
        gameLost = false;
        gameEnded = false;
        endReason = "";

        StartGame(selectedEnemyCount);
        return;
    }

    if (inMenu) {

        DrawMenu();

        const Keyboard* kb = Window::GetKeyboard();

        if (kb->KeyPressed(KeyCodes::NUM1)) {
            StartGame(0); 
        }

        if (kb->KeyPressed(KeyCodes::NUM2)) {
            StartGame(1);
        }

        if (kb->KeyPressed(KeyCodes::NUM3)) {
            StartGame(2);
        }

        if (kb->KeyPressed(KeyCodes::NUM4)) {
            StartGame(3);
        }

        return;
    }

    
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::M)) {

        inMenu = true;

        world.ClearAndErase();
        physics.Clear();

        return;
    }

    
    if (inSelectionMode) {
        Window::GetWindow()->ShowOSPointer(true);
        Window::GetWindow()->LockMouseToWindow(false);
    }
    else {
        Window::GetWindow()->ShowOSPointer(false);
        Window::GetWindow()->LockMouseToWindow(true);
       
        controller->Update(dt);
    }

    
    if (!gameEnded) {
        PlayerMovement(dt);
        Vector3 p = playerObject->GetTransform().GetPosition();

        world.GetMainCamera().SetPosition(
            p + Vector3(0, 15, 30)
        );
    }


    if (!gameEnded) {

        UpdateEnemyAI(dt);
        UpdatePuzzleLogic(dt);
        UpdateGrapple(dt);
        UpdateFragileItem(dt);
        UpdateDelivery(dt);
        UpdateBonuses(dt);
    }

    DrawHUD();
    DrawNavigationGraph();

    
    SelectObject();
    MoveSelectedObject();

    
    if (!gameEnded) {

        world.OperateOnContents([dt](GameObject* o) {
            o->Update(dt);
            });
    }
}

void TutorialGame::BuildNavigationGraph() {
    
    for (auto n : navigationNodes) {
        delete n;
    }
    
    navigationNodes.clear();

    
    NavigationNode* n1 = new NavigationNode(Vector3(-26, 1, -26));
    NavigationNode* n2 = new NavigationNode(Vector3(-15, 1, -30));
    NavigationNode* n3 = new NavigationNode(Vector3(0, 1, -30));
    NavigationNode* n4 = new NavigationNode(Vector3(15, 1, -30));
    NavigationNode* n5 = new NavigationNode(Vector3(30, 1, -30));

    
    NavigationNode* n16 = new NavigationNode(Vector3(-30, 1, -15));
    NavigationNode* n17 = new NavigationNode(Vector3(-15, 1, -15));
    NavigationNode* n18 = new NavigationNode(Vector3(0, 1, -15));
    NavigationNode* n19 = new NavigationNode(Vector3(15, 1, -15));
    NavigationNode* n20 = new NavigationNode(Vector3(30, 1, -15));

    
    NavigationNode* n6 = new NavigationNode(Vector3(-30, 1, 0));
    NavigationNode* n7 = new NavigationNode(Vector3(-15, 1, 0));
    NavigationNode* n8 = new NavigationNode(Vector3(0, 1, 0));
    NavigationNode* n9 = new NavigationNode(Vector3(15, 1, 0));
    NavigationNode* n10 = new NavigationNode(Vector3(30, 1, 0));

    
    NavigationNode* n21 = new NavigationNode(Vector3(-30, 1, 15));
    NavigationNode* n22 = new NavigationNode(Vector3(-15, 1, 15));
    NavigationNode* n23 = new NavigationNode(Vector3(0, 1, 15));
    NavigationNode* n24 = new NavigationNode(Vector3(15, 1, 15));
    NavigationNode* n25 = new NavigationNode(Vector3(30, 1, 15));

   
    NavigationNode* n11 = new NavigationNode(Vector3(-30, 1, 30));
    NavigationNode* n12 = new NavigationNode(Vector3(-15, 1, 30));
    NavigationNode* n13 = new NavigationNode(Vector3(0, 1, 30));
    NavigationNode* n14 = new NavigationNode(Vector3(15, 1, 30));
    NavigationNode* n15 = new NavigationNode(Vector3(30, 1, 30));

    navigationNodes = {
        n1,n2,n3,n4,n5,
        n16,n17,n18,n19,n20,
        n6,n7,n8,n9,n10,
        n21,n22,n23,n24,n25,
        n11,n12,n13,n14,n15
    };

    auto Link = [](NavigationNode* a, NavigationNode* b) {
        a->neighbours.push_back(b);
        b->neighbours.push_back(a);
        };


    Link(n1, n2);
    Link(n2, n3);
    Link(n3, n4);
    Link(n4, n5);

   
    Link(n16, n17);
    Link(n17, n18);

    Link(n18, n19);
    Link(n19, n20);

    
    Link(n6, n7);
    Link(n7, n8);
    Link(n8, n9);
    Link(n9, n10);

    
    Link(n21, n22);
    Link(n22, n23);
    Link(n23, n24);
    Link(n24, n25);

    
    Link(n11, n12);
    Link(n12, n13);
    Link(n13, n14);
    Link(n14, n15);

   

   
    Link(n1, n16);
    Link(n16, n6);
    Link(n6, n21);
    Link(n21, n11);

    
    Link(n2, n17);
    Link(n22, n12);

   
    Link(n3, n18);
    Link(n8, n23);
    Link(n23, n13);

    
    Link(n4, n19);
    Link(n24, n14);

    
    Link(n5, n20);
    Link(n20, n10);
    Link(n10, n25);
    Link(n25, n15);
}


void TutorialGame::DrawNavigationGraph() {

    if (navigationNodes.empty())
    return;

    for (auto node : navigationNodes) {

        for (auto neighbour : node->neighbours) {

            Debug::DrawLine(
                node->position,
                neighbour->position,
                Debug::GREEN
            );
        }
    }

    for (auto& enemy : enemies) {

        for (size_t i = 0; i + 1 < enemy.path.size(); ++i) {

            Debug::DrawLine(
                enemy.path[i]->position,
                enemy.path[i + 1]->position,
                Debug::RED
            );
        }
    }
}

    NavigationNode* TutorialGame::GetClosestNode(const Vector3 & position) {

        NavigationNode* closest = nullptr;

        float closestDist = FLT_MAX;

        for (auto node : navigationNodes) {

            float dist =
                Vector::Length(node->position - position);

            if (dist < closestDist) {

                closestDist = dist;
                closest = node;
            }
        }

        return closest;
    }

    bool TutorialGame::CanEnemySeePlayer(EnemyData& enemy) {

        if (!playerObject || !enemy.enemyObject)
            return false;

        Vector3 enemyPos =
            enemy.enemyObject->GetTransform().GetPosition();

        Vector3 playerPos =
            playerObject->GetTransform().GetPosition();

        Vector3 toPlayer =
            playerPos - enemyPos;

        float dist =
            Vector::Length(toPlayer);

        

        if (dist > 100.0f)
            return false;

        toPlayer = Vector::Normalise(toPlayer);

        

        float dot =
            Vector::Dot(enemy.forward, toPlayer);

        dot = std::max(-1.0f, std::min(dot, 1.0f));

        float angle =
            acos(dot) * 57.2958f;

        
        if (angle > 100.0f)
            return false;

       

        Ray ray(enemyPos + Vector3(0, 2, 0), toPlayer);

        RayCollision rc;

        if (world.Raycast(ray, rc, true)) {

            if (rc.node == playerObject)
                return true;
        }

        return false;
    }