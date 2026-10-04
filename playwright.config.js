const { defineConfig } = require("@playwright/test");

module.exports = defineConfig({
    testDir: "./Automation/playwright",
    timeout: 10000,
    workers: 1,

    use: {
        baseURL: "http://127.0.0.1:8000",
    },

    webServer: {
        command:
            "python -m uvicorn validation_api:app --app-dir Automation --host 127.0.0.1 --port 8000",
        url: "http://127.0.0.1:8000/health",
        timeout: 10000,
        reuseExistingServer: false,
        env: {
            ...process.env,
            VALIDATION_RESULTS_PATH:
                "./Automation/tests/runtime/playwright_validation_results.json",
        },
    },
});