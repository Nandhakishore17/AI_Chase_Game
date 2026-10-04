#pragma once

#include "ValidationResult.h"

#include <cstddef>
#include <vector>

namespace NCL {
    namespace CSC8503 {

        class ValidationResultCollector {
        public:
            void AddResult(const ValidationResult& result);
            void Clear();

            const std::vector<ValidationResult>& GetResults() const;

            std::size_t GetResultCount() const;
            std::size_t GetPassCount() const;
            std::size_t GetFailCount() const;

            bool HasFailures() const;

        private:
            std::vector<ValidationResult> results;
        };

    }
}
