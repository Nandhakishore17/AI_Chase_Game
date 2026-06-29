#pragma once

#include "../NCLCoreClasses/Vector.h"
#include <vector>

namespace NCL {
    namespace CSC8503 {

        using namespace Maths;

        class NavigationNode {
        public:

            NavigationNode(const Vector3& pos);

            Vector3 position;

            std::vector<NavigationNode*> neighbours;

            float gCost;
            float hCost;
            float fCost;

            NavigationNode* parent;
        };
    }
}