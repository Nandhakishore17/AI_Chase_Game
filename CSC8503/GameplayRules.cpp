#include "GameplayRules.h"

namespace NCL {
    namespace CSC8503 {

        bool GameplayRules::CanPickupItem(
            float playerItemDistance,
            bool itemCarried,
            bool itemBroken
        ) {
            return !itemCarried &&
                   !itemBroken &&
                   playerItemDistance < PickupDistance;
        }

        bool GameplayRules::IsValidDelivery(
            float itemDeliveryDistance,
            bool itemBroken
        ) {
            return !itemBroken &&
                   itemDeliveryDistance < DeliveryDistance;
        }

        bool GameplayRules::ShouldBreakItem(
            float relativeImpact
        ) {
            return relativeImpact > BreakImpactThreshold;
        }

        bool GameplayRules::IsDeliveryComplete(
            int deliveredItems,
            int totalItems
        ) {
            return deliveredItems >= totalItems;
        }

    }
}
