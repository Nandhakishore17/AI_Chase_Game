import json
import tempfile
import unittest
from pathlib import Path

from Automation.validation_processor import (
    ValidationContractError,
    load_validation_results,
    validate_contract,
)


class ValidationProcessorTests(unittest.TestCase):

    def test_valid_contract_is_accepted(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 0,
                "passed": 0,
                "failed": 0,
                "hasFailures": False,
            },
            "results": [],
        }

        validate_contract(data)

    def test_non_object_root_is_rejected(self):
        with self.assertRaisesRegex(
            ValidationContractError,
            "root must be a JSON object",
        ):
            validate_contract([])

    def test_missing_required_root_field_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "results": [],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "Missing required root field",
        ):
            validate_contract(data)

    def test_unsupported_schema_version_is_rejected(self):
        data = {
            "schemaVersion": 2,
            "summary": {
                "total": 0,
                "passed": 0,
                "failed": 0,
                "hasFailures": False,
            },
            "results": [],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "Unsupported schemaVersion",
        ):
            validate_contract(data)

    def test_invalid_summary_type_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "summary": [],
            "results": [],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary must be a JSON object",
        ):
            validate_contract(data)

    def test_invalid_results_type_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 0,
                "passed": 0,
                "failed": 0,
                "hasFailures": False,
            },
            "results": {},
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "results must be a JSON array",
        ):
            validate_contract(data)

    def test_missing_file_is_rejected(self):
        missing_path = Path(
            "file_that_does_not_exist_validation.json"
        )

        with self.assertRaises(FileNotFoundError):
            load_validation_results(missing_path)

    def test_malformed_json_file_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            file_path = Path(directory) / "invalid.json"
            file_path.write_text(
                "{ invalid json",
                encoding="utf-8",
            )

            with self.assertRaisesRegex(
                ValidationContractError,
                "Invalid JSON",
            ):
                load_validation_results(file_path)

    def test_valid_json_file_is_loaded(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 0,
                "passed": 0,
                "failed": 0,
                "hasFailures": False,
            },
            "results": [],
        }

        with tempfile.TemporaryDirectory() as directory:
            file_path = Path(directory) / "valid.json"
            file_path.write_text(
                json.dumps(data),
                encoding="utf-8",
            )

            loaded = load_validation_results(file_path)

        self.assertEqual(loaded, data)



    def test_summary_total_must_match_result_count(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 2,
                "passed": 1,
                "failed": 1,
                "hasFailures": True,
            },
            "results": [],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary total does not match result count",
        ):
            validate_contract(data)

    def test_summary_counts_must_add_up_to_total(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 1,
                "failed": 1,
                "hasFailures": True,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "FAIL",
                    "message": "Enemy stuck",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary passed and failed counts do not match total",
        ):
            validate_contract(data)

    def test_has_failures_must_match_failed_count(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 0,
                "failed": 1,
                "hasFailures": False,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "FAIL",
                    "message": "Enemy stuck",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary hasFailures is inconsistent with failed count",
        ):
            validate_contract(data)

    def test_result_missing_required_field_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 0,
                "failed": 1,
                "hasFailures": True,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "FAIL",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "Missing required result field",
        ):
            validate_contract(data)

    def test_invalid_result_status_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 0,
                "failed": 1,
                "hasFailures": True,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "UNKNOWN",
                    "message": "Unexpected state",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "Invalid result status",
        ):
            validate_contract(data)

    def test_summary_pass_count_must_match_result_statuses(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 1,
                "failed": 0,
                "hasFailures": False,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "FAIL",
                    "message": "Enemy stuck",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary status counts do not match result statuses",
        ):
            validate_contract(data)

    def test_summary_fail_count_must_match_result_statuses(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 0,
                "failed": 1,
                "hasFailures": True,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "PASS",
                    "message": "Enemy movement healthy",
                    "timestampSeconds": 3.0,
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "summary status counts do not match result statuses",
        ):
            validate_contract(data)

    def test_invalid_timestamp_type_is_rejected(self):
        data = {
            "schemaVersion": 1,
            "summary": {
                "total": 1,
                "passed": 1,
                "failed": 0,
                "hasFailures": False,
            },
            "results": [
                {
                    "validatorId": "AI_STUCK",
                    "entityId": "Enemy_1",
                    "status": "PASS",
                    "message": "Enemy movement healthy",
                    "timestampSeconds": "3.0",
                }
            ],
        }

        with self.assertRaisesRegex(
            ValidationContractError,
            "timestampSeconds",
        ):
            validate_contract(data)

if __name__ == "__main__":
    unittest.main()
