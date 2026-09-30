\# AI Chase â€” Game Quality Engineering Test Strategy

\## 1. Purpose

This document defines the test strategy for the AI Chase Delivery Game and its extension into an automated Game Quality Engineering and Validation platform.

The project was originally developed as a C++ gameplay project and contains gameplay systems including player movement, enemy AI, navigation/pathfinding, physics and collision, fragile-item handling, delivery mechanics, puzzle interactions, bonuses, grappling and HUD/game-state logic.

The purpose of this quality-engineering work is to apply structured software testing, automation, runtime validation, regression testing and continuous integration practices to these real-time game systems.

\---

\## 2. Quality Objectives

The primary objectives are to:

\- Verify critical gameplay systems behave according to expected rules.

\- Detect regressions introduced by code changes.

\- Validate AI and navigation behaviour using measurable runtime conditions.

\- Test positive, negative, boundary and failure scenarios.

\- Reduce reliance on visual/manual verification where automation is practical.

\- Produce reproducible evidence when a test fails.

\- Generate structured test results suitable for automated reporting.

\- Integrate automated validation into a continuous integration pipeline.

\- Maintain manual and exploratory testing for behaviours that are difficult or inappropriate to automate.

\---

\## 3. Systems Under Test

The current test scope includes:

\### Player

\- Player movement

\- Player interaction

\- Collision behaviour

\- Gameplay boundaries

\### Enemy AI

\- Enemy spawning

\- Patrol behaviour

\- Chase behaviour

\- Search behaviour

\- Return behaviour

\- AI state transitions

\- Multiple-enemy behaviour

\### Navigation and Pathfinding

\- Valid path generation

\- Unreachable destinations

\- Obstacle navigation

\- Path consistency

\- Repeated pathfinding execution

\- Invalid and boundary conditions

\### Physics and Collision

\- Player/world collision

\- Enemy/world collision

\- Gameplay-object collision

\- Collision edge cases

\### Fragile Item

\- Item availability

\- Pickup

\- Carry

\- Drop

\- Break conditions

\- Repeated interaction

\### Delivery System

\- Delivery-zone detection

\- Valid delivery

\- Invalid delivery

\- Delivery-distance boundary conditions

\- Score/state updates following delivery

\### Puzzle System

\- Pressure-button interaction

\- Door behaviour

\- Valid and abnormal interaction sequences

\### Grappling

\- Valid grapple targets

\- Invalid grapple targets

\- Line-of-sight behaviour

\- Unusual geometry and boundary cases

\### Bonuses

\- Bonus spawning

\- Bonus interaction

\- Gameplay effects

\### HUD and Game State

\- Score

\- Delivery status

\- Item status

\- Distance information

\- Enemy state information

\- Game-state transitions

\---

\## 4. Test Levels

\### Unit Testing

Individual game-system logic will be tested independently where practical.

Initial candidates include:

\- Pathfinding

\- AI state logic

\- Delivery rules

\- Score calculations

\- Fragile-item rules

C++ automated tests will be introduced using GoogleTest with CMake/CTest integration.

\### Integration Testing

Integration tests will verify interactions between systems, for example:

\- AI + navigation

\- Player + collision

\- Fragile item + delivery

\- Delivery + scoring

\- Pressure button + door

\- Gameplay systems + HUD

\### Runtime Validation

Runtime validators will measure behaviours that cannot be sufficiently validated through isolated unit tests.

Planned validators include:

\- Stuck-agent detection

\- Target-reach validation

\- AI-state transition validation

\- Path validation

\- Delivery validation

\- Timeout detection

\### Regression Testing

Automated regression suites will be executed after changes to determine whether previously working functionality has been affected.

\### Exploratory Testing

Manual exploratory sessions will be used to identify unexpected behaviour and investigate scenarios not adequately represented by scripted tests.

\### Non-Functional Testing

Where applicable, the project will measure:

\- Pathfinding execution time

\- Scenario execution time

\- Stability

\- Multiple-agent behaviour

\- Supporting service/API response performance

\---

\## 5. Test Design Techniques

The project will use:

\- Positive testing

\- Negative testing

\- Boundary-value analysis

\- Equivalence partitioning

\- State-transition testing

\- Error guessing

\- Exploratory testing

\- Risk-based testing

\- Regression testing

\---

\## 6. Automation Strategy

Automation will be introduced at multiple layers.

\### Layer 1 â€” C++ Component Tests

GoogleTest and CTest will validate isolated game logic.

\### Layer 2 â€” Runtime Game Validation

Instrumentation will evaluate live gameplay behaviour such as AI movement, target reaching and state transitions.

\### Layer 3 â€” Scenario Automation

Automated scenarios will execute reproducible gameplay-validation conditions.

\### Layer 4 â€” Test Orchestration

Python tooling will execute scenarios, collect results and generate reports.

\### Layer 5 â€” Service/API Validation

Where supporting test services are introduced, REST API behaviour will be validated using automated API tests and Postman collections.

\### Layer 6 â€” Performance Validation

Relevant service and runtime performance characteristics will be measured using appropriate tooling, including JMeter where applicable.

\### Layer 7 â€” Continuous Integration

Jenkins will build the project, execute appropriate automated test suites, collect results and provide quality-gate evidence.

\---

\## 7. Test Evidence

Testing evidence may include:

\- Automated test output

\- Structured JSON results

\- Logs

\- Screenshots

\- Gameplay recordings

\- CI pipeline results

\- Performance reports

\- Defect reports

\- Root-cause analysis

\- Regression results

A test will not be marked as passed unless sufficient evidence exists to support the result.

\---

\## 8. Defect Management

Defects will contain, where applicable:

\- Defect ID

\- Summary

\- Environment/build

\- Preconditions

\- Reproduction steps

\- Expected result

\- Actual result

\- Severity

\- Priority

\- Supporting evidence

\- Logs

\- Root-cause analysis

\- Fix reference

\- Regression result

\---

\## 9. Risk-Based Testing

Higher testing priority will be assigned to systems where failure significantly affects gameplay or blocks progression.

Initial high-risk areas include:

\- Navigation/pathfinding

\- Enemy AI

\- Player movement/collision

\- Fragile-item lifecycle

\- Delivery/progression logic

\- Game stability

Risk priorities will be documented separately in the project Risk Matrix.

\---

\## 10. Entry Criteria

Testing can begin when:

\- The project builds successfully.

\- The game can launch.

\- Required test environment/configuration is available.

\- The system being tested is sufficiently implemented.

\- Expected behaviour can be identified.

\---

\## 11. Exit Criteria

A test cycle may be considered complete when:

\- Planned critical tests have been executed.

\- Critical failures have been investigated.

\- Regression testing has completed.

\- Test results have been recorded.

\- Known unresolved issues are documented.

\- Required evidence has been retained.

Passing every test is not required to complete a test cycle; known failures may remain if they are understood, documented and accepted for the current build.

\---

\## 12. Baseline Status

A baseline gameplay verification was performed before introducing the new Game Quality Engineering framework.

The following behaviours were observed successfully during the baseline session:

\- Game execution

\- Player movement

\- Enemy presence

\- Enemy movement

\- Enemy/player gameplay interaction

\- Fragile-item availability/carry state

\- Delivery-system presence

\- HUD/diagnostic information

\- Session stability during the recorded test period

The following require additional controlled validation before being classified as verified:

\- Complete patrol behaviour

\- Repeated fragile-item pickup/drop behaviour

\- Delivery boundary behaviour

\- Abnormal pressure-button interaction

\- Grapple edge cases

\- Reproducible enemy stuck conditions

This baseline will be used as a reference while automated testing and runtime validation are introduced.
