#pragma once

#include <string>

namespace NCL {
    namespace CSC8503 {

        enum class ValidationStatus {
            Pass,
            Fail
        };

        struct ValidationResult {
            std::string validatorId;
            std::string entityId;
            ValidationStatus status = ValidationStatus::Pass;
            std::string message;
            float timestampSeconds = 0.0f;

            bool IsPassed() const {
                return status == ValidationStatus::Pass;
            }
        };

    }
}
