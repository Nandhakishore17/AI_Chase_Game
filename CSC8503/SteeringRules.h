#pragma once

#include "../NCLCoreClasses/Vector.h"

namespace NCL {
    namespace CSC8503 {

        class SteeringRules {
        public:
            static Maths::Vector3 RemoveOpposingAvoidance(
                const Maths::Vector3& avoidance,
                const Maths::Vector3& navigationDirection
            );
        };

    }
}