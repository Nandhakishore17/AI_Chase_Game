const fs = require("fs");
const path = require("path");

const { test, expect } = require("@playwright/test");

const validFixturePath = path.resolve(
    "Automation/tests/fixtures/valid_validation_results.json"
);

const malformedFixturePath = path.resolve(
    "Automation/tests/fixtures/malformed_validation_results.json"
);

const runtimeResultPath = path.resolve(
    "Automation/tests/runtime/playwright_validation_results.json"
);

test.beforeEach(() => {
    fs.mkdirSync(
        path.dirname(runtimeResultPath),
        { recursive: true }
    );

    fs.copyFileSync(
        validFixturePath,
        runtimeResultPath
    );
});

test("health endpoint reports API is operational", async ({ request }) => {
    const response = await request.get("/health");

    expect(response.status()).toBe(200);
    expect(await response.json()).toEqual({
        status: "ok",
    });
});

test("validation results endpoint returns validated result data", async ({ request }) => {
    const response = await request.get("/validation/results");

    expect(response.status()).toBe(200);

    const body = await response.json();

    expect(body.schemaVersion).toBe(1);

    expect(body.summary).toEqual({
        total: 2,
        passed: 1,
        failed: 1,
        hasFailures: true,
    });

    expect(body.results).toHaveLength(2);

    expect(body.results[0]).toMatchObject({
        validatorId: "AI_STUCK",
        entityId: "Enemy_1",
        status: "PASS",
    });

    expect(body.results[1]).toMatchObject({
        validatorId: "AI_STUCK",
        entityId: "Enemy_2",
        status: "FAIL",
    });
});

test("validation results endpoint returns 404 when result file is missing", async ({ request }) => {
    fs.rmSync(
        runtimeResultPath,
        { force: true }
    );

    const response = await request.get("/validation/results");

    expect(response.status()).toBe(404);
    expect(await response.json()).toEqual({
        detail: "Validation results file was not found.",
    });
});

test("validation results endpoint returns 422 for malformed JSON", async ({ request }) => {
    fs.copyFileSync(
        malformedFixturePath,
        runtimeResultPath
    );

    const response = await request.get("/validation/results");

    expect(response.status()).toBe(422);

    const body = await response.json();

    expect(body.detail).toContain("Invalid JSON:");
});