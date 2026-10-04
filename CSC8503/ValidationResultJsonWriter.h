#pragma once

#include "ValidationResultCollector.h"

#include <string>

namespace NCL {
    namespace CSC8503 {

        class ValidationResultJsonWriter {
        public:
            static std::string ToJson(
                const ValidationResultCollector& collector
            );

            static bool WriteToFile(
                const ValidationResultCollector& collector,
                const std::string& filePath
            );

        private:
            static std::string EscapeJsonString(
                const std::string& value
            );

            static std::string StatusToString(
                ValidationStatus status
            );
        };

    }
}
