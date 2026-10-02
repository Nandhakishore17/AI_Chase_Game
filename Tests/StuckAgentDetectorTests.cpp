#include <gtest/gtest.h>

#include "../CSC8503/StuckAgentDetector.h"

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

TEST(StuckAgentDetectorTests, FirstObservationDoesNotReportStuck) {
    StuckAgentDetector detector;

    EXPECT_FALSE(
        detector.Update(Vector3(0, 0, 0), 1.0f, true)
    );
}

TEST(StuckAgentDetectorTests, StationaryAgentBelowTimeThresholdIsNotStuck) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    EXPECT_FALSE(
        detector.Update(Vector3(0, 0, 0), 0.9f, true)
    );
}

TEST(StuckAgentDetectorTests, StationaryAgentAtTimeThresholdIsStuck) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    EXPECT_TRUE(
        detector.Update(Vector3(0, 0, 0), 1.0f, true)
    );
}

TEST(StuckAgentDetectorTests, MovementResetsStationaryTimer) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    EXPECT_FALSE(
        detector.Update(Vector3(1, 0, 0), 1.0f, true)
    );

    EXPECT_FLOAT_EQ(detector.GetStationaryTime(), 0.0f);
}

TEST(StuckAgentDetectorTests, ExpectedStationaryBehaviourDoesNotReportStuck) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, false);
    detector.Update(Vector3(0, 0, 0), 1.0f, false);
    detector.Update(Vector3(0, 0, 0), 1.0f, false);
    detector.Update(Vector3(0, 0, 0), 1.0f, false);

    EXPECT_FALSE(
        detector.Update(Vector3(0, 0, 0), 1.0f, false)
    );

    EXPECT_FLOAT_EQ(detector.GetStationaryTime(), 0.0f);
}

TEST(StuckAgentDetectorTests, MovementAtExactThresholdResetsTimer) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    EXPECT_FALSE(
        detector.Update(Vector3(0.1f, 0, 0), 1.0f, true)
    );

    EXPECT_FLOAT_EQ(detector.GetStationaryTime(), 0.0f);
}

TEST(StuckAgentDetectorTests, MovementBelowThresholdAccumulatesStationaryTime) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    EXPECT_FALSE(
        detector.Update(Vector3(0.05f, 0, 0), 1.0f, true)
    );

    EXPECT_FLOAT_EQ(detector.GetStationaryTime(), 1.0f);
}

TEST(StuckAgentDetectorTests, ResetClearsPreviousStationaryHistory) {
    StuckAgentDetector detector;

    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);
    detector.Update(Vector3(0, 0, 0), 1.0f, true);

    detector.Reset(Vector3(10, 0, 10));

    EXPECT_FLOAT_EQ(detector.GetStationaryTime(), 0.0f);

    EXPECT_FALSE(
        detector.Update(Vector3(10, 0, 10), 1.0f, true)
    );
}

TEST(StuckAgentDetectorTests, CumulativeSmallMovementsPreventFalseStuckDetection) {
    StuckAgentDetector detector;

    Vector3 position(0, 0, 0);

    EXPECT_FALSE(
        detector.Update(position, 0.5f, true)
    );

    for (int i = 0; i < 20; ++i) {
        position.x += 0.05f;

        EXPECT_FALSE(
            detector.Update(position, 0.5f, true)
        );
    }
}