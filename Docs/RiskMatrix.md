\# AI Chase â€” Quality Risk Matrix

\## Risk Scoring

Probability:

1 = Low

2 = Medium

3 = High

Impact:

1 = Low

2 = Medium

3 = High

Risk Score = Probability Ã— Impact

Priority:

\- 7â€“9 = Critical Test Priority

\- 4â€“6 = High Test Priority

\- 2â€“3 = Medium Test Priority

\- 1 = Low Test Priority

| ID | System | Risk | Probability | Impact | Score | Test Priority |

|---|---|---|---:|---:|---:|---|

| RISK-001 | Pathfinding | Enemy cannot generate a valid route | 3 | 3 | 9 | Critical |

| RISK-002 | Enemy AI | Enemy becomes permanently stuck | 3 | 3 | 9 | Critical |

| RISK-003 | Enemy AI | Invalid AI state transition | 2 | 3 | 6 | High |

| RISK-004 | Navigation | Previous path calculation influences later calculations | 2 | 3 | 6 | High |

| RISK-005 | Player | Player becomes unable to move | 2 | 3 | 6 | High |

| RISK-006 | Collision | Player passes through blocking geometry | 2 | 3 | 6 | High |

| RISK-007 | Fragile Item | Item cannot be picked up/carried/dropped correctly | 2 | 3 | 6 | High |

| RISK-008 | Fragile Item | Break condition produces incorrect state | 2 | 2 | 4 | High |

| RISK-009 | Delivery | Valid delivery does not complete | 2 | 3 | 6 | High |

| RISK-010 | Delivery | Invalid/boundary delivery completes incorrectly | 2 | 3 | 6 | High |

| RISK-011 | Score | Score does not match gameplay event | 2 | 2 | 4 | High |

| RISK-012 | Puzzle | Pressure button/door enters invalid state | 2 | 2 | 4 | High |

| RISK-013 | Grapple | Invalid geometry produces incorrect grapple behaviour | 2 | 2 | 4 | High |

| RISK-014 | Multiple Enemies | Multiple agents cause unstable behaviour | 2 | 3 | 6 | High |

| RISK-015 | Stability | Gameplay crashes during normal execution | 1 | 3 | 3 | Medium |

| RISK-016 | HUD | Displayed state differs from gameplay state | 2 | 2 | 4 | High |
