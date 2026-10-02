#include "StuckAgentDetector.h"

namespace NCL {
    namespace CSC8503 {

        StuckAgentDetector::StuckAgentDetector(
            float movementThreshold,
            float stuckTimeThreshold
        )
            : movementThreshold(movementThreshold),
              stuckTimeThreshold(stuckTimeThreshold) {
        }

        void StuckAgentDetector::Reset(
            const Maths::Vector3& position
        ) {
            lastPosition = position;
            stationaryTime = 0.0f;
            initialised = true;
        }

        bool StuckAgentDetector::Update(
            const Maths::Vector3& currentPosition,
            float dt,
            bool shouldBeMoving
        ) {
            if (!initialised) {
                Reset(currentPosition);
                return false;
            }

            if (!shouldBeMoving) {
                Reset(currentPosition);
                return false;
            }

            float movement =
    Maths::Vector::Length(currentPosition - lastPosition);

if (movement >= movementThreshold) {
    lastPosition = currentPosition;
    stationaryTime = 0.0f;
}
else if (dt > 0.0f) {
    stationaryTime += dt;
}

return stationaryTime >= stuckTimeThreshold;
        }

        float StuckAgentDetector::GetStationaryTime() const {
            return stationaryTime;
        }

 float StuckAgentDetector::GetDistanceFromLastProgressPosition(
            const Maths::Vector3& currentPosition
        ) const {
            if (!initialised) {
                return 0.0f;
            }

            return Maths::Vector::Length(
                currentPosition - lastPosition
            );
        }

    }
}