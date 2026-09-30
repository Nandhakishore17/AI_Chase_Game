#include <gtest/gtest.h>
#include "GameplayRules.h"

using namespace NCL;
using namespace NCL::CSC8503;

// -----------------------------------------------------------------------------
// Pickup Rules
// -----------------------------------------------------------------------------

// AT-ITEM-001
TEST(GameplayRulesTests, AllowsPickupInsidePickupDistance) {
    EXPECT_TRUE(
        GameplayRules::CanPickupItem(11.99f, false, false)
    );
}

// AT-ITEM-002
TEST(GameplayRulesTests, RejectsPickupAtExactPickupBoundary) {
    EXPECT_FALSE(
        GameplayRules::CanPickupItem(12.0f, false, false)
    );
}

// AT-ITEM-003
TEST(GameplayRulesTests, RejectsPickupWhenAlreadyCarried) {
    EXPECT_FALSE(
        GameplayRules::CanPickupItem(5.0f, true, false)
    );
}

// AT-ITEM-004
TEST(GameplayRulesTests, RejectsPickupWhenItemIsBroken) {
    EXPECT_FALSE(
        GameplayRules::CanPickupItem(5.0f, false, true)
    );
}


// -----------------------------------------------------------------------------
// Delivery Rules
// -----------------------------------------------------------------------------

// AT-DEL-001
TEST(GameplayRulesTests, AllowsDeliveryInsideDeliveryDistance) {
    EXPECT_TRUE(
        GameplayRules::IsValidDelivery(7.99f, false)
    );
}

// AT-DEL-002
TEST(GameplayRulesTests, RejectsDeliveryAtExactBoundary) {
    EXPECT_FALSE(
        GameplayRules::IsValidDelivery(8.0f, false)
    );
}

// AT-DEL-003
TEST(GameplayRulesTests, RejectsDeliveryWhenItemIsBroken) {
    EXPECT_FALSE(
        GameplayRules::IsValidDelivery(5.0f, true)
    );
}


// -----------------------------------------------------------------------------
// Fragile Item Break Rules
// -----------------------------------------------------------------------------

// AT-ITEM-005
TEST(GameplayRulesTests, DoesNotBreakBelowImpactThreshold) {
    EXPECT_FALSE(
        GameplayRules::ShouldBreakItem(29.99f)
    );
}

// AT-ITEM-006
TEST(GameplayRulesTests, DoesNotBreakAtExactImpactThreshold) {
    EXPECT_FALSE(
        GameplayRules::ShouldBreakItem(30.0f)
    );
}

// AT-ITEM-007
TEST(GameplayRulesTests, BreaksAboveImpactThreshold) {
    EXPECT_TRUE(
        GameplayRules::ShouldBreakItem(30.01f)
    );
}


// -----------------------------------------------------------------------------
// Delivery Completion Rules
// -----------------------------------------------------------------------------

// AT-DEL-004
TEST(GameplayRulesTests, DeliveryIsIncompleteBelowRequiredCount) {
    EXPECT_FALSE(
        GameplayRules::IsDeliveryComplete(0, 1)
    );
}

// AT-DEL-005
TEST(GameplayRulesTests, DeliveryCompletesAtRequiredCount) {
    EXPECT_TRUE(
        GameplayRules::IsDeliveryComplete(1, 1)
    );
}

// AT-DEL-006
TEST(GameplayRulesTests, DeliveryRemainsCompleteAboveRequiredCount) {
    EXPECT_TRUE(
        GameplayRules::IsDeliveryComplete(2, 1)
    );
}