#include <gtest/gtest.h>

#include "../CSC8503/ValidationResult.h"

using namespace NCL::CSC8503;

TEST(ValidationResultTests, DefaultStatusIsPass) {
    ValidationResult result;

    EXPECT_EQ(result.status, ValidationStatus::Pass);
    EXPECT_TRUE(result.IsPassed());
}

TEST(ValidationResultTests, PassStatusReportsPassed) {
    ValidationResult result;
    result.status = ValidationStatus::Pass;

    EXPECT_TRUE(result.IsPassed());
}

TEST(ValidationResultTests, FailStatusReportsNotPassed) {
    ValidationResult result;
    result.status = ValidationStatus::Fail;

    EXPECT_FALSE(result.IsPassed());
}

TEST(ValidationResultTests, StoresValidationEventData) {
    ValidationResult result;

    result.validatorId = "AI_STUCK";
    result.entityId = "Enemy_1";
    result.status = ValidationStatus::Fail;
    result.message =
        "Enemy failed to make meaningful movement progress";
    result.timestampSeconds = 18.42f;

    EXPECT_EQ(result.validatorId, "AI_STUCK");
    EXPECT_EQ(result.entityId, "Enemy_1");
    EXPECT_EQ(result.status, ValidationStatus::Fail);
    EXPECT_EQ(
        result.message,
        "Enemy failed to make meaningful movement progress"
    );
    EXPECT_FLOAT_EQ(result.timestampSeconds, 18.42f);
    EXPECT_FALSE(result.IsPassed());
}
