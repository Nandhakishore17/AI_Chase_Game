#pragma once
#include "RenderObject.h"
#include "CollisionDetection.h"  
#include "NavigationNode.h"
#include "StuckAgentDetector.h"
#include "ValidationResultCollector.h"

namespace NCL {
	class Controller;

	namespace Rendering {
		class Mesh;
		class Texture;
		class Shader;
	}

	namespace CSC8503 {
		class GameTechRendererInterface;
		class PhysicsSystem;
		class GameWorld;
		class GameObject;

		class TutorialGame {
		public:
			TutorialGame(GameWorld& gameWorld, GameTechRendererInterface& renderer, PhysicsSystem& physics);
			~TutorialGame();

			virtual void UpdateGame(float dt);

			GameObject* AddDeliveryMarker(const Vector3& pos);


		protected:
			void InitCamera();
			void InitWorld();

			void InitGameExamples();
			void CreateSphereGrid(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void CreatedMixedGrid(int numRows, int numCols, float rowSpacing, float colSpacing);
			void CreateAABBGrid(int numRows, int numCols, float rowSpacing, float colSpacing, const NCL::Maths::Vector3& cubeDims);

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();
			void InitCourierLevel();

			const float arenaHalfSize = 40.0f;
			const float wallThickness = 4.0f;
			const float actorRadius = 2.0f;

			const float playLimit =
				arenaHalfSize - wallThickness - actorRadius; 

			GameObject* AddOBBToWorld(const NCL::Maths::Vector3& pos,
				const NCL::Maths::Vector3& halfDims,
				float inverseMass,
				float pitch = 0.0f,
				float yaw = 0.0f,
				float roll = 0.0f);

			void PlayerMovement(float dt);

			GameObject* playerObject = nullptr;

			
			void InitEnemyAI();
			void UpdateEnemyAI(float dt);


			void InitPuzzleObjects();
			void UpdatePuzzleLogic(float dt);

			
			void UpdateGrapple(float dt);

			
			bool inMenu = true;

			int selectedEnemyCount = 0;

			
			void StartGame(int enemyCount);
                        void WriteValidationResults();
			void DrawMenu();

			std::vector<Vector3> enemyPatrolPoints;

			float enemyDetectRange = 20.0f;
			float enemySpeed = 10.0f;
			float playerSpeedMultiplier = 1.0f;
			float enemyChaseSpeed = 20.0f;

			float bonusTimer = 0.0f;
			bool speedBoostActive = false;
			bool freezeEnemyActive = false;
			bool heavyPlayerActive = false;

			enum class EnemyState {
				Patrol,
				Chase,
				Search,
				Return
			};

			struct EnemyData {

				GameObject* enemyObject = nullptr;

				EnemyState state = EnemyState::Patrol;

				std::vector<NavigationNode*> path;

				int currentPathIndex = 0;

				Vector3 lastKnownPlayerPos;

				int patrolIndex = 0;

				float patrolWaitTimer = 0.0f;
				bool waitingAtNode = false;
				float searchTimer = 0.0f;

                                StuckAgentDetector stuckDetector;
                                bool stuckDetected = false;
                                bool movementExpected = false;

				Vector3 forward = Vector3(0, 0, -1);

				float visionAngle = 45.0f;
				float visionRange = 25.0f;

			};

			enum class BonusType {

				SpeedBoost,
				FreezeEnemy,
				HeavyPlayer
			};

			struct BonusItem {

				GameObject* object = nullptr;

				BonusType type;

				bool active = true;
			};

			bool CanEnemySeePlayer(EnemyData& enemy);

			std::vector<EnemyData> enemies;

			

			std::vector<BonusItem> bonuses;

			
			std::vector<NavigationNode*> navigationNodes;
			

			

			float pathUpdateTimer = 0.0f;
			float pathUpdateInterval = 0.25f;

			float losePlayerDistance = 60.0f;
			float attackDistance = 3.0f;

			

			void BuildNavigationGraph();
			void DrawNavigationGraph();

			NavigationNode* GetClosestNode(const Vector3& position);

			
			bool grappling = false;
			Vector3 grapplePoint;

			
			GameObject* pressureButton = nullptr;
			GameObject* puzzleDoor = nullptr;
			bool doorOpen = false;

			
			GameObject* fragileItem = nullptr;
			bool itemCarried = false;
			bool itemBroken = false;


			GameObject* deliveryZone = nullptr;

			int totalItems = 1;
			int deliveredItems = 0;
			int score = 0;

			bool gameWon = false;
			bool gameLost = false;
			bool gameEnded = false;

			std::string endReason = "";

			
			GameObject* AddFragileItemToWorld(const Vector3& position);
			GameObject* AddDeliveryZone(const Vector3& position, const Vector3& size);

			void UpdateFragileItem(float dt);
			void CheckItemBreak(GameObject* item, const CollisionDetection::CollisionInfo& info); 
			void UpdateDelivery(float dt);

			void DrawHUD();

			GameObject* AddFloorToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddSphereToWorld(const NCL::Maths::Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const NCL::Maths::Vector3& position, NCL::Maths::Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddEnemyToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddBonusToWorld(const NCL::Maths::Vector3& position);

			void SpawnBonuses();

			void UpdateBonuses(float dt);

			void EnemyBonusDecision(EnemyData& enemy);

			Vector3 CalculateAvoidanceForce(EnemyData& enemy);

                 ValidationResultCollector validationResultCollector;
                 float validationElapsedTime = 0.0f;

			GameWorld& world;
			GameTechRendererInterface& renderer;
			PhysicsSystem& physics;
			Controller* controller;

			bool useGravity;
			bool inSelectionMode;

			float forceMagnitude;

			GameObject* selectionObject = nullptr;

			Rendering::Mesh* capsuleMesh = nullptr;
			Rendering::Mesh* cubeMesh = nullptr;
			Rendering::Mesh* sphereMesh = nullptr;

			Rendering::Texture* defaultTex = nullptr;
			Rendering::Texture* checkerTex = nullptr;
			Rendering::Texture* glassTex = nullptr;

			Rendering::Mesh* catMesh = nullptr;
			Rendering::Mesh* kittenMesh = nullptr;
			Rendering::Mesh* enemyMesh = nullptr;
			Rendering::Mesh* bonusMesh = nullptr;

			GameTechMaterial checkerMaterial;
			GameTechMaterial glassMaterial;
			GameTechMaterial notexMaterial;

			GameObject* lockedObject = nullptr;
			NCL::Maths::Vector3 lockedOffset = NCL::Maths::Vector3(0, 14, 20);

			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			GameObject* objClosest = nullptr;
		};
	}
}
