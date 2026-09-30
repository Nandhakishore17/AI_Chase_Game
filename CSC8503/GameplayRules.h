#pragma once

namespace NCL {
    namespace CSC8503 {

        class GameplayRules {
        public:
            static constexpr float PickupDistance = 12.0f;
            static constexpr float DeliveryDistance = 8.0f;
            static constexpr float BreakImpactThreshold = 30.0f;
            static constexpr int DeliveryScore = 10;

            static bool CanPickupItem(
                float playerItemDistance,
                bool itemCarried,
                bool itemBroken
            );

            static bool IsValidDelivery(
                float itemDeliveryDistance,
                bool itemBroken
            );

            static bool ShouldBreakItem(
                float relativeImpact
            );

            static bool IsDeliveryComplete(
                int deliveredItems,
                int totalItems
            );
        };

    }
}
