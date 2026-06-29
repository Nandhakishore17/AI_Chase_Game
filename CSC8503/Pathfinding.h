#pragma once

#include "NavigationNode.h"
#include <vector>

namespace NCL {
    namespace CSC8503 {

        class Pathfinding {
        public:

            static std::vector<NavigationNode*> FindPath(
                NavigationNode* start,
                NavigationNode* goal
            );

        protected:

            static float Heuristic(
                NavigationNode* a,
                NavigationNode* b
            );
        };
    }
}