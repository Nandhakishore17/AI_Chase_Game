#include "SteeringRules.h"

namespace NCL {
    namespace CSC8503 {

        Maths::Vector3 SteeringRules::RemoveOpposingAvoidance(
            const Maths::Vector3& avoidance,
            const Maths::Vector3& navigationDirection
        ) {
            Maths::Vector3 correctedAvoidance = avoidance;

            float avoidanceAlongPath =
                Maths::Vector::Dot(
                    correctedAvoidance,
                    navigationDirection
                );

            if (avoidanceAlongPath < 0.0f) {
                correctedAvoidance -=
                    navigationDirection * avoidanceAlongPath;
            }

            return correctedAvoidance;
        }

    }
}