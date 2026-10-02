# Day 3 - Runtime AI Validation and Defect Investigation

## 1. Objective

Day 3 extends the AI Chase quality engineering platform from isolated automated tests into runtime validation.

The primary objective was to detect situations where an enemy AI is expected to move but fails to make meaningful positional progress.

The work followed this validation cycle:

Runtime Observation -> Validator -> Reproduction -> Instrumentation -> Root Cause Analysis -> Production Fix -> Automated Regression -> Runtime Regression

The implementation focused on two reusable components:

- `StuckAgentDetector` - detects sustained lack of positional progress while movement is expected.
- `SteeringRules` - contains testable steering correction logic extracted from runtime AI behaviour.

---

## 2. Runtime Validation Requirement

### RV-AI-001 - Enemy Movement Progress

During normal AI behaviour, an enemy must not be reported as stuck unless:

1. movement was expected,
2. the enemy failed to make meaningful positional progress, and
3. the lack of progress persisted for the configured duration.

Current validation configuration:

- Movement threshold: `0.1`
- Stuck time threshold: `3.0 seconds`

Expected stationary behaviour must not generate a stuck warning.

Examples include:

- intentional patrol waiting,
- movement-disabled conditions,
- frames where no movement command was issued.

Runtime failure indication:

`VALIDATION: ENEMY STUCK E<n>`

where `<n>` identifies the enemy index.

---

## 3. StuckAgentDetector

`StuckAgentDetector` was introduced as a reusable runtime validation component.

The detector tracks:

- the last position at which meaningful progress was observed,
- accumulated stationary time,
- whether movement was expected.

The detector is deliberately separated from enemy gameplay state logic so that validation behaviour can be unit tested independently.

### Detection behaviour

If movement is not expected, the detector resets its progress baseline.

If movement is expected, displacement is measured relative to the last meaningful progress position.

When displacement reaches or exceeds the movement threshold:

- the progress position is updated,
- stationary time is reset.

When displacement remains below the threshold:

- stationary time accumulates.

The enemy is reported as stuck once stationary time reaches the configured threshold.

---

## 4. Validator Defect - VAL-DEF-001

### Title

Cumulative small movements incorrectly reported as stuck.

### Initial behaviour

The first detector implementation compared movement between consecutive frames.

A slowly moving enemy could therefore move less than `0.1` units during every individual frame while still making genuine cumulative progress over time.

Because the comparison position was updated every frame, that cumulative movement was lost.

This could eventually produce:

`VALIDATION: ENEMY STUCK`

even though the enemy was moving.

### Reproduction

A regression test was created in which the agent moves `0.05` units repeatedly while movement remains expected.

Before the fix:

- 27 tests passed.
- 1 test failed.
- The failing test reproduced the validator false positive.

### Fix

The detector was changed to keep the previous meaningful progress position as its anchor.

The anchor is now updated only when cumulative displacement reaches the configured movement threshold.

This allows multiple small movements to accumulate into meaningful progress.

### Regression protection

Test:

`StuckAgentDetectorTests.CumulativeSmallMovementsPreventFalseStuckDetection`

Result:

`PASS`

### Status

`VAL-DEF-001: FIXED`

---

## 5. Runtime Investigation After Validator Fix

After VAL-DEF-001 was corrected, runtime stuck warnings were still observed.

At this point the warnings could not safely be classified as another validator defect.

Additional runtime instrumentation was introduced to distinguish:

- false-positive validation,
- legitimate stationary AI behaviour,
- actual navigation no-progress.

Observed data included:

- enemy index,
- AI state,
- movement expectation,
- navigation path index,
- path size,
- distance to active navigation node,
- current velocity,
- avoidance magnitude,
- steering alignment,
- avoidance contributors.

---

## 6. Production Defect - AI-DEF-001

### Title

Enemy navigation can enter sustained no-progress due to opposing avoidance steering.

### Observed behaviour

Multiple enemies remained on the same navigation waypoint for extended periods despite movement being actively commanded.

During reproduced failures:

- movement was expected,
- physics velocity was being commanded,
- the active waypoint was not reached,
- path progression did not occur,
- meaningful positional progress remained insufficient,
- the runtime validator eventually reported the enemy as stuck.

This demonstrated that the remaining warning represented a genuine production navigation problem rather than the previously fixed validator false positive.

### Status before fix

`AI-DEF-001: REPRODUCED`

---

## 7. Root Cause Analysis

Runtime telemetry showed that avoidance steering could become strongly opposed to the active navigation direction.

During reproduced stalls:

- avoidance magnitude was approximately `3.0`,
- steering alignment repeatedly changed between strongly aligned and strongly opposed to the navigation waypoint,
- enemies could remain unable to converge on the waypoint.

Observed steering alignment values included values close to:

- `+0.9`
- `-0.9`

and, during some observations, approximately:

- `+0.98`
- `-0.99`

This indicated oscillating steering behaviour rather than absence of a movement command.

### Root cause

Inter-agent avoidance could contain a component directly opposing the navigation direction.

That opposing component could prevent reliable convergence on the active navigation waypoint and produce sustained navigation no-progress.

---

## 8. Controlled Investigation - Path Index

The navigation path contract was inspected.

`FindPath` returns a path containing the start node.

An experimental change set `currentPathIndex` to `1` after path generation in order to skip the start node during path consumption.

### Result

The experiment changed path consumption behaviour but did not eliminate the sustained physical movement stall.

The enemy could advance from the initial path index and still enter prolonged no-progress.

### Decision

The experiment was not the production fix.

The path-index change was reverted.

Final production behaviour retains:

`currentPathIndex = 0`

This preserves the existing pathfinding contract.

---

## 9. Controlled Investigation - Pressure Button

Runtime avoidance-contributor diagnostics identified the pressure button as one contributor during reproduced stuck behaviour.

This exposed a semantic concern: the generic avoidance system treated the pressure button as an avoidance object even though the gameplay mechanic expects objects to approach it.

An experimental change excluded the pressure button from avoidance.

### Result

After the exclusion:

- the pressure button disappeared from avoidance-contributor telemetry,
- the sustained navigation defect could still be reproduced,
- nearby enemies remained avoidance contributors.

### Decision

The pressure-button exclusion was not the root-cause fix.

It was reverted.

The final runtime regression therefore uses the original avoidance inputs.

---

## 10. Production Fix - SteeringRules

A small testable steering rule was extracted:

`SteeringRules::RemoveOpposingAvoidance`

The rule removes only the component of the avoidance vector that points backwards relative to the active navigation direction.

Conceptually:

- forward avoidance is preserved,
- lateral avoidance is preserved,
- opposing avoidance is removed.

This avoids disabling obstacle/enemy separation entirely and avoids solving the problem by arbitrarily reducing all avoidance strength.

Runtime integration:

```cpp
Vector3 avoidance =
    CalculateAvoidanceForce(enemy);

avoidance =
    SteeringRules::RemoveOpposingAvoidance(
        avoidance,
        dir
    );
```

The corrected avoidance vector is then combined with navigation steering.

---

## 11. Steering Regression Tests

Five automated tests protect the steering rule:

1. `ForwardAvoidanceIsPreserved`
2. `LateralAvoidanceIsPreserved`
3. `OpposingAvoidanceIsRemoved`
4. `MixedAvoidanceRemovesOnlyOpposingComponent`
5. `CorrectedAvoidanceNeverOpposesNavigation`

All five tests pass.

---

## 12. Stuck Detector Regression Tests

Nine automated tests currently cover `StuckAgentDetector`:

1. `FirstObservationDoesNotReportStuck`
2. `StationaryAgentBelowTimeThresholdIsNotStuck`
3. `StationaryAgentAtTimeThresholdIsStuck`
4. `MovementResetsStationaryTimer`
5. `ExpectedStationaryBehaviourDoesNotReportStuck`
6. `MovementAtExactThresholdResetsTimer`
7. `MovementBelowThresholdAccumulatesStationaryTime`
8. `ResetClearsPreviousStationaryHistory`
9. `CumulativeSmallMovementsPreventFalseStuckDetection`

All nine tests pass.

---

## 13. Automated Regression Result

Final CTest result:

`33/33 tests passed`

`0 tests failed`

Coverage currently includes:

- Pathfinding: 6 tests
- Gameplay rules: 13 tests
- StuckAgentDetector: 9 tests
- SteeringRules: 5 tests

Total:

`33 automated tests`

The production `CSC8503` target also builds successfully.

Known C4244 integer-to-float conversion warnings remain outside the scope of the Day 3 change and do not prevent the build.

---

## 14. Runtime Regression Result

Runtime regression was performed after the steering correction.

The unrelated path-index experiment was reverted.

The pressure-button avoidance exclusion was also reverted before final validation.

Therefore, the final runtime test exercised the steering correction while retaining the original avoidance inputs.

Observed result:

- enemy AI continued navigating,
- enemy positions changed over time,
- normal AI states including Return were observed,
- the previous sustained stuck condition was not reproduced,
- no sustained `VALIDATION: ENEMY STUCK` warning was observed,
- gameplay continued normally to the player-loss condition,
- no crash occurred.

### Final status

`AI-DEF-001: FIX VERIFIED`

---

## 15. Defect Summary

| ID | Type | Description | Status |
|---|---|---|---|
| VAL-DEF-001 | Validation | Per-frame movement comparison caused false stuck detection during cumulative small movement | FIXED |
| AI-DEF-001 | Production AI | Opposing avoidance steering could prevent reliable waypoint convergence | FIX VERIFIED |

The later runtime stuck warnings were therefore not classified as a second validator defect. They became the evidence that exposed AI-DEF-001.

---

## 16. Engineering Outcome

Day 3 demonstrates a complete quality-engineering workflow rather than only adding automated tests.

The validator initially exposed a defect in its own detection logic. A deterministic regression test reproduced that false positive and protected the correction.

Once the validator became reliable, continued runtime warnings triggered production investigation. Temporary telemetry was used to isolate steering behaviour, controlled hypotheses were tested and reverted when they did not solve the defect, and the final correction was extracted into independently testable steering logic.

Final evidence:

`Runtime Detection -> Reproduction -> Instrumentation -> RCA -> Fix -> Automated Regression -> Runtime Regression -> PASS`

This provides reusable runtime validation infrastructure for later CI, batch execution, result collection and automated game-system verification.
