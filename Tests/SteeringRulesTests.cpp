#include <gtest/gtest.h>

#include "../CSC8503/SteeringRules.h"

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

namespace {

    constexpr float Epsilon = 0.0001f;

    TEST(SteeringRulesTests, ForwardAvoidanceIsPreserved) {
        Vector3 navigationDirection(1.0f, 0.0f, 0.0f);
        Vector3 avoidance(2.0f, 0.0f, 0.0f);

        Vector3 corrected =
            SteeringRules::RemoveOpposingAvoidance(
                avoidance,
                navigationDirection
            );

        EXPECT_NEAR(corrected.x, 2.0f, Epsilon);
        EXPECT_NEAR(corrected.y, 0.0f, Epsilon);
        EXPECT_NEAR(corrected.z, 0.0f, Epsilon);
    }

    TEST(SteeringRulesTests, LateralAvoidanceIsPreserved) {
        Vector3 navigationDirection(1.0f, 0.0f, 0.0f);
        Vector3 avoidance(0.0f, 0.0f, 3.0f);

        Vector3 corrected =
            SteeringRules::RemoveOpposingAvoidance(
                avoidance,
                navigationDirection
            );

        EXPECT_NEAR(corrected.x, 0.0f, Epsilon);
        EXPECT_NEAR(corrected.z, 3.0f, Epsilon);
    }

    TEST(SteeringRulesTests, OpposingAvoidanceIsRemoved) {
        Vector3 navigationDirection(1.0f, 0.0f, 0.0f);
        Vector3 avoidance(-3.0f, 0.0f, 0.0f);

        Vector3 corrected =
            SteeringRules::RemoveOpposingAvoidance(
                avoidance,
                navigationDirection
            );

        EXPECT_NEAR(corrected.x, 0.0f, Epsilon);
        EXPECT_NEAR(corrected.y, 0.0f, Epsilon);
        EXPECT_NEAR(corrected.z, 0.0f, Epsilon);
    }

    TEST(SteeringRulesTests, MixedAvoidanceRemovesOnlyOpposingComponent) {
        Vector3 navigationDirection(1.0f, 0.0f, 0.0f);
        Vector3 avoidance(-2.0f, 0.0f, 3.0f);

        Vector3 corrected =
            SteeringRules::RemoveOpposingAvoidance(
                avoidance,
                navigationDirection
            );

        EXPECT_NEAR(corrected.x, 0.0f, Epsilon);
        EXPECT_NEAR(corrected.z, 3.0f, Epsilon);
    }

    TEST(SteeringRulesTests, CorrectedAvoidanceNeverOpposesNavigation) {
        Vector3 navigationDirection(1.0f, 0.0f, 0.0f);
        Vector3 avoidance(-3.0f, 0.0f, 2.0f);

        Vector3 corrected =
            SteeringRules::RemoveOpposingAvoidance(
                avoidance,
                navigationDirection
            );

        float alignment =
            Vector::Dot(
                corrected,
                navigationDirection
            );

        EXPECT_GE(alignment, -Epsilon);
    }

}