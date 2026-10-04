#include "ValidationResultJsonWriter.h"

#include <fstream>
#include <iomanip>
#include <sstream>

namespace NCL {
    namespace CSC8503 {

        std::string ValidationResultJsonWriter::ToJson(
            const ValidationResultCollector& collector
        ) {
            std::ostringstream json;

            json << "{\n";
            json << "  \"schemaVersion\": 1,\n";
            json << "  \"summary\": {\n";
            json << "    \"total\": "
                 << collector.GetResultCount() << ",\n";
            json << "    \"passed\": "
                 << collector.GetPassCount() << ",\n";
            json << "    \"failed\": "
                 << collector.GetFailCount() << ",\n";
            json << "    \"hasFailures\": "
                 << (collector.HasFailures() ? "true" : "false")
                 << "\n";
            json << "  },\n";
            json << "  \"results\": [\n";

            const std::vector<ValidationResult>& results =
                collector.GetResults();

            for (std::size_t i = 0; i < results.size(); ++i) {
                const ValidationResult& result = results[i];

                json << "    {\n";
                json << "      \"validatorId\": \""
                     << EscapeJsonString(result.validatorId)
                     << "\",\n";
                json << "      \"entityId\": \""
                     << EscapeJsonString(result.entityId)
                     << "\",\n";
                json << "      \"status\": \""
                     << StatusToString(result.status)
                     << "\",\n";
                json << "      \"message\": \""
                     << EscapeJsonString(result.message)
                     << "\",\n";
                json << "      \"timestampSeconds\": "
                     << std::setprecision(9)
                     << result.timestampSeconds << "\n";
                json << "    }";

                if (i + 1 < results.size()) {
                    json << ",";
                }

                json << "\n";
            }

            json << "  ]\n";
            json << "}\n";

            return json.str();
        }

        bool ValidationResultJsonWriter::WriteToFile(
            const ValidationResultCollector& collector,
            const std::string& filePath
        ) {
            std::ofstream output(filePath);

            if (!output.is_open()) {
                return false;
            }

            output << ToJson(collector);

            return output.good();
        }

        std::string ValidationResultJsonWriter::EscapeJsonString(
            const std::string& value
        ) {
            std::ostringstream escaped;

            for (char character : value) {
                switch (character) {
                    case '"':
                        escaped << "\\\"";
                        break;

                    case '\\':
                        escaped << "\\\\";
                        break;

                    case '\n':
                        escaped << "\\n";
                        break;

                    case '\r':
                        escaped << "\\r";
                        break;

                    case '\t':
                        escaped << "\\t";
                        break;

                    default:
                        escaped << character;
                        break;
                }
            }

            return escaped.str();
        }

        std::string ValidationResultJsonWriter::StatusToString(
            ValidationStatus status
        ) {
            return status == ValidationStatus::Pass
                ? "PASS"
                : "FAIL";
        }

    }
}
