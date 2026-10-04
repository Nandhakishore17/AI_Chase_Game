#include <gtest/gtest.h>

#include "../CSC8503/ValidationResultCollector.h"

using namespace NCL::CSC8503;

TEST(ValidationResultCollectorTests, NewCollectorIsEmpty) {
    ValidationResultCollector collector;

    EXPECT_EQ(collector.GetResultCount(), 0);
    EXPECT_EQ(collector.GetPassCount(), 0);
    EXPECT_EQ(collector.GetFailCount(), 0);
    EXPECT_FALSE(collector.HasFailures());
    EXPECT_TRUE(collector.GetResults().empty());
}

TEST(ValidationResultCollectorTests, StoresAddedResult) {
    ValidationResultCollector collector;

    ValidationResult result;
    result.validatorId = "AI_STUCK";
    result.entityId = "Enemy_1";
    result.status = ValidationStatus::Fail;
    result.message = "Enemy failed to make movement progress";
    result.timestampSeconds = 12.5f;

    collector.AddResult(result);

    ASSERT_EQ(collector.GetResultCount(), 1);

    const ValidationResult& stored =
        collector.GetResults().front();

    EXPECT_EQ(stored.validatorId, "AI_STUCK");
    EXPECT_EQ(stored.entityId, "Enemy_1");
    EXPECT_EQ(stored.status, ValidationStatus::Fail);
    EXPECT_EQ(
        stored.message,
        "Enemy failed to make movement progress"
    );
    EXPECT_FLOAT_EQ(stored.timestampSeconds, 12.5f);
}

TEST(ValidationResultCollectorTests, CountsPassAndFailResults) {
    ValidationResultCollector collector;

    ValidationResult passed;
    passed.status = ValidationStatus::Pass;

    ValidationResult failed;
    failed.status = ValidationStatus::Fail;

    collector.AddResult(passed);
    collector.AddResult(failed);
    collector.AddResult(passed);

    EXPECT_EQ(collector.GetResultCount(), 3);
    EXPECT_EQ(collector.GetPassCount(), 2);
    EXPECT_EQ(collector.GetFailCount(), 1);
    EXPECT_TRUE(collector.HasFailures());
}

TEST(ValidationResultCollectorTests, PreservesInsertionOrder) {
    ValidationResultCollector collector;

    ValidationResult first;
    first.entityId = "Enemy_1";

    ValidationResult second;
    second.entityId = "Enemy_2";

    collector.AddResult(first);
    collector.AddResult(second);

    ASSERT_EQ(collector.GetResults().size(), 2);
    EXPECT_EQ(collector.GetResults()[0].entityId, "Enemy_1");
    EXPECT_EQ(collector.GetResults()[1].entityId, "Enemy_2");
}

TEST(ValidationResultCollectorTests, PassOnlyResultsDoNotReportFailure) {
    ValidationResultCollector collector;

    ValidationResult first;
    first.status = ValidationStatus::Pass;

    ValidationResult second;
    second.status = ValidationStatus::Pass;

    collector.AddResult(first);
    collector.AddResult(second);

    EXPECT_EQ(collector.GetPassCount(), 2);
    EXPECT_EQ(collector.GetFailCount(), 0);
    EXPECT_FALSE(collector.HasFailures());
}

TEST(ValidationResultCollectorTests, ClearRemovesAllResults) {
    ValidationResultCollector collector;

    ValidationResult failed;
    failed.status = ValidationStatus::Fail;

    collector.AddResult(failed);

    ASSERT_EQ(collector.GetResultCount(), 1);
    ASSERT_TRUE(collector.HasFailures());

    collector.Clear();

    EXPECT_EQ(collector.GetResultCount(), 0);
    EXPECT_EQ(collector.GetPassCount(), 0);
    EXPECT_EQ(collector.GetFailCount(), 0);
    EXPECT_FALSE(collector.HasFailures());
    EXPECT_TRUE(collector.GetResults().empty());
}
