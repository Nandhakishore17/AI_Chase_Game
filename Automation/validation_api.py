import os
from pathlib import Path

from fastapi import FastAPI, HTTPException

from validation_processor import (
    ValidationContractError,
    load_validation_results,
)


DEFAULT_RESULTS_PATH = Path(
    "ValidationResults/validation_results.json"
)

app = FastAPI(
    title="AI Chase Validation API",
    version="1.0.0",
)


def get_results_path() -> Path:
    configured_path = os.getenv("VALIDATION_RESULTS_PATH")

    if configured_path:
        return Path(configured_path)

    return DEFAULT_RESULTS_PATH


@app.get("/health")
def health() -> dict[str, str]:
    return {
        "status": "ok",
    }


@app.get("/validation/results")
def validation_results() -> dict:
    results_path = get_results_path()

    try:
        return load_validation_results(results_path)

    except FileNotFoundError as error:
        raise HTTPException(
            status_code=404,
            detail="Validation results file was not found.",
        ) from error

    except ValidationContractError as error:
        raise HTTPException(
            status_code=422,
            detail=str(error),
        ) from error