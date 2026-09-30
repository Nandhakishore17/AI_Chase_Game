\# AI Chase â€” Manual Test Cases

\## Test Case Status

\- NOT RUN â€” Test has not yet been executed.

\- PASS â€” Actual result matches expected result.

\- FAIL â€” Actual result differs from expected result.

\- BLOCKED â€” Test cannot be completed because of a blocking condition.

\- NOT VERIFIED â€” Available evidence is insufficient to determine PASS or FAIL.

\---

\# 1. Smoke Testing

\## TC-STAB-001 â€” Verify Game Launch

Requirement: REQ-STAB-001

Priority: Critical

Type: Smoke / Functional

\### Preconditions

\- Project builds successfully.

\- Required game assets are available.

\### Steps

1\. Launch the AI Chase game.

2\. Wait for the game environment to load.

3\. Observe the application.

\### Expected Result

\- Game launches successfully.

\- Gameplay environment loads.

\- Application does not terminate unexpectedly.

\### Actual Result

Game launched successfully during baseline recording.

\### Status

PASS

\---

\## TC-STAB-002 â€” Verify Basic Gameplay Stability

Requirement: REQ-STAB-002

Priority: High

Type: Smoke / Stability

\### Preconditions

\- Game has launched successfully.

\### Steps

1\. Begin gameplay.

2\. Move around the environment.

3\. Interact with available gameplay systems.

4\. Continue gameplay for the baseline test period.

\### Expected Result

Game remains operational without unexpected termination.

\### Actual Result

No crash was observed during the recorded baseline session.

\### Status

PASS

\---

\# 2. Player Testing

\## TC-PLY-001 â€” Verify Player Movement

Requirement: REQ-PLY-001

Priority: Critical

Type: Functional

\### Steps

1\. Start gameplay.

2\. Move the player through the environment.

3\. Change movement direction several times.

\### Expected Result

Player responds to movement input and moves through valid navigable areas.

\### Actual Result

Player movement was observed during baseline gameplay.

\### Status

PASS

\---

\## TC-PLY-002 â€” Verify Player Collision With Blocking Geometry

Requirement: REQ-PLY-002

Priority: High

Type: Functional / Negative / Exploratory

\### Steps

1\. Move toward a wall.

2\. Continue movement input against the wall.

3\. Repeat against corners and obstacles.

4\. Attempt movement at different angles.

\### Expected Result

Player should not unintentionally pass through blocking geometry.

\### Actual Result

Wall/obstacle interaction was observed, but systematic collision coverage has not yet been completed.

\### Status

NOT VERIFIED

\---

\# 3. Enemy AI Testing

\## TC-AI-001 â€” Verify Enemy Presence

Requirement: REQ-AI-001

Priority: Critical

Type: Smoke / Functional

\### Steps

1\. Start gameplay.

2\. Observe the game environment.

\### Expected Result

At least one intended enemy is present.

\### Actual Result

Enemy presence was observed during baseline gameplay.

\### Status

PASS

\---

\## TC-AI-002 â€” Verify Enemy Movement

Requirement: REQ-AI-002

Priority: Critical

Type: Functional

\### Steps

1\. Locate an enemy.

2\. Observe its position.

3\. Continue gameplay.

4\. Compare its position over time.

\### Expected Result

Enemy changes position when its current AI behaviour requires movement.

\### Actual Result

Enemy movement was observed during baseline gameplay.

\### Status

PASS

\---

\## TC-AI-003 â€” Verify Patrol Behaviour

Requirement: REQ-AI-003

Priority: High

Type: Functional / State Transition

\### Steps

1\. Start gameplay.

2\. Keep the player outside intended chase conditions.

3\. Observe enemy behaviour over a controlled period.

4\. Record enemy state and movement.

\### Expected Result

Enemy follows intended patrol behaviour while patrol conditions apply.

\### Actual Result

Enemy movement was observed, but complete controlled patrol behaviour has not yet been verified.

\### Status

NOT VERIFIED

\---

\## TC-AI-004 â€” Verify Chase Behaviour

Requirement: REQ-AI-004

Priority: Critical

Type: Functional / State Transition

\### Steps

1\. Allow enemy to begin in patrol behaviour.

2\. Move player into the intended detection/chase condition.

3\. Observe enemy state.

4\. Move player through the environment.

\### Expected Result

Enemy transitions into chase behaviour and attempts to pursue the player.

\### Actual Result

Player/enemy interaction consistent with chase behaviour was observed, but a controlled transition test is still required.

\### Status

NOT VERIFIED

\---

\## TC-AI-007 â€” Detect Unintentionally Stuck Enemy

Requirement: REQ-AI-007

Risk: RISK-002

Priority: Critical

Type: Negative / Runtime Validation

\### Steps

1\. Cause an enemy to navigate toward a valid target.

2\. Position gameplay around obstacles/corners.

3\. Observe enemy movement over time.

4\. Determine whether movement is expected.

5\. Record position changes.

\### Expected Result

Enemy continues making meaningful progress while movement is expected.

\### Failure Condition

Enemy remains below the defined movement threshold for longer than the allowed timeout while movement is expected.

\### Current Status

NOT VERIFIED

\### Planned Automation

StuckAgentDetector

\---

\# 4. Navigation / Pathfinding

\## TC-NAV-001 â€” Valid Path

Requirement: REQ-NAV-001

Risk: RISK-001

Priority: Critical

Type: Functional / Automated Candidate

\### Preconditions

\- Start node exists.

\- Goal node exists.

\- Valid connection exists between them.

\### Steps

1\. Request a path between start and goal.

2\. Inspect returned path.

\### Expected Result

A valid path from start to goal is returned.

\### Status

NOT RUN

\### Planned Automation

GoogleTest

\---

\## TC-NAV-002 â€” Unreachable Destination

Requirement: REQ-NAV-002

Risk: RISK-001

Priority: Critical

Type: Negative / Automated Candidate

\### Preconditions

Start and goal exist but no valid route connects them.

\### Steps

1\. Request a path.

2\. Inspect the result.

3\. Observe application stability.

\### Expected Result

Pathfinding safely reports that a valid route cannot be produced and does not crash or return an invalid path.

\### Status

NOT RUN

\### Planned Automation

GoogleTest

\---

\## TC-NAV-003 â€” Start Equals Goal

Requirement: REQ-NAV-003

Priority: High

Type: Boundary / Automated Candidate

\### Steps

1\. Use the same navigation node as start and goal.

2\. Request a path.

3\. Inspect the result.

\### Expected Result

The condition is handled safely and consistently without invalid memory/state behaviour.

\### Status

NOT RUN

\### Planned Automation

GoogleTest

\---

\## TC-NAV-004 â€” Repeated Pathfinding Independence

Requirement: REQ-NAV-004

Risk: RISK-004

Priority: High

Type: Regression / State Isolation

\### Steps

1\. Execute a path calculation.

2\. Record the result.

3\. Execute additional path calculations using the same navigation graph.

4\. Repeat the original path request.

5\. Compare results.

\### Expected Result

Previous pathfinding executions must not incorrectly influence later path calculations.

\### Status

NOT RUN

\### Planned Automation

GoogleTest

\### Investigation Note

This test specifically investigates whether mutable navigation-node pathfinding state can affect subsequent calculations. No defect is assumed until test evidence confirms incorrect behaviour.

\---

\# 5. Fragile Item

\## TC-ITEM-003 â€” Carry Fragile Item

Requirement: REQ-ITEM-003

Priority: High

Type: Functional

\### Steps

1\. Acquire the fragile item.

2\. Move while carrying it.

3\. Observe gameplay/HUD state.

\### Expected Result

Item remains in the intended carried state and the game reflects that state correctly.

\### Actual Result

The baseline recording displayed the carrying-item state.

\### Status

PASS

\---

\## TC-ITEM-005 â€” Repeated Pickup / Drop

Requirement: REQ-ITEM-005

Risk: RISK-007

Priority: High

Type: Exploratory / Regression

\### Steps

1\. Pick up the item.

2\. Drop the item.

3\. Pick it up again.

4\. Repeat the cycle several times.

5\. Observe item state after each operation.

\### Expected Result

Item remains in a valid state and can be repeatedly interacted with according to gameplay rules.

\### Status

NOT RUN

\---

\## TC-ITEM-006 â€” Fragile Item Break Condition

Requirement: REQ-ITEM-006

Risk: RISK-008

Priority: High

Type: Boundary / Functional

\### Steps

1\. Acquire the fragile item.

2\. Create conditions around the defined break threshold.

3\. Test below the threshold.

4\. Test at/near the threshold.

5\. Test above the threshold.

6\. Observe item state.

\### Expected Result

Break behaviour occurs only according to the implemented game rule.

\### Status

NOT RUN

\---

\# 6. Delivery

\## TC-DEL-002 â€” Valid Delivery

Requirement: REQ-DEL-002

Risk: RISK-009

Priority: Critical

Type: Functional

\### Steps

1\. Acquire the required delivery item.

2\. Carry it to the delivery area.

3\. Enter the valid delivery condition.

4\. Observe delivery state.

\### Expected Result

Delivery completes successfully and relevant gameplay state is updated.

\### Actual Result

Recorded runtime regression successfully delivered the fragile item to the delivery area. Delivery count changed to 1/1 and the game entered the expected Delivery Complete win state.
### Status

PASS

\---

\## TC-DEL-003 â€” Invalid Delivery

Requirement: REQ-DEL-003

Risk: RISK-010

Priority: Critical

Type: Negative

\### Steps

1\. Approach the delivery area without satisfying valid delivery conditions.

2\. Attempt delivery.

\### Expected Result

Delivery must not incorrectly complete.

\### Status

NOT RUN

\---

\## TC-DEL-004 â€” Delivery Boundary

Requirement: REQ-DEL-004

Risk: RISK-010

Priority: High

Type: Boundary Value Analysis

\### Test Conditions

A. Clearly outside delivery threshold

B. Immediately outside threshold

C. At/near threshold

D. Immediately inside threshold

E. Clearly inside delivery threshold

\### Expected Result

Delivery behaviour changes consistently according to the implemented threshold rule.

\### Status

NOT RUN

\---

\## TC-DEL-006 â€” Delivery Score Update

Requirement: REQ-DEL-006

Risk: RISK-011

Priority: High

Type: Integration

\### Steps

1\. Record current score.

2\. Perform valid delivery.

3\. Record new score.

\### Expected Result

Score changes exactly according to the implemented delivery rule.

\### Actual Result

Recorded runtime regression showed the score changing from 0 to 10 following successful delivery, matching the implemented delivery scoring rule.
### Status

PASS

\---

\# 7. Puzzle

\## TC-PUZ-003 â€” Abnormal Pressure Button Interaction

Requirement: REQ-PUZ-003

Risk: RISK-012

Priority: Medium

Type: Negative / Exploratory

\### Steps

1\. Activate pressure button.

2\. Leave it.

3\. Re-enter rapidly.

4\. Repeat interaction.

5\. Attempt interaction from unusual positions.

6\. Observe door state.

\### Expected Result

Button and door remain in valid and consistent gameplay states.

\### Status

NOT RUN

\---

\# 8. Grappling

\## TC-GRP-002 â€” Invalid Grapple Target

Requirement: REQ-GRP-002

Risk: RISK-013

Priority: Medium

Type: Negative

\### Steps

1\. Aim grapple toward invalid/non-grapple geometry.

2\. Activate grapple.

3\. Repeat against several invalid targets.

\### Expected Result

Invalid grapple attempts are handled safely without producing invalid player/game state.

\### Status

NOT RUN

\---

\## TC-GRP-003 â€” Grapple Geometry Edge Cases

Requirement: REQ-GRP-003

Risk: RISK-013

Priority: Medium

Type: Exploratory

\### Steps

1\. Test grapple near corners.

2\. Test around obstacle edges.

3\. Test unusual angles.

4\. Test near maximum practical range.

5\. Observe player and grapple behaviour.

\### Expected Result

Grappling remains stable and does not produce unintended movement or invalid state.

\### Status

NOT RUN

\---

\# 9. Multiple Enemy Testing

\## TC-MULTI-001 â€” Multiple Enemy Stability

Requirement: REQ-MULTI-001

Risk: RISK-014

Priority: High

Type: Integration / Stability

\### Steps

1\. Run a gameplay condition involving multiple enemies.

2\. Allow multiple enemies to move toward/interact with the player.

3\. Continue gameplay.

4\. Observe stability and behaviour.

\### Expected Result

Game remains operational and enemies continue functioning without immediate instability.

\### Status

NOT VERIFIED

\---

\# 10. HUD / Diagnostics

\## TC-HUD-005 â€” HUD State Consistency

Requirement: REQ-HUD-005

Risk: RISK-016

Priority: High

Type: Integration

\### Steps

1\. Observe HUD before gameplay-state change.

2\. Trigger item, delivery or enemy-state change.

3\. Compare internal gameplay event with displayed HUD information.

\### Expected Result

HUD information accurately reflects the corresponding gameplay state.

\### Status

NOT RUN

\---

\# Test Execution Summary

| Category | Total Selected | Passed | Failed | Not Run / Not Verified |

|---|---:|---:|---:|---:|

| Stability | 2 | 2 | 0 | 0 |

| Player | 2 | 1 | 0 | 1 |

| Enemy AI | 5 | 2 | 0 | 3 |

| Navigation | 4 | 0 | 0 | 4 |

| Fragile Item | 3 | 1 | 0 | 2 |

| Delivery | 4 | 0 | 0 | 4 |

| Puzzle | 1 | 0 | 0 | 1 |

| Grapple | 2 | 0 | 0 | 2 |

| Multiple Enemy | 1 | 0 | 0 | 1 |

| HUD | 1 | 0 | 0 | 1 |

| \*\*TOTAL\*\* | \*\*25\*\* | \*\*6\*\* | \*\*0\*\* | \*\*19\*\* |

This summary represents the current baseline state and will change as controlled manual and automated execution progresses.
