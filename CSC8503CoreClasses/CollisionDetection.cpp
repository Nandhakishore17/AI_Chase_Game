//#include "CollisionDetection.h"
//#include "CollisionVolume.h"
//#include "AABBVolume.h"
//#include "OBBVolume.h"
//#include "SphereVolume.h"
//#include "Window.h"
//#include "Maths.h"
//#include "Debug.h"
//
//using namespace NCL;
//
//bool CollisionDetection::RayPlaneIntersection(const Ray&r, const Plane&p, RayCollision& collisions) {
//	float ln = Vector::Dot(p.GetNormal(), r.GetDirection());
//
//	if (ln == 0.0f) {
//		return false; //direction vectors are perpendicular!
//	}
//	
//	Vector3 planePoint = p.GetPointOnPlane();
//
//	Vector3 pointDir = planePoint - r.GetPosition();
//
//	float d = Vector::Dot(pointDir, p.GetNormal()) / ln;
//
//	collisions.collidedAt = r.GetPosition() + (r.GetDirection() * d);
//
//	return true;
//}
//
//bool CollisionDetection::RayIntersection(const Ray& r,GameObject& object, RayCollision& collision) {
//	bool hasCollided = false;
//
//	const Transform& worldTransform = object.GetTransform();
//	const CollisionVolume* volume	= object.GetBoundingVolume();
//
//	if (!volume) {
//		return false;
//	}
//
//	switch (volume->type) {
//		case VolumeType::AABB:		hasCollided = RayAABBIntersection(r, worldTransform, (const AABBVolume&)*volume	, collision); break;
//		case VolumeType::OBB:		hasCollided = RayOBBIntersection(r, worldTransform, (const OBBVolume&)*volume	, collision); break;
//		case VolumeType::Sphere:	hasCollided = RaySphereIntersection(r, worldTransform, (const SphereVolume&)*volume	, collision); break;
//
//		case VolumeType::Capsule:	hasCollided = RayCapsuleIntersection(r, worldTransform, (const CapsuleVolume&)*volume, collision); break;
//	}
//
//	return hasCollided;
//}
//
//bool CollisionDetection::RayBoxIntersection(const Ray&r, const Vector3& boxPos, const Vector3& boxSize, RayCollision& collision) {
//	return false;
//}
//
//bool CollisionDetection::RayAABBIntersection(const Ray&r, const Transform& worldTransform, const AABBVolume& volume, RayCollision& collision) {
//	return false;
//}
//
//bool CollisionDetection::RayOBBIntersection(const Ray&r, const Transform& worldTransform, const OBBVolume& volume, RayCollision& collision) {
//	return false;
//}
//
//bool CollisionDetection::RaySphereIntersection(const Ray&r, const Transform& worldTransform, const SphereVolume& volume, RayCollision& collision) {
//	return false;
//}
//
//bool CollisionDetection::RayCapsuleIntersection(const Ray& r, const Transform& worldTransform, const CapsuleVolume& volume, RayCollision& collision) {
//	return false;
//}
//
//bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo) {
//	const CollisionVolume* volA = a->GetBoundingVolume();
//	const CollisionVolume* volB = b->GetBoundingVolume();
//
//	if (!volA || !volB) {
//		return false;
//	}
//
//	collisionInfo.a = a;
//	collisionInfo.b = b;
//
//	Transform& transformA = a->GetTransform();
//	Transform& transformB = b->GetTransform();
//
//	VolumeType pairType = (VolumeType)((int)volA->type | (int)volB->type);
//
//	//Two AABBs
//	if (pairType == VolumeType::AABB) {
//		return AABBIntersection((AABBVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
//	}
//	//Two Spheres
//	if (pairType == VolumeType::Sphere) {
//		return SphereIntersection((SphereVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
//	}
//	//Two OBBs
//	if (pairType == VolumeType::OBB) {
//		return OBBIntersection((OBBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
//	}
//	//Two Capsules
//
//	//AABB vs Sphere pairs
//	if (volA->type == VolumeType::AABB && volB->type == VolumeType::Sphere) {
//		return AABBSphereIntersection((AABBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
//	}
//	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::AABB) {
//		collisionInfo.a = b;
//		collisionInfo.b = a;
//		return AABBSphereIntersection((AABBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
//	}
//
//	//OBB vs sphere pairs
//	if (volA->type == VolumeType::OBB && volB->type == VolumeType::Sphere) {
//		return OBBSphereIntersection((OBBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
//	}
//	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::OBB) {
//		collisionInfo.a = b;
//		collisionInfo.b = a;
//		return OBBSphereIntersection((OBBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
//	}
//
//	//Capsule vs other interactions
//	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::Sphere) {
//		return SphereCapsuleIntersection((CapsuleVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
//	}
//	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::Capsule) {
//		collisionInfo.a = b;
//		collisionInfo.b = a;
//		return SphereCapsuleIntersection((CapsuleVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
//	}
//
//	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::AABB) {
//		return AABBCapsuleIntersection((CapsuleVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
//	}
//	if (volB->type == VolumeType::Capsule && volA->type == VolumeType::AABB) {
//		collisionInfo.a = b;
//		collisionInfo.b = a;
//		return AABBCapsuleIntersection((CapsuleVolume&)*volB, transformB, (AABBVolume&)*volA, transformA, collisionInfo);
//	}
//
//	return false;
//}
//
//bool CollisionDetection::AABBTest(const Vector3& posA, const Vector3& posB, const Vector3& halfSizeA, const Vector3& halfSizeB) {
//	Vector3 delta = posB - posA;
//	Vector3 totalSize = halfSizeA + halfSizeB;
//
//	if (abs(delta.x) < totalSize.x &&
//		abs(delta.y) < totalSize.y &&
//		abs(delta.z) < totalSize.z) {
//		return true;
//	}
//	return false;
//}
//
////AABB/AABB Collisions
//bool CollisionDetection::AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
//	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
////Sphere / Sphere Collision
//bool CollisionDetection::SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
//	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
////AABB - Sphere Collision
//bool CollisionDetection::AABBSphereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
//	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
//bool  CollisionDetection::OBBSphereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
//	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
//bool CollisionDetection::AABBCapsuleIntersection(
//	const CapsuleVolume& volumeA, const Transform& worldTransformA,
//	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
//bool CollisionDetection::SphereCapsuleIntersection(
//	const CapsuleVolume& volumeA, const Transform& worldTransformA,
//	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
//bool CollisionDetection::OBBIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
//	const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
//	return false;
//}
//
//Matrix4 GenerateInverseView(const Camera &c) {
//	float pitch = c.GetPitch();
//	float yaw	= c.GetYaw();
//	Vector3 position = c.GetPosition();
//
//	Matrix4 iview =
//		Matrix::Translation(position) *
//		Matrix::Rotation(-yaw, Vector3(0, -1, 0)) *
//		Matrix::Rotation(-pitch, Vector3(-1, 0, 0));
//
//	return iview;
//}
//
//Matrix4 GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
//	float negDepth = nearPlane - farPlane;
//
//	float invNegDepth = negDepth / (2 * (farPlane * nearPlane));
//
//	Matrix4 m;
//
//	float h = 1.0f / tan(fov*PI_OVER_360);
//
//	m.array[0][0] = aspect / h;
//	m.array[1][1] = tan(fov * PI_OVER_360);
//	m.array[2][2] = 0.0f;
//
//	m.array[2][3] = invNegDepth;//// +PI_OVER_360;
//	m.array[3][2] = -1.0f;
//	m.array[3][3] = (0.5f / nearPlane) + (0.5f / farPlane);
//
//	return m;
//}
//
//Vector3 CollisionDetection::Unproject(const Vector3& screenPos, const PerspectiveCamera& cam) {
//	Vector2i screenSize = Window::GetWindow()->GetScreenSize();
//
//	float aspect = Window::GetWindow()->GetScreenAspect();
//	float fov		= cam.GetFieldOfVision();
//	float nearPlane = cam.GetNearPlane();
//	float farPlane  = cam.GetFarPlane();
//
//	//Create our inverted matrix! Note how that to get a correct inverse matrix,
//	//the order of matrices used to form it are inverted, too.
//	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);
//
//	Matrix4 proj  = cam.BuildProjectionMatrix(aspect);
//
//	//Our mouse position x and y values are in 0 to screen dimensions range,
//	//so we need to turn them into the -1 to 1 axis range of clip space.
//	//We can do that by dividing the mouse values by the width and height of the
//	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
//	//and then subtracting 1 (-1.0 to 1.0).
//	Vector4 clipSpace = Vector4(
//		(screenPos.x / (float)screenSize.x) * 2.0f - 1.0f,
//		(screenPos.y / (float)screenSize.y) * 2.0f - 1.0f,
//		(screenPos.z),
//		1.0f
//	);
//
//	//Then, we multiply our clipspace coordinate by our inverted matrix
//	Vector4 transformed = invVP * clipSpace;
//
//	//our transformed w coordinate is now the 'inverse' perspective divide, so
//	//we can reconstruct the final world space by dividing x,y,and z by w.
//	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
//}
//
//Ray CollisionDetection::BuildRayFromMouse(const PerspectiveCamera& cam) {
//	Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
//	Vector2i screenSize	= Window::GetWindow()->GetScreenSize();
//
//	//We remove the y axis mouse position from height as OpenGL is 'upside down',
//	//and thinks the bottom left is the origin, instead of the top left!
//	Vector3 nearPos = Vector3(screenMouse.x,
//		screenSize.y - screenMouse.y,
//		-0.99999f
//	);
//
//	//We also don't use exactly 1.0 (the normalised 'end' of the far plane) as this
//	//causes the unproject function to go a bit weird. 
//	Vector3 farPos = Vector3(screenMouse.x,
//		screenSize.y - screenMouse.y,
//		0.99999f
//	);
//
//	Vector3 a = Unproject(nearPos, cam);
//	Vector3 b = Unproject(farPos, cam);
//	Vector3 c = b - a;
//
//	c = Vector::Normalise(c);
//
//	return Ray(cam.GetPosition(), c);
//}
//
////http://bookofhook.com/mousepick.pdf
//Matrix4 CollisionDetection::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
//	Matrix4 m;
//
//	float t = tan(fov*PI_OVER_360);
//
//	float neg_depth = nearPlane - farPlane;
//
//	const float h = 1.0f / t;
//
//	float c = (farPlane + nearPlane) / neg_depth;
//	float e = -1.0f;
//	float d = 2.0f*(nearPlane*farPlane) / neg_depth;
//
//	m.array[0][0] = aspect / h;
//	m.array[1][1] = tan(fov * PI_OVER_360);
//	m.array[2][2] = 0.0f;
//
//	m.array[2][3] = 1.0f / d;
//
//	m.array[3][2] = 1.0f / e;
//	m.array[3][3] = -c / (d * e);
//
//	return m;
//}
//
///*
//And here's how we generate an inverse view matrix. It's pretty much
//an exact inversion of the BuildViewMatrix function of the Camera class!
//*/
//Matrix4 CollisionDetection::GenerateInverseView(const Camera &c) {
//	float pitch = c.GetPitch();
//	float yaw	= c.GetYaw();
//	Vector3 position = c.GetPosition();
//
//	Matrix4 iview =
//		Matrix::Translation(position) *
//		Matrix::Rotation(yaw, Vector3(0, 1, 0)) *
//		Matrix::Rotation(pitch, Vector3(1, 0, 0));
//
//	return iview;
//}
//
//
///*
//If you've read through the Deferred Rendering tutorial you should have a pretty
//good idea what this function does. It takes a 2D position, such as the mouse
//position, and 'unprojects' it, to generate a 3D world space position for it.
//
//Just as we turn a world space position into a clip space position by multiplying
//it by the model, view, and projection matrices, we can turn a clip space
//position back to a 3D position by multiply it by the INVERSE of the
//view projection matrix (the model matrix has already been assumed to have
//'transformed' the 2D point). As has been mentioned a few times, inverting a
//matrix is not a nice operation, either to understand or code. But! We can cheat
//the inversion process again, just like we do when we create a view matrix using
//the camera.
//
//So, to form the inverted matrix, we need the aspect and fov used to create the
//projection matrix of our scene, and the camera used to form the view matrix.
//
//*/
//Vector3	CollisionDetection::UnprojectScreenPosition(Vector3 position, float aspect, float fov, const PerspectiveCamera& c) {
//	//Create our inverted matrix! Note how that to get a correct inverse matrix,
//	//the order of matrices used to form it are inverted, too.
//	Matrix4 invVP = GenerateInverseView(c) * GenerateInverseProjection(aspect, fov, c.GetNearPlane(), c.GetFarPlane());
//
//
//	Vector2i screenSize = Window::GetWindow()->GetScreenSize();
//
//	//Our mouse position x and y values are in 0 to screen dimensions range,
//	//so we need to turn them into the -1 to 1 axis range of clip space.
//	//We can do that by dividing the mouse values by the width and height of the
//	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
//	//and then subtracting 1 (-1.0 to 1.0).
//	Vector4 clipSpace = Vector4(
//		(position.x / (float)screenSize.x) * 2.0f - 1.0f,
//		(position.y / (float)screenSize.y) * 2.0f - 1.0f,
//		(position.z) - 1.0f,
//		1.0f
//	);
//
//	//Then, we multiply our clipspace coordinate by our inverted matrix
//	Vector4 transformed = invVP * clipSpace;
//
//	//our transformed w coordinate is now the 'inverse' perspective divide, so
//	//we can reconstruct the final world space by dividing x,y,and z by w.
//	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
//}
//





#include "CollisionDetection.h"
#include "CollisionVolume.h"
#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "CapsuleVolume.h"
#include "Window.h"
#include "Maths.h"
#include "Debug.h"

using namespace NCL;
using namespace NCL::Maths;

// ============ RAY / PLANE (unchanged) ============

bool CollisionDetection::RayPlaneIntersection(const Ray& r, const Plane& p, RayCollision& collisions) {
    float ln = Vector::Dot(p.GetNormal(), r.GetDirection());

    if (ln == 0.0f) {
        return false; //direction vectors are perpendicular!
    }

    Vector3 planePoint = p.GetPointOnPlane();
    Vector3 pointDir = planePoint - r.GetPosition();
    float   d = Vector::Dot(pointDir, p.GetNormal()) / ln;

    if (d < 0.0f) {
        return false; // plane is behind ray origin
    }

    collisions.rayDistance = d;
    collisions.collidedAt = r.GetPosition() + (r.GetDirection() * d);
    return true;
}

// ============ GENERIC RAY / VOLUME DISPATCH ============

bool CollisionDetection::RayIntersection(const Ray& r, GameObject& object, RayCollision& collision) {
    bool hasCollided = false;

    const Transform& worldTransform = object.GetTransform();
    const CollisionVolume* volume = object.GetBoundingVolume();

    if (!volume) {
        return false;
    }

    switch (volume->type) {
    case VolumeType::AABB:
        hasCollided = RayAABBIntersection(r, worldTransform, (const AABBVolume&)*volume, collision);
        break;
    case VolumeType::OBB:
        hasCollided = RayOBBIntersection(r, worldTransform, (const OBBVolume&)*volume, collision);
        break;
    case VolumeType::Sphere:
        hasCollided = RaySphereIntersection(r, worldTransform, (const SphereVolume&)*volume, collision);
        break;
    case VolumeType::Capsule:
        hasCollided = RayCapsuleIntersection(r, worldTransform, (const CapsuleVolume&)*volume, collision);
        break;
    }

    return hasCollided;
}

// ============ RAY HELPERS ============

bool CollisionDetection::RayBoxIntersection(const Ray& r, const Vector3& boxPos, const Vector3& boxHalfSize, RayCollision& collision) {
    // Axis-aligned slab method
    const Vector3& rayPos = r.GetPosition();
    const Vector3& rayDir = r.GetDirection();

    Vector3 boxMin = boxPos - boxHalfSize;
    Vector3 boxMax = boxPos + boxHalfSize;

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    // For each axis
    for (int axis = 0; axis < 3; ++axis) {
        float origin = (&rayPos.x)[axis];
        float dir = (&rayDir.x)[axis];
        float minB = (&boxMin.x)[axis];
        float maxB = (&boxMax.x)[axis];

        if (fabs(dir) < 1e-6f) {
            // Ray is parallel to this axis - must be inside slab
            if (origin < minB || origin > maxB) {
                return false;
            }
        }
        else {
            float t1 = (minB - origin) / dir;
            float t2 = (maxB - origin) / dir;
            if (t1 > t2) std::swap(t1, t2);

            if (t1 > tMin) tMin = t1;
            if (t2 < tMax) tMax = t2;

            if (tMin > tMax) {
                return false;
            }
        }
    }

    collision.rayDistance = tMin;
    collision.collidedAt = rayPos + rayDir * tMin;
    return true;
}

bool CollisionDetection::RayAABBIntersection(const Ray& r, const Transform& worldTransform, const AABBVolume& volume, RayCollision& collision) {
    Vector3 boxPos = worldTransform.GetPosition();
    Vector3 boxHalfSize = volume.GetHalfDimensions();
    return RayBoxIntersection(r, boxPos, boxHalfSize, collision);
}

bool CollisionDetection::RayOBBIntersection(const Ray& r, const Transform& worldTransform, const OBBVolume& volume, RayCollision& collision) {
    // Transform ray into OBB local space
    Vector3 boxPos = worldTransform.GetPosition();
    Vector3 halfSizes = volume.GetHalfDimensions();

    Quaternion q = worldTransform.GetOrientation();
    Matrix3    orient = Quaternion::RotationMatrix<Matrix3>(q);
    Matrix3    invOrient = Matrix::Transpose(orient); // inverse for pure rotation

    Vector3 localRayPos = invOrient * (r.GetPosition() - boxPos);
    Vector3 localRayDir = invOrient * r.GetDirection();

    Ray localRay(localRayPos, localRayDir);
    RayCollision localCollision;

    if (!RayBoxIntersection(localRay, Vector3(0, 0, 0), halfSizes, localCollision)) {
        return false;
    }

    // Transform hit point back to world space
    collision.rayDistance = localCollision.rayDistance;
    collision.collidedAt = r.GetPosition() + r.GetDirection() * localCollision.rayDistance;
    return true;
}

bool CollisionDetection::RaySphereIntersection(const Ray& r, const Transform& worldTransform, const SphereVolume& volume, RayCollision& collision) {
    Vector3 spherePos = worldTransform.GetPosition();
    float   radius = volume.GetRadius();

    const Vector3& rayPos = r.GetPosition();
    const Vector3& rayDir = r.GetDirection();

    Vector3 m = rayPos - spherePos;

    float b = Vector::Dot(m, rayDir);
    float c = Vector::Dot(m, m) - radius * radius;

    // If ray origin outside sphere (c > 0) and pointing away (b > 0) -> no hit
    if (c > 0.0f && b > 0.0f) {
        return false;
    }

    float discriminant = b * b - c;
    if (discriminant < 0.0f) {
        return false;
    }

    float t = -b - sqrt(discriminant);
    if (t < 0.0f) {
        t = 0.0f; // origin inside sphere
    }

    collision.rayDistance = t;
    collision.collidedAt = rayPos + rayDir * t;
    return true;
}

bool CollisionDetection::RayCapsuleIntersection(const Ray& r, const Transform& worldTransform, const CapsuleVolume& volume, RayCollision& collision) {
    // Not needed for your current gameplay – return false for now
    return false;
}

// ============ OBJECT / OBJECT BROAD DISPATCH (unchanged) ============

bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo) {
    const CollisionVolume* volA = a->GetBoundingVolume();
    const CollisionVolume* volB = b->GetBoundingVolume();

    if (!volA || !volB) {
        return false;
    }

    collisionInfo.a = a;
    collisionInfo.b = b;

    Transform& transformA = a->GetTransform();
    Transform& transformB = b->GetTransform();

    VolumeType pairType = (VolumeType)((int)volA->type | (int)volB->type);

    //Two AABBs
    if (pairType == VolumeType::AABB) {
        return AABBIntersection((AABBVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
    }
    //Two Spheres
    if (pairType == VolumeType::Sphere) {
        return SphereIntersection((SphereVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
    }
    //Two OBBs
    if (pairType == VolumeType::OBB) {
        return OBBIntersection((OBBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
    }
    //Two Capsules (not implemented)

    //AABB vs Sphere pairs
    if (volA->type == VolumeType::AABB && volB->type == VolumeType::Sphere) {
        return AABBSphereIntersection((AABBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
    }
    if (volA->type == VolumeType::Sphere && volB->type == VolumeType::AABB) {
        collisionInfo.a = b;
        collisionInfo.b = a;
        return AABBSphereIntersection((AABBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
    }

    //OBB vs sphere pairs
    if (volA->type == VolumeType::OBB && volB->type == VolumeType::Sphere) {
        return OBBSphereIntersection((OBBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
    }
    if (volA->type == VolumeType::Sphere && volB->type == VolumeType::OBB) {
        collisionInfo.a = b;
        collisionInfo.b = a;
        return OBBSphereIntersection((OBBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
    }

    //Capsule vs other interactions – not used in your current game
    if (volA->type == VolumeType::Capsule && volB->type == VolumeType::Sphere) {
        return SphereCapsuleIntersection((CapsuleVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
    }
    if (volA->type == VolumeType::Sphere && volB->type == VolumeType::Capsule) {
        collisionInfo.a = b;
        collisionInfo.b = a;
        return SphereCapsuleIntersection((CapsuleVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
    }

    if (volA->type == VolumeType::Capsule && volB->type == VolumeType::AABB) {
        return AABBCapsuleIntersection((CapsuleVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
    }
    if (volB->type == VolumeType::Capsule && volA->type == VolumeType::AABB) {
        collisionInfo.a = b;
        collisionInfo.b = a;
        return AABBCapsuleIntersection((CapsuleVolume&)*volB, transformB, (AABBVolume&)*volA, transformA, collisionInfo);
    }

    return false;
}

// ============ SIMPLE AABB TEST (unchanged) ============

bool CollisionDetection::AABBTest(const Vector3& posA, const Vector3& posB, const Vector3& halfSizeA, const Vector3& halfSizeB) {
    Vector3 delta = posB - posA;
    Vector3 totalSize = halfSizeA + halfSizeB;

    if (fabs(delta.x) < totalSize.x &&
        fabs(delta.y) < totalSize.y &&
        fabs(delta.z) < totalSize.z) {
        return true;
    }
    return false;
}

// ============ NARROW-PHASE COLLISIONS ============

// AABB vs AABB
bool CollisionDetection::AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
    const AABBVolume& volumeB, const Transform& worldTransformB,
    CollisionInfo& collisionInfo) {

    Vector3 posA = worldTransformA.GetPosition();
    Vector3 posB = worldTransformB.GetPosition();
    Vector3 halfSizeA = volumeA.GetHalfDimensions();
    Vector3 halfSizeB = volumeB.GetHalfDimensions();

    if (!AABBTest(posA, posB, halfSizeA, halfSizeB)) {
        return false;
    }

    // Compute penetration and normal
    Vector3 delta = posB - posA;
    Vector3 total = halfSizeA + halfSizeB;

    float px = total.x - fabs(delta.x);
    float py = total.y - fabs(delta.y);
    float pz = total.z - fabs(delta.z);

    Vector3 normal;
    float penetration = px;

    normal = Vector3((delta.x < 0) ? -1.0f : 1.0f, 0, 0);

    if (py < penetration) {
        penetration = py;
        normal = Vector3(0, (delta.y < 0) ? -1.0f : 1.0f, 0);
    }
    if (pz < penetration) {
        penetration = pz;
        normal = Vector3(0, 0, (delta.z < 0) ? -1.0f : 1.0f);
    }

    Vector3 localA = posA + normal * halfSizeA;
    Vector3 localB = posB - normal * halfSizeB;

    collisionInfo.AddContactPoint(localA, localB, normal, penetration);
    return true;
}

// Sphere vs Sphere
bool CollisionDetection::SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
    const SphereVolume& volumeB, const Transform& worldTransformB,
    CollisionInfo& collisionInfo) {

    Vector3 posA = worldTransformA.GetPosition();
    Vector3 posB = worldTransformB.GetPosition();

    float radiusA = volumeA.GetRadius();
    float radiusB = volumeB.GetRadius();

    Vector3 delta = posB - posA;
    float  distSq = Vector::LengthSquared(delta);
    float  totalR = radiusA + radiusB;

    if (distSq > totalR * totalR) {
        return false;
    }

    float dist = sqrt(distSq);
    Vector3 normal;

    if (dist > 0.0f) {
        normal = delta * (1.0f / dist);
    }
    else {
        // centres at same point – pick arbitrary normal
        normal = Vector3(0, 1, 0);
        dist = totalR;
    }

    float penetration = totalR - dist;

    Vector3 localA = posA + normal * radiusA;
    Vector3 localB = posB - normal * radiusB;

    collisionInfo.AddContactPoint(localA, localB, normal, penetration);
    return true;
}

// AABB vs Sphere
bool CollisionDetection::AABBSphereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
    const SphereVolume& volumeB, const Transform& worldTransformB,
    CollisionInfo& collisionInfo) {

    Vector3 boxPos = worldTransformA.GetPosition();
    Vector3 halfSizes = volumeA.GetHalfDimensions();
    Vector3 spherePos = worldTransformB.GetPosition();
    float   radius = volumeB.GetRadius();

    // Closest point on the AABB to the sphere centre
    Vector3 localPoint = spherePos - boxPos;

    Vector3 clamped;
    clamped.x = std::clamp(localPoint.x, -halfSizes.x, halfSizes.x);
    clamped.y = std::clamp(localPoint.y, -halfSizes.y, halfSizes.y);
    clamped.z = std::clamp(localPoint.z, -halfSizes.z, halfSizes.z);

    Vector3 closest = boxPos + clamped;
    Vector3 delta = spherePos - closest;

    float distSq = Vector::LengthSquared(delta);
    if (distSq > radius * radius) {
        return false;
    }

    float dist = sqrt(distSq);
    Vector3 normal;

    if (dist > 0.0f) {
        normal = delta * (1.0f / dist);
    }
    else {
        // Sphere centre inside box; pick axis of maximum penetration
        Vector3 absLocal(fabs(localPoint.x), fabs(localPoint.y), fabs(localPoint.z));
        if (absLocal.x > absLocal.y && absLocal.x > absLocal.z)
            normal = Vector3((localPoint.x > 0) ? 1.0f : -1.0f, 0, 0);
        else if (absLocal.y > absLocal.z)
            normal = Vector3(0, (localPoint.y > 0) ? 1.0f : -1.0f, 0);
        else
            normal = Vector3(0, 0, (localPoint.z > 0) ? 1.0f : -1.0f);
        dist = 0.0f;
    }

    float penetration = radius - dist;

    Vector3 localA = closest;
    Vector3 localB = spherePos - normal * radius;

    collisionInfo.AddContactPoint(localA, localB, normal, penetration);
    return true;
}

// OBB vs Sphere
bool CollisionDetection::OBBSphereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
    const SphereVolume& volumeB, const Transform& worldTransformB,
    CollisionInfo& collisionInfo) {

    Vector3 boxPos = worldTransformA.GetPosition();
    Vector3 halfSizes = volumeA.GetHalfDimensions();
    Vector3 spherePos = worldTransformB.GetPosition();
    float   radius = volumeB.GetRadius();

    Quaternion q = worldTransformA.GetOrientation();
    Matrix3    orient = Quaternion::RotationMatrix<Matrix3>(q);
    Matrix3    invOrient = Matrix::Transpose(orient);

    // Transform sphere centre to box local space
    Vector3 localCenter = invOrient * (spherePos - boxPos);

    // Closest point in OBB local space
    Vector3 clamped;
    clamped.x = std::clamp(localCenter.x, -halfSizes.x, halfSizes.x);
    clamped.y = std::clamp(localCenter.y, -halfSizes.y, halfSizes.y);
    clamped.z = std::clamp(localCenter.z, -halfSizes.z, halfSizes.z);

    Vector3 closestLocal = clamped;
    Vector3 deltaLocal = localCenter - closestLocal;

    float distSq = Vector::LengthSquared(deltaLocal);
    if (distSq > radius * radius) {
        return false;
    }

    float dist = sqrt(distSq);

    Vector3 normalWorld;

    if (dist > 0.0f) {
        Vector3 normalLocal = deltaLocal * (1.0f / dist);
        normalWorld = orient * normalLocal;
    }
    else {
        // Centre inside box – take normal from largest axis
        Vector3 absLocal(fabs(localCenter.x), fabs(localCenter.y), fabs(localCenter.z));
        Vector3 normalLocal;

        if (absLocal.x > absLocal.y && absLocal.x > absLocal.z)
            normalLocal = Vector3((localCenter.x > 0) ? 1.0f : -1.0f, 0, 0);
        else if (absLocal.y > absLocal.z)
            normalLocal = Vector3(0, (localCenter.y > 0) ? 1.0f : -1.0f, 0);
        else
            normalLocal = Vector3(0, 0, (localCenter.z > 0) ? 1.0f : -1.0f);

        normalWorld = orient * normalLocal;
        dist = 0.0f;
    }

    float penetration = radius - dist;

    Vector3 closestWorld = boxPos + orient * closestLocal;
    Vector3 localA = closestWorld;
    Vector3 localB = spherePos - normalWorld * radius;

    collisionInfo.AddContactPoint(localA, localB, normalWorld, penetration);
    return true;
}

// Capsule helpers – not needed for current gameplay
bool CollisionDetection::AABBCapsuleIntersection(
    const CapsuleVolume& volumeA, const Transform& worldTransformA,
    const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
    return false;
}

bool CollisionDetection::SphereCapsuleIntersection(
    const CapsuleVolume& volumeA, const Transform& worldTransformA,
    const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
    return false;
}

// Very simple OBB vs OBB (approximate – treat as AABBs in world space)
bool CollisionDetection::OBBIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
    const OBBVolume& volumeB, const Transform& worldTransformB,
    CollisionInfo& collisionInfo) {

    // Approximation: use centres and half-sizes ignoring rotation in narrow phase.
    // This is enough for your static doors / walls.
    AABBVolume aProxy(volumeA.GetHalfDimensions());
    AABBVolume bProxy(volumeB.GetHalfDimensions());
    return AABBIntersection(aProxy, worldTransformA, bProxy, worldTransformB, collisionInfo);
}

// ============ existing Unproject / BuildRay / matrices stay UNCHANGED ============
// (leave all the GenerateInverseView / GenerateInverseProjection / Unproject /
//  BuildRayFromMouse / UnprojectScreenPosition functions exactly as you had them)



Ray CollisionDetection::BuildRayFromMouse(const PerspectiveCamera& cam) {
    Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
    Vector2i screenSize = Window::GetWindow()->GetScreenSize();

    // invert Y because OpenGL screens are upside down
    Vector3 nearPos = Vector3(
        screenMouse.x,
        screenSize.y - screenMouse.y,
        -0.99999f
    );

    Vector3 farPos = Vector3(
        screenMouse.x,
        screenSize.y - screenMouse.y,
        0.99999f
    );

    Vector3 a = Unproject(nearPos, cam);
    Vector3 b = Unproject(farPos, cam);
    Vector3 dir = b - a;

    dir = Vector::Normalise(dir);
    return Ray(cam.GetPosition(), dir);
}

Vector3 CollisionDetection::Unproject(const Vector3& screenPos, const PerspectiveCamera& cam) {
    Vector2i screenSize = Window::GetWindow()->GetScreenSize();

    float aspect = Window::GetWindow()->GetScreenAspect();
    float fov = cam.GetFieldOfVision();
    float nearPlane = cam.GetNearPlane();
    float farPlane = cam.GetFarPlane();

    // Build inverse matrices
    Matrix4 invProj = GenerateInverseProjection(aspect, fov, nearPlane, farPlane);
    Matrix4 invView = GenerateInverseView(cam);

    Matrix4 invVP = invView * invProj;

    // Convert mouse/screen position to clip space
    Vector4 clipSpace;

    clipSpace.x = (screenPos.x / (float)screenSize.x) * 2.0f - 1.0f;
    clipSpace.y = (screenPos.y / (float)screenSize.y) * 2.0f - 1.0f;
    clipSpace.z = screenPos.z;
    clipSpace.w = 1.0f;

    // Transform to world space
    Vector4 worldPos = invVP * clipSpace;

    // Perspective divide
    return Vector3(
        worldPos.x / worldPos.w,
        worldPos.y / worldPos.w,
        worldPos.z / worldPos.w
    );
}


Matrix4 CollisionDetection::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
    Matrix4 m;

    float t = tan(fov * PI_OVER_360);
    float negDepth = nearPlane - farPlane;

    float h = 1.0f / t;

    float c = (farPlane + nearPlane) / negDepth;
    float e = -1.0f;
    float d = 2.0f * (nearPlane * farPlane) / negDepth;

    m.array[0][0] = aspect / h;
    m.array[1][1] = tan(fov * PI_OVER_360);
    m.array[2][2] = 0.0f;

    m.array[2][3] = 1.0f / d;
    m.array[3][2] = 1.0f / e;
    m.array[3][3] = -c / (d * e);

    return m;
}

Matrix4 CollisionDetection::GenerateInverseView(const Camera& c) {
    float pitch = c.GetPitch();
    float yaw = c.GetYaw();
    Vector3 pos = c.GetPosition();

    Matrix4 translation = Matrix::Translation(pos);
    Matrix4 yawRot = Matrix::Rotation(yaw, Vector3(0, 1, 0));
    Matrix4 pitchRot = Matrix::Rotation(pitch, Vector3(1, 0, 0));

    return translation * yawRot * pitchRot;
}
