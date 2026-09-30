\# AI Chase â€” Game Quality Engineering Test Plan

\## 1. Test Plan ID

AI-CHASE-QE-TP-001

\## 2. Objective

The objective of this test plan is to verify the functional behaviour, integration, stability and selected non-functional characteristics of the AI Chase Delivery Game while developing an automated Game Quality Engineering framework around the existing C++ project.

The plan combines manual testing, exploratory testing, C++ automated testing, runtime validation, scenario automation, API/service testing where applicable, performance testing and continuous integration.

\---

\## 3. Test Items

The following gameplay systems are included in the current test scope:

\- Player movement

\- Player/world collision

\- Enemy spawning

\- Enemy AI state behaviour

\- Navigation

\- Pathfinding

\- Fragile-item interaction

\- Delivery mechanics

\- Score/progression

\- Pressure-button and door puzzle

\- Grappling

\- Bonus system

\- HUD/game-state information

\- Multiple-enemy behaviour

\- Game stability

Supporting quality-engineering components introduced during this project will also be tested:

\- C++ automated test suite

\- Runtime validation framework

\- Scenario runner

\- Test-result generation

\- Python automation/reporting

\- REST test service, if implemented

\- CI pipeline

\- Performance-test workflow

\---

\## 4. Test Environment

\### Development Environment

\- Operating System: Windows

\- Language: C++

\- Build System: CMake

\- IDE/Development Environment: Visual Studio

\- Source Control: Git / GitHub

\### Planned Quality Engineering Tools

\- GoogleTest

\- CTest

\- Python

\- Postman

\- JMeter

\- Docker

\- Jenkins

Tools will only be recorded as implemented project technologies after successful integration and use.

\---

\## 5. Functional Test Areas

\### Player

Verify:

\- Player can move through valid navigable areas.

\- Player interacts correctly with world collision.

\- Player does not unintentionally pass through blocking geometry.

\- Player remains controllable during normal gameplay.

\### Enemy AI

Verify:

\- Enemy can spawn correctly.

\- Enemy can move.

\- Enemy can enter expected AI states.

\- Patrol behaviour operates as expected.

\- Chase behaviour operates as expected.

\- Search behaviour operates as expected.

\- Return behaviour operates as expected.

\- Invalid or unexpected state transitions can be detected.

\- Multiple enemies do not cause immediate gameplay instability.

\### Navigation and Pathfinding

Verify:

\- Valid paths can be generated.

\- Unreachable destinations are handled.

\- Obstacles influence navigation correctly.

\- Repeated path calculations remain consistent.

\- Start and goal edge cases are handled.

\- Pathfinding failures can be diagnosed.

\### Fragile Item

Verify:

\- Item can exist/spawn as expected.

\- Item can be collected.

\- Item can be carried.

\- Item can be dropped.

\- Repeated pickup/drop interactions behave correctly.

\- Break conditions behave according to game rules.

\### Delivery

Verify:

\- Delivery area exists.

\- Delivery only occurs under valid conditions.

\- Invalid delivery attempts do not incorrectly complete delivery.

\- Delivery boundaries behave consistently.

\- Successful delivery updates relevant gameplay state.

\### Puzzle

Verify:

\- Pressure button responds to intended interactions.

\- Door responds correctly to puzzle state.

\- Repeated or unusual interaction sequences do not create invalid states.

\### Grappling

Verify:

\- Valid grapple conditions work.

\- Invalid targets are rejected appropriately.

\- Geometry and line-of-sight edge cases are handled.

\### HUD

Verify:

\- Score information is displayed.

\- Delivery information is displayed.

\- Relevant item state is displayed.

\- Enemy state/debug information is displayed where expected.

\- Displayed values remain consistent with gameplay state.

\---

\## 6. Test Types

The project will use:

\- Smoke testing

\- Functional testing

\- Integration testing

\- System testing

\- Regression testing

\- Exploratory testing

\- Negative testing

\- Boundary-value testing

\- State-transition testing

\- Stability testing

\- Performance testing where applicable

\---

\## 7. Initial Smoke Test

ID: SMOKE-001

Purpose:

Confirm that the existing game remains sufficiently operational before quality-engineering changes are introduced.

Checks:

\- Game launches.

\- Player movement works.

\- Enemy is present.

\- Enemy movement occurs.

\- Fragile-item functionality is present.

\- Delivery functionality is present.

\- HUD is visible.

\- No crash occurs during the test session.

Status:

PASS for the recorded baseline session.

Limitations:

The smoke test confirms basic operational behaviour only. It does not prove complete correctness of individual gameplay systems.

\---

\## 8. Exploratory Baseline Session

Session ID:

EXP-BASELINE-001

Charter:

Explore core gameplay before automation is introduced, with emphasis on movement, AI, navigation, item interaction, delivery, collision and unusual player behaviour.

Observed:

\- Player movement operational.

\- Enemy presence operational.

\- Enemy movement observed.

\- Player/enemy interaction observed.

\- Fragile-item carry state observed.

\- Delivery-distance information observed.

\- HUD/debug information operational.

\- No crash observed during the recorded session.

Requires further controlled testing:

\- Complete enemy patrol sequence.

\- Repeated fragile-item pickup/drop.

\- Exact delivery-zone boundary behaviour.

\- Abnormal pressure-button interactions.

\- Grapple geometry edge cases.

\- Reproducible stuck-enemy conditions.

\---

\## 9. Initial Automation Candidates

Priority 1:

\- Pathfinding valid path

\- Pathfinding unreachable target

\- Pathfinding repeated execution

\- AI state transitions

\- Stuck-agent detection

\- Target-reach validation

Priority 2:

\- Fragile-item rules

\- Delivery rules

\- Delivery boundary conditions

\- Score updates

\- Collision-related validation

Priority 3:

\- Puzzle interaction

\- Grapple edge cases

\- Bonus behaviour

\- Multiple-agent scenarios

\- Performance/stress scenarios

\---

\## 10. Entry Criteria

A test activity may begin when:

\- Required build is available.

\- Relevant functionality is implemented.

\- Required environment is operational.

\- Expected behaviour can be determined.

\- Required test data/configuration is available.

\---

\## 11. Exit Criteria

A planned test cycle may finish when:

\- Critical planned tests have executed.

\- Test results are recorded.

\- Critical failures are investigated.

\- Regression tests have executed where required.

\- Known issues are documented.

\- Supporting evidence is available.

\---

\## 12. Pass / Fail Rules

PASS:

Observed result satisfies the defined expected result and sufficient evidence exists.

FAIL:

Observed result conflicts with the defined expected result.

BLOCKED:

Execution cannot be completed because a prerequisite, environment, dependency or other blocking condition prevents validation.

NOT VERIFIED:

Available evidence is insufficient to determine whether the requirement passes or fails.

NOT APPLICABLE:

The test does not apply to the current build/configuration.

\---

\## 13. Defect Severity

\### Critical

Failure prevents meaningful gameplay/testing, causes severe data/system failure or consistently crashes the application.

\### High

Major gameplay functionality is unavailable or progression is significantly affected without a reasonable workaround.

\### Medium

Functionality behaves incorrectly but gameplay/testing can continue using a workaround or the impact is limited.

\### Low

Minor issue with limited gameplay impact, such as small presentation, diagnostic or usability problems.

Severity describes impact.

Priority will be recorded separately because priority describes how urgently an issue should be addressed.

\---

\## 14. Deliverables

Planned quality deliverables include:

\- Test Strategy

\- Test Plan

\- Risk Matrix

\- Requirements Traceability Matrix

\- Manual test cases

\- Exploratory test records

\- Automated C++ tests

\- Runtime validators

\- Scenario definitions

\- Structured test results

\- Automated reports

\- API test collection where applicable

\- Performance-test evidence

\- CI pipeline

\- Defect reports

\- Regression evidence

\- Final quality report

\---

\## 15. Current Status

Baseline established.

Current branch:

game-quality-engineering

Current phase:

Day 1 â€” Baseline, test planning and architecture analysis.

Automated test implementation has not yet started.
