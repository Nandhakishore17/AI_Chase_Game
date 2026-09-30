\# AI Chase â€” Exploratory Testing

\## Session EXP-BASELINE-001

\### Session Objective

Establish the behavioural baseline of the existing AI Chase Delivery Game before introducing automated Game Quality Engineering changes.

\### Test Approach

Session-based exploratory testing.

The tester interacted with the existing game while observing:

\- Player movement

\- Collision behaviour

\- Enemy presence and movement

\- Enemy/player interaction

\- Navigation around level geometry

\- Fragile-item state

\- Delivery information

\- Multiple-enemy behaviour

\- HUD/debug information

\- General application stability

\---

\## Test Environment

Branch:

game-quality-engineering

Platform:

Windows

Build:

Existing working project baseline before quality-engineering implementation.

Evidence:

Gameplay screen recording captured during the baseline session.

\---

\## Observations

\### OBS-001 â€” Game Startup

The game launched successfully and entered gameplay.

Result:

PASS

\---

\### OBS-002 â€” Player Movement

Player movement was observed throughout the recorded session.

Result:

PASS

\---

\### OBS-003 â€” Collision / Geometry Interaction

The player interacted with walls, corners and level geometry.

No obvious wall penetration was identified during the observed session.

However, controlled collision testing at multiple angles and boundary positions has not yet been completed.

Result:

PARTIAL / REQUIRES CONTROLLED TESTING

\---

\### OBS-004 â€” Enemy Presence

At least one enemy was visible during gameplay.

Result:

PASS

\---

\### OBS-005 â€” Enemy Movement

Enemy movement was observed during gameplay.

Result:

PASS

\---

\### OBS-006 â€” Enemy Patrol

Enemy movement/state behaviour was visible, but the complete expected patrol sequence was not established through a controlled test.

Result:

NOT VERIFIED

\---

\### OBS-007 â€” Enemy Chase / Player Interaction

Enemy/player interaction consistent with chase behaviour was observed.

A controlled state-transition test is still required to verify the exact trigger and expected transition.

Result:

PARTIAL

\---

\### OBS-008 â€” Navigation Around Obstacles

Gameplay included enemies and player movement around level obstacles.

This provides exploratory evidence that navigation is operating.

It does not prove that all generated paths are valid or optimal.

Result:

PARTIAL

\---

\### OBS-009 â€” Fragile Item

The HUD displayed:

Carrying Item (G to drop)

This confirms that the carried-item state was reached during the baseline session.

Repeated pickup/drop behaviour was not sufficiently validated.

Result:

PARTIAL

\---

\### OBS-010 â€” Delivery System

Delivery-distance information was visible in the HUD.

The delivery system therefore appears active in the baseline build.

Exact delivery threshold and boundary behaviour were not validated.

Result:

PARTIAL

\---

\### OBS-011 â€” Pressure Button / Door

Abnormal or repeated pressure-button interaction was not sufficiently demonstrated during the baseline session.

Result:

NOT VERIFIED

\---

\### OBS-012 â€” Grappling Edge Cases

Systematic grappling tests against unusual geometry were not completed.

Result:

NOT VERIFIED

\---

\### OBS-013 â€” Multiple Enemies

Multiple enemy/gameplay objects were observed during portions of the session.

A controlled multiple-agent stress scenario is still required.

Result:

PARTIAL

\---

\### OBS-014 â€” Potential Stuck Enemy Conditions

Gameplay around obstacles created conditions where enemy movement could potentially become restricted.

Visual observation alone is insufficient to classify an enemy as stuck.

A measurable runtime validator is required.

Planned validation:

StuckAgentDetector

Result:

NOT VERIFIED

\---

\### OBS-015 â€” HUD / Diagnostics

The gameplay HUD displayed diagnostic information including values such as:

\- Score

\- Delivered count

\- Item/delivery distance

\- Enemy state

\- Carrying-item state

The HUD will later be used as supporting evidence when validating gameplay-state changes.

Result:

PASS

\---

\### OBS-016 â€” Stability

No application crash was observed during the recorded baseline gameplay session.

This result applies only to the observed session and does not establish long-duration stability.

Result:

PASS

\---

\## Exploratory Risks Identified

The session highlighted the following areas for deeper validation:

1\. Enemy stuck detection.

2\. AI state-transition correctness.

3\. Navigation/pathfinding repeatability.

4\. Collision edge cases.

5\. Repeated fragile-item interaction.

6\. Delivery boundary conditions.

7\. Pressure-button state consistency.

8\. Grapple geometry edge cases.

9\. Multiple-enemy behaviour.

10\. HUD/game-state consistency.

\---

\## Follow-Up Actions

The following testing will be introduced:

\- Automated C++ pathfinding tests.

\- AI runtime validation.

\- Stuck-agent detection.

\- State-transition validation.

\- Delivery boundary testing.

\- Fragile-item regression testing.

\- Structured scenario execution.

\- Automated result reporting.

\- CI regression execution.

\---

\## Session Conclusion

The existing game is sufficiently operational to proceed with automated quality-engineering work.

The exploratory session established a baseline but deliberately does not classify behaviours as verified where sufficient controlled evidence does not yet exist.
