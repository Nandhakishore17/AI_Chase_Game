# AI Chase - Requirements Traceability Matrix

## Purpose

This Requirements Traceability Matrix (RTM) maps gameplay requirements to identified quality risks and test coverage.

The matrix will evolve as manual tests, automated C++ tests, runtime validators and regression scenarios are implemented.

Status values:

- BASELINE PASS - behaviour observed during baseline testing

- PARTIAL - some evidence exists but controlled validation is required

- NOT VERIFIED - insufficient evidence currently exists

- PLANNED - test has not yet been implemented/executed

- AUTOMATED - automated coverage has been implemented

- PASS - requirement has passed its defined test

- FAIL - requirement has failed its defined test

---

## Player

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-PLY-001 | Player shall be able to move during gameplay | RISK-005 | TC-PLY-001 | Runtime candidate | BASELINE PASS |

| REQ-PLY-002 | Player shall be blocked by intended world geometry | RISK-006 | TC-PLY-002 | Runtime candidate | PARTIAL |

| REQ-PLY-003 | Player shall remain controllable during normal gameplay | RISK-005 | TC-PLY-003 | Runtime candidate | BASELINE PASS |

---

## Enemy AI

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-AI-001 | Enemy shall spawn successfully | RISK-002 | TC-AI-001 | Runtime candidate | BASELINE PASS |

| REQ-AI-002 | Enemy shall move when movement is expected | RISK-002 | TC-AI-002 | Runtime validator | BASELINE PASS |

| REQ-AI-003 | Enemy shall perform patrol behaviour | RISK-003 | TC-AI-003 | Runtime validator | PARTIAL |

| REQ-AI-004 | Enemy shall transition to chase behaviour under defined conditions | RISK-003 | TC-AI-004 | Runtime validator | PARTIAL |

| REQ-AI-005 | Enemy shall perform search behaviour under defined conditions | RISK-003 | TC-AI-005 | Runtime validator | NOT VERIFIED |

| REQ-AI-006 | Enemy shall return appropriately after search/chase behaviour | RISK-003 | TC-AI-006 | Runtime validator | PARTIAL |

| REQ-AI-007 | Enemy shall not remain unintentionally stuck while movement is expected | RISK-002 | TC-AI-007 | StuckAgentDetector | NOT VERIFIED |

| REQ-AI-008 | AI shall not enter an invalid state transition | RISK-003 | TC-AI-008 | State validator | NOT VERIFIED |

---

## Navigation / Pathfinding

| Requirement ID | Requirement | Risk | Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-NAV-001 | Pathfinding shall return a valid path between reachable nodes | RISK-001 | TC-NAV-001 | GoogleTest / CTest | PASS |

| REQ-NAV-002 | Pathfinding shall handle unreachable destinations without invalid behaviour | RISK-001 | TC-NAV-002 | GoogleTest / CTest | PASS |

| REQ-NAV-003 | Pathfinding shall handle start equal to goal safely | RISK-001 | TC-NAV-003 | GoogleTest / CTest | PASS |

| REQ-NAV-004 | Repeated path calculations shall remain consistent and independent | RISK-004 | TC-NAV-004 | GoogleTest / CTest | PASS |

| REQ-NAV-005 | Navigation shall account for blocking geometry | RISK-001 | TC-NAV-005 | Integration/runtime | PARTIAL |

| REQ-NAV-006 | Pathfinding failure shall be detectable and diagnosable | RISK-001 | TC-NAV-006 | GoogleTest/runtime | PLANNED |

### Additional Automated Navigation Coverage

| Automated Test ID | Test | Related Requirement | Risk | Framework | Result |

|---|---|---|---|---|---|

| AT-NAV-001 | Direct connected-node path | REQ-NAV-001 | RISK-001 | GoogleTest / CTest | PASS |

| AT-NAV-002 | Unreachable goal returns empty path | REQ-NAV-002 | RISK-001 | GoogleTest / CTest | PASS |

| AT-NAV-003 | Start equal to goal returns a single-node path | REQ-NAV-003 | RISK-001 | GoogleTest / CTest | PASS |

| AT-NAV-004 | Repeated execution produces an independent valid path | REQ-NAV-004 | RISK-004 | GoogleTest / CTest | PASS |

| AT-NAV-005 | Lower-cost route selected when multiple paths exist | REQ-NAV-001 | RISK-001 | GoogleTest / CTest | PASS |

| AT-NAV-006 | Repeated search recalculates the optimal route across shared nodes | REQ-NAV-004 | RISK-004 | GoogleTest / CTest | PASS |

Navigation automated execution summary: \*\*6 executed, 6 passed, 0 failed.\*\*

The repeated-search scenarios did not reproduce path corruption from persistent node search state. This result applies to the tested graph scenarios and does not eliminate RISK-004 from future regression consideration.

---

## Fragile Item

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |
|---|---|---|---|---|---|
| REQ-ITEM-001 | Fragile item shall be available during the intended gameplay sequence | RISK-007 | TC-ITEM-001 | Integration/runtime | PASS |
| REQ-ITEM-002 | Player shall be able to pick up the fragile item | RISK-007 | TC-ITEM-002 | Unit + integration/runtime | PASS |
| REQ-ITEM-003 | Player shall be able to carry the fragile item | RISK-007 | TC-ITEM-003 | Integration/runtime | PASS |
| REQ-ITEM-004 | Player shall be able to drop the fragile item | RISK-007 | TC-ITEM-004 | Integration/runtime | PASS |
| REQ-ITEM-005 | Repeated pickup/drop interactions shall maintain valid item state | RISK-007 | TC-ITEM-005 | Scenario automation/runtime | PARTIAL |
| REQ-ITEM-006 | Fragile-item break behaviour shall follow defined game rules | RISK-008 | TC-ITEM-006 | GoogleTest + runtime integration | PARTIAL |

### Automated Fragile-Item Coverage

| Automated Test ID | Test | Related Requirement | Risk | Framework | Result |
|---|---|---|---|---|---|
| AT-ITEM-001 | Pickup allowed inside pickup distance | REQ-ITEM-002 | RISK-007 | GoogleTest / CTest | PASS |
| AT-ITEM-002 | Pickup rejected at exact 12.0 boundary | REQ-ITEM-002 | RISK-007 | GoogleTest / CTest | PASS |
| AT-ITEM-003 | Pickup rejected when item is already carried | REQ-ITEM-005 | RISK-007 | GoogleTest / CTest | PASS |
| AT-ITEM-004 | Pickup rejected when item is broken | REQ-ITEM-006 | RISK-008 | GoogleTest / CTest | PASS |
| AT-ITEM-005 | Item does not break below 30.0 impact threshold | REQ-ITEM-006 | RISK-008 | GoogleTest / CTest | PASS |
| AT-ITEM-006 | Item does not break at exact 30.0 impact threshold | REQ-ITEM-006 | RISK-008 | GoogleTest / CTest | PASS |
| AT-ITEM-007 | Item breaks above 30.0 impact threshold | REQ-ITEM-006 | RISK-008 | GoogleTest / CTest | PASS |

Recorded runtime regression verified pickup, carry, drop and subsequent pickup behaviour. Repeated pickup/drop cycling remains only partially verified because the documented scenario requires several cycles. Fragile-item break decision boundaries are automated, but the physics-to-break runtime integration was not demonstrated in the recorded regression.

---

## Delivery

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |
|---|---|---|---|---|---|
| REQ-DEL-001 | Delivery area shall be available during gameplay | RISK-009 | TC-DEL-001 | Integration/runtime | PASS |
| REQ-DEL-002 | Valid delivery conditions shall complete delivery | RISK-009 | TC-DEL-002 | GoogleTest + integration/runtime | PASS |
| REQ-DEL-003 | Invalid delivery conditions shall not complete delivery | RISK-010 | TC-DEL-003 | GoogleTest + integration/runtime | PARTIAL |
| REQ-DEL-004 | Delivery boundary behaviour shall be consistent | RISK-010 | TC-DEL-004 | GoogleTest / CTest | PASS |
| REQ-DEL-005 | Successful delivery shall update the relevant gameplay state | RISK-009 | TC-DEL-005 | Integration/runtime | PASS |
| REQ-DEL-006 | Successful delivery shall update score according to game rules | RISK-011 | TC-DEL-006 | Integration/runtime | PASS |

### Automated Delivery Coverage

| Automated Test ID | Test | Related Requirement | Risk | Framework | Result |
|---|---|---|---|---|---|
| AT-DEL-001 | Delivery allowed inside 8.0 delivery distance | REQ-DEL-002 | RISK-009 | GoogleTest / CTest | PASS |
| AT-DEL-002 | Delivery rejected at exact 8.0 boundary | REQ-DEL-003, REQ-DEL-004 | RISK-010 | GoogleTest / CTest | PASS |
| AT-DEL-003 | Delivery rejected when item is broken | REQ-DEL-003 | RISK-010 | GoogleTest / CTest | PASS |
| AT-DEL-004 | Delivery incomplete below required item count | REQ-DEL-005 | RISK-009 | GoogleTest / CTest | PASS |
| AT-DEL-005 | Delivery complete at required item count | REQ-DEL-005 | RISK-009 | GoogleTest / CTest | PASS |
| AT-DEL-006 | Delivery remains complete above required item count | REQ-DEL-005 | RISK-009 | GoogleTest / CTest | PASS |

Recorded runtime regression verified successful delivery, Delivered 1/1, score change from 0 to 10, the Delivery Complete end state, and no crash or freeze during the tested sequence. Invalid delivery has automated rule coverage but has not been comprehensively exercised as a runtime scenario.
## Puzzle

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-PUZ-001 | Pressure button shall detect intended interaction | RISK-012 | TC-PUZ-001 | Integration candidate | NOT VERIFIED |

| REQ-PUZ-002 | Door shall respond correctly to pressure-button state | RISK-012 | TC-PUZ-002 | Integration candidate | NOT VERIFIED |

| REQ-PUZ-003 | Repeated or abnormal interaction shall not create invalid puzzle state | RISK-012 | TC-PUZ-003 | Scenario candidate | NOT VERIFIED |

---

## Grappling

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-GRP-001 | Grapple shall work against valid targets | RISK-013 | TC-GRP-001 | Runtime candidate | NOT VERIFIED |

| REQ-GRP-002 | Invalid grapple targets shall not produce invalid behaviour | RISK-013 | TC-GRP-002 | Runtime candidate | NOT VERIFIED |

| REQ-GRP-003 | Grapple behaviour shall remain stable around unusual geometry | RISK-013 | TC-GRP-003 | Exploratory/runtime | NOT VERIFIED |

---

## Multiple Enemy Behaviour

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-MULTI-001 | Multiple enemies shall operate without immediate game instability | RISK-014 | TC-MULTI-001 | Runtime/stress | PARTIAL |

| REQ-MULTI-002 | Multiple enemies shall maintain valid individual AI behaviour | RISK-014 | TC-MULTI-002 | Runtime validator | NOT VERIFIED |

---

## HUD / Diagnostics

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-HUD-001 | Score shall be displayed | RISK-016 | TC-HUD-001 | Integration candidate | BASELINE PASS |

| REQ-HUD-002 | Delivery status shall be displayed | RISK-016 | TC-HUD-002 | Integration candidate | BASELINE PASS |

| REQ-HUD-003 | Item status shall reflect relevant gameplay state | RISK-016 | TC-HUD-003 | Integration candidate | PARTIAL |

| REQ-HUD-004 | Enemy state diagnostic shall be displayed where enabled | RISK-016 | TC-HUD-004 | Integration candidate | BASELINE PASS |

| REQ-HUD-005 | Displayed diagnostic values shall correspond to actual game state | RISK-016 | TC-HUD-005 | Integration/runtime | NOT VERIFIED |

---

## Stability

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-STAB-001 | Game shall launch successfully | RISK-015 | TC-STAB-001 | CI smoke candidate | BASELINE PASS |

| REQ-STAB-002 | Game shall remain operational during normal gameplay | RISK-015 | TC-STAB-002 | Runtime/stability | BASELINE PASS |

| REQ-STAB-003 | Automated validation failures shall not terminate without diagnostic evidence | RISK-015 | TC-STAB-003 | Validation framework | PLANNED |

---

## Traceability Model

The intended traceability chain is:

Requirement

-> Risk

-> Test Case

-> Automated Test / Runtime Validator

-> Execution Result

-> Defect (if applicable)

-> Fix

-> Regression Result

Example:

REQ-NAV-004

-> RISK-004

-> TC-NAV-004

-> AT-NAV-004 / AT-NAV-006

-> PASS

-> No defect reproduced in tested scenarios

-> Continue regression coverage
