#include <gtest/gtest.h>

#include "../CSC8503/ValidationResultJsonWriter.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

using namespace NCL::CSC8503;

TEST(ValidationResultJsonWriterTests, EmptyCollectorProducesEmptyResultSet) {
    ValidationResultCollector collector;

    const std::string json =
        ValidationResultJsonWriter::ToJson(collector);

    EXPECT_NE(
        json.find("\"schemaVersion\": 1"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"total\": 0"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"passed\": 0"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"failed\": 0"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"hasFailures\": false"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"results\": ["),
        std::string::npos
    );
}

TEST(ValidationResultJsonWriterTests, SummaryReflectsCollectedResults) {
    ValidationResultCollector collector;

    ValidationResult passed;
    passed.status = ValidationStatus::Pass;

    ValidationResult failed;
    failed.status = ValidationStatus::Fail;

    collector.AddResult(passed);
    collector.AddResult(failed);
    collector.AddResult(passed);

    const std::string json =
        ValidationResultJsonWriter::ToJson(collector);

    EXPECT_NE(json.find("\"total\": 3"), std::string::npos);
    EXPECT_NE(json.find("\"passed\": 2"), std::string::npos);
    EXPECT_NE(json.find("\"failed\": 1"), std::string::npos);
    EXPECT_NE(
        json.find("\"hasFailures\": true"),
        std::string::npos
    );
}

TEST(ValidationResultJsonWriterTests, SerializesValidationResultFields) {
    ValidationResultCollector collector;

    ValidationResult result;
    result.validatorId = "AI_STUCK";
    result.entityId = "Enemy_1";
    result.status = ValidationStatus::Fail;
    result.message = "Enemy failed to make progress";
    result.timestampSeconds = 18.5f;

    collector.AddResult(result);

    const std::string json =
        ValidationResultJsonWriter::ToJson(collector);

    EXPECT_NE(
        json.find("\"validatorId\": \"AI_STUCK\""),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"entityId\": \"Enemy_1\""),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"status\": \"FAIL\""),
        std::string::npos
    );
    EXPECT_NE(
        json.find(
            "\"message\": \"Enemy failed to make progress\""
        ),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"timestampSeconds\": 18.5"),
        std::string::npos
    );
}

TEST(ValidationResultJsonWriterTests, SerializesPassAndFailStatuses) {
    ValidationResultCollector collector;

    ValidationResult passed;
    passed.entityId = "Enemy_1";
    passed.status = ValidationStatus::Pass;

    ValidationResult failed;
    failed.entityId = "Enemy_2";
    failed.status = ValidationStatus::Fail;

    collector.AddResult(passed);
    collector.AddResult(failed);

    const std::string json =
        ValidationResultJsonWriter::ToJson(collector);

    EXPECT_NE(
        json.find("\"status\": \"PASS\""),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"status\": \"FAIL\""),
        std::string::npos
    );
}

TEST(ValidationResultJsonWriterTests, EscapesSpecialCharacters) {
    ValidationResultCollector collector;

    ValidationResult result;
    result.validatorId = "AI\"STUCK";
    result.entityId = "Enemy\\1";
    result.message = "Line1\nLine2\tTabbed";
    result.status = ValidationStatus::Fail;

    collector.AddResult(result);

    const std::string json =
        ValidationResultJsonWriter::ToJson(collector);

    EXPECT_NE(
        json.find("AI\\\"STUCK"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("Enemy\\\\1"),
        std::string::npos
    );
    EXPECT_NE(
        json.find("Line1\\nLine2\\tTabbed"),
        std::string::npos
    );
}

TEST(ValidationResultJsonWriterTests, WritesJsonToFile) {
    ValidationResultCollector collector;

    ValidationResult result;
    result.validatorId = "AI_STUCK";
    result.entityId = "Enemy_1";
    result.status = ValidationStatus::Fail;

    collector.AddResult(result);

    const std::string filePath =
        "validation_result_writer_test.json";

    ASSERT_TRUE(
        ValidationResultJsonWriter::WriteToFile(
            collector,
            filePath
        )
    );

    std::ifstream input(filePath);
    ASSERT_TRUE(input.is_open());

    std::ostringstream contents;
    contents << input.rdbuf();
    input.close();

    const std::string json = contents.str();

    EXPECT_NE(
        json.find("\"validatorId\": \"AI_STUCK\""),
        std::string::npos
    );
    EXPECT_NE(
        json.find("\"entityId\": \"Enemy_1\""),
        std::string::npos
    );

    std::remove(filePath.c_str());
}
