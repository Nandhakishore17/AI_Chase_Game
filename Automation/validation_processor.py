import json
from pathlib import Path
from typing import Any


SUPPORTED_SCHEMA_VERSION = 1

REQUIRED_SUMMARY_FIELDS = {
    "total",
    "passed",
    "failed",
    "hasFailures",
}

REQUIRED_RESULT_FIELDS = {
    "validatorId",
    "entityId",
    "status",
    "message",
    "timestampSeconds",
}

VALID_STATUSES = {
    "PASS",
    "FAIL",
}


class ValidationContractError(Exception):
    """Raised when validation result JSON violates the expected contract."""


def load_validation_results(file_path: str | Path) -> dict[str, Any]:
    path = Path(file_path)

    if not path.is_file():
        raise FileNotFoundError(
            f"Validation result file not found: {path}"
        )

    try:
        with path.open("r", encoding="utf-8") as file:
            data = json.load(file)
    except json.JSONDecodeError as error:
        raise ValidationContractError(
            f"Invalid JSON: {error.msg}"
        ) from error

    validate_contract(data)

    return data


def validate_contract(data: Any) -> None:
    if not isinstance(data, dict):
        raise ValidationContractError(
            "Validation result root must be a JSON object."
        )

    required_fields = {
        "schemaVersion",
        "summary",
        "results",
    }

    missing_fields = required_fields - data.keys()

    if missing_fields:
        missing = ", ".join(sorted(missing_fields))
        raise ValidationContractError(
            f"Missing required root field(s): {missing}"
        )

    if data["schemaVersion"] != SUPPORTED_SCHEMA_VERSION:
        raise ValidationContractError(
            "Unsupported schemaVersion: "
            f"{data['schemaVersion']}"
        )

    summary = data["summary"]
    results = data["results"]

    if not isinstance(summary, dict):
        raise ValidationContractError(
            "summary must be a JSON object."
        )

    if not isinstance(results, list):
        raise ValidationContractError(
            "results must be a JSON array."
        )

    missing_summary_fields = (
        REQUIRED_SUMMARY_FIELDS - summary.keys()
    )

    if missing_summary_fields:
        missing = ", ".join(
            sorted(missing_summary_fields)
        )
        raise ValidationContractError(
            f"Missing required summary field(s): {missing}"
        )

    for field_name in ("total", "passed", "failed"):
        value = summary[field_name]

        if type(value) is not int or value < 0:
            raise ValidationContractError(
                f"summary {field_name} must be "
                "a non-negative integer."
            )

    if type(summary["hasFailures"]) is not bool:
        raise ValidationContractError(
            "summary hasFailures must be a boolean."
        )

    for index, result in enumerate(results):
        if not isinstance(result, dict):
            raise ValidationContractError(
                f"result at index {index} "
                "must be a JSON object."
            )

        missing_result_fields = (
            REQUIRED_RESULT_FIELDS - result.keys()
        )

        if missing_result_fields:
            missing = ", ".join(
                sorted(missing_result_fields)
            )
            raise ValidationContractError(
                "Missing required result field(s) "
                f"at index {index}: {missing}"
            )

        for field_name in (
            "validatorId",
            "entityId",
            "message",
        ):
            if not isinstance(result[field_name], str):
                raise ValidationContractError(
                    f"result {field_name} at index "
                    f"{index} must be a string."
                )

        if result["status"] not in VALID_STATUSES:
            raise ValidationContractError(
                "Invalid result status at index "
                f"{index}: {result['status']}"
            )

        timestamp = result["timestampSeconds"]

        if (
            isinstance(timestamp, bool)
            or not isinstance(timestamp, (int, float))
            or timestamp < 0
        ):
            raise ValidationContractError(
                "result timestampSeconds at index "
                f"{index} must be a non-negative number."
            )

    if summary["total"] != len(results):
        raise ValidationContractError(
            "summary total does not match result count."
        )

    if (
        summary["passed"] + summary["failed"]
        != summary["total"]
    ):
        raise ValidationContractError(
            "summary passed and failed counts "
            "do not match total."
        )

    if summary["hasFailures"] != (
        summary["failed"] > 0
    ):
        raise ValidationContractError(
            "summary hasFailures is inconsistent "
            "with failed count."
        )

    actual_passed = sum(
        1
        for result in results
        if result["status"] == "PASS"
    )

    actual_failed = sum(
        1
        for result in results
        if result["status"] == "FAIL"
    )

    if (
        summary["passed"] != actual_passed
        or summary["failed"] != actual_failed
    ):
        raise ValidationContractError(
            "summary status counts do not match result statuses."
        )
