\# AI Chase â€” Requirements Traceability Matrix

\## Purpose

This Requirements Traceability Matrix (RTM) maps gameplay requirements to identified quality risks and planned test coverage.

The matrix will evolve as manual tests, automated C++ tests, runtime validators and regression scenarios are implemented.

Status values:

\- BASELINE PASS â€” behaviour observed during baseline testing

\- PARTIAL â€” some evidence exists but controlled validation is required

\- NOT VERIFIED â€” insufficient evidence currently exists

\- PLANNED â€” test has not yet been implemented/executed

\- AUTOMATED â€” automated coverage has been implemented

\- PASS â€” requirement has passed its defined test

\- FAIL â€” requirement has failed its defined test

\---

\## Player

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-PLY-001 | Player shall be able to move during gameplay | RISK-005 | TC-PLY-001 | Runtime candidate | BASELINE PASS |

| REQ-PLY-002 | Player shall be blocked by intended world geometry | RISK-006 | TC-PLY-002 | Runtime candidate | PARTIAL |

| REQ-PLY-003 | Player shall remain controllable during normal gameplay | RISK-005 | TC-PLY-003 | Runtime candidate | BASELINE PASS |

\---

\## Enemy AI

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

\---

\## Navigation / Pathfinding

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-NAV-001 | Pathfinding shall return a valid path between reachable nodes | RISK-001 | TC-NAV-001 | GoogleTest | PLANNED |

| REQ-NAV-002 | Pathfinding shall handle unreachable destinations without invalid behaviour | RISK-001 | TC-NAV-002 | GoogleTest | PLANNED |

| REQ-NAV-003 | Pathfinding shall handle start equal to goal safely | RISK-001 | TC-NAV-003 | GoogleTest | PLANNED |

| REQ-NAV-004 | Repeated path calculations shall remain consistent and independent | RISK-004 | TC-NAV-004 | GoogleTest | PLANNED |

| REQ-NAV-005 | Navigation shall account for blocking geometry | RISK-001 | TC-NAV-005 | Integration/runtime | PARTIAL |

| REQ-NAV-006 | Pathfinding failure shall be detectable and diagnosable | RISK-001 | TC-NAV-006 | GoogleTest/runtime | PLANNED |

\---

\## Fragile Item

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-ITEM-001 | Fragile item shall be available during the intended gameplay sequence | RISK-007 | TC-ITEM-001 | Integration candidate | BASELINE PASS |

| REQ-ITEM-002 | Player shall be able to pick up the fragile item | RISK-007 | TC-ITEM-002 | Integration candidate | PARTIAL |

| REQ-ITEM-003 | Player shall be able to carry the fragile item | RISK-007 | TC-ITEM-003 | Runtime candidate | BASELINE PASS |

| REQ-ITEM-004 | Player shall be able to drop the fragile item | RISK-007 | TC-ITEM-004 | Integration candidate | NOT VERIFIED |

| REQ-ITEM-005 | Repeated pickup/drop interactions shall maintain valid item state | RISK-007 | TC-ITEM-005 | Scenario automation | NOT VERIFIED |

| REQ-ITEM-006 | Fragile-item break behaviour shall follow defined game rules | RISK-008 | TC-ITEM-006 | Unit/integration | NOT VERIFIED |

\---

\## Delivery

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-DEL-001 | Delivery area shall be available during gameplay | RISK-009 | TC-DEL-001 | Runtime candidate | BASELINE PASS |

| REQ-DEL-002 | Valid delivery conditions shall complete delivery | RISK-009 | TC-DEL-002 | Integration/runtime | NOT VERIFIED |

| REQ-DEL-003 | Invalid delivery conditions shall not complete delivery | RISK-010 | TC-DEL-003 | Integration/runtime | NOT VERIFIED |

| REQ-DEL-004 | Delivery boundary behaviour shall be consistent | RISK-010 | TC-DEL-004 | Boundary automation | NOT VERIFIED |

| REQ-DEL-005 | Successful delivery shall update the relevant gameplay state | RISK-009 | TC-DEL-005 | Integration | NOT VERIFIED |

| REQ-DEL-006 | Successful delivery shall update score according to game rules | RISK-011 | TC-DEL-006 | Unit/integration | NOT VERIFIED |

\---

\## Puzzle

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-PUZ-001 | Pressure button shall detect intended interaction | RISK-012 | TC-PUZ-001 | Integration candidate | NOT VERIFIED |

| REQ-PUZ-002 | Door shall respond correctly to pressure-button state | RISK-012 | TC-PUZ-002 | Integration candidate | NOT VERIFIED |

| REQ-PUZ-003 | Repeated or abnormal interaction shall not create invalid puzzle state | RISK-012 | TC-PUZ-003 | Scenario candidate | NOT VERIFIED |

\---

\## Grappling

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-GRP-001 | Grapple shall work against valid targets | RISK-013 | TC-GRP-001 | Runtime candidate | NOT VERIFIED |

| REQ-GRP-002 | Invalid grapple targets shall not produce invalid behaviour | RISK-013 | TC-GRP-002 | Runtime candidate | NOT VERIFIED |

| REQ-GRP-003 | Grapple behaviour shall remain stable around unusual geometry | RISK-013 | TC-GRP-003 | Exploratory/runtime | NOT VERIFIED |

\---

\## Multiple Enemy Behaviour

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-MULTI-001 | Multiple enemies shall operate without immediate game instability | RISK-014 | TC-MULTI-001 | Runtime/stress | PARTIAL |

| REQ-MULTI-002 | Multiple enemies shall maintain valid individual AI behaviour | RISK-014 | TC-MULTI-002 | Runtime validator | NOT VERIFIED |

\---

\## HUD / Diagnostics

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-HUD-001 | Score shall be displayed | RISK-016 | TC-HUD-001 | Integration candidate | BASELINE PASS |

| REQ-HUD-002 | Delivery status shall be displayed | RISK-016 | TC-HUD-002 | Integration candidate | BASELINE PASS |

| REQ-HUD-003 | Item status shall reflect relevant gameplay state | RISK-016 | TC-HUD-003 | Integration candidate | PARTIAL |

| REQ-HUD-004 | Enemy state diagnostic shall be displayed where enabled | RISK-016 | TC-HUD-004 | Integration candidate | BASELINE PASS |

| REQ-HUD-005 | Displayed diagnostic values shall correspond to actual game state | RISK-016 | TC-HUD-005 | Integration/runtime | NOT VERIFIED |

\---

\## Stability

| Requirement ID | Requirement | Risk | Planned Test ID | Automation | Current Status |

|---|---|---|---|---|---|

| REQ-STAB-001 | Game shall launch successfully | RISK-015 | TC-STAB-001 | CI smoke candidate | BASELINE PASS |

| REQ-STAB-002 | Game shall remain operational during normal gameplay | RISK-015 | TC-STAB-002 | Runtime/stability | BASELINE PASS |

| REQ-STAB-003 | Automated validation failures shall not terminate without diagnostic evidence | RISK-015 | TC-STAB-003 | Validation framework | PLANNED |

\---

\## Traceability Model

The intended traceability chain is:

Requirement

â†’ Risk

â†’ Test Case

â†’ Automated Test / Runtime Validator

â†’ Execution Result

â†’ Defect (if applicable)

â†’ Fix

â†’ Regression Result

Example:

REQ-NAV-004

â†’ RISK-004

â†’ TC-NAV-004

â†’ Automated repeated-pathfinding test

â†’ PASS or FAIL

â†’ DEFECT-NAV-001 if failure is confirmed

â†’ Code fix

â†’ Regression execution
