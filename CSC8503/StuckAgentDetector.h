#pragma once

#include "../NCLCoreClasses/Vector.h"

namespace NCL {
    namespace CSC8503 {

        class StuckAgentDetector {
        public:
            static constexpr float DefaultMovementThreshold = 0.1f;
            static constexpr float DefaultStuckTimeThreshold = 3.0f;

            StuckAgentDetector(
                float movementThreshold = DefaultMovementThreshold,
                float stuckTimeThreshold = DefaultStuckTimeThreshold
            );

            void Reset(const Maths::Vector3& position);

            bool Update(
                const Maths::Vector3& currentPosition,
                float dt,
                bool shouldBeMoving
            );

            float GetStationaryTime() const;
            float GetDistanceFromLastProgressPosition(
    const Maths::Vector3& currentPosition
) const;

        private:
            Maths::Vector3 lastPosition;
            float stationaryTime = 0.0f;
            float movementThreshold;
            float stuckTimeThreshold;
            bool initialised = false;
        };

    }
}