#include "ValidationResultCollector.h"

namespace NCL {
    namespace CSC8503 {

        void ValidationResultCollector::AddResult(
            const ValidationResult& result
        ) {
            results.push_back(result);
        }

        void ValidationResultCollector::Clear() {
            results.clear();
        }

        const std::vector<ValidationResult>&
            ValidationResultCollector::GetResults() const {
            return results;
        }

        std::size_t ValidationResultCollector::GetResultCount() const {
            return results.size();
        }

        std::size_t ValidationResultCollector::GetPassCount() const {
            std::size_t passCount = 0;

            for (const ValidationResult& result : results) {
                if (result.IsPassed()) {
                    ++passCount;
                }
            }

            return passCount;
        }

        std::size_t ValidationResultCollector::GetFailCount() const {
            std::size_t failCount = 0;

            for (const ValidationResult& result : results) {
                if (!result.IsPassed()) {
                    ++failCount;
                }
            }

            return failCount;
        }

        bool ValidationResultCollector::HasFailures() const {
            return GetFailCount() > 0;
        }

    }
}
