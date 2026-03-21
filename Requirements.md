# 1. Component Overview

**Component Name:** Traction Inverter Control (TIC)
**Purpose:** Control torque delivery to the electric motor via inverter switching (PWM), ensuring efficiency, safety, and drivability.

---

# 2. Functional Requirements (Complex)

### FR-01: Torque Request Processing

* The component shall accept torque requests from:

  * Vehicle Control Unit (VCU)
  * Brake blending system (regen)
* The component shall:

  * Prioritize safety torque limits over driver demand
  * Apply rate limiting:

    ```
    ΔTorque/Δt ≤ 500 Nm/s
    ```
* If conflicting requests exist:

  * Use arbitration:

    ```
    FinalTorque = min(DriverTorque, SafetyTorqueLimit, ThermalLimit)
    ```

---

### FR-02: Field-Oriented Control (FOC)

* The component shall implement **FOC** using Clarke and Park transformations.
* Control loops:

  * Inner loop: Current control (Id, Iq)
  * Outer loop: Speed / torque control
* Sampling frequency:

  * Current loop ≥ 10 kHz
* The system shall maintain:

  * Torque error ≤ ±2%

---

### FR-03: PWM Signal Generation

* The component shall generate **Space Vector PWM (SVPWM)** signals.
* Switching frequency:

  * Configurable between 8 kHz – 20 kHz
* Dead-time insertion:

  * 1–3 µs configurable
* Ensure:

  * No shoot-through condition under any circumstances

---

### FR-04: Regenerative Braking Control

* When brake request is active:

  * Convert braking demand into negative torque
* Limit regen torque based on:

  * Battery SOC
  * Battery temperature
* If battery cannot accept charge:

  * Gradually reduce regen torque to zero

---

### FR-05: Thermal Derating

* The component shall monitor:

  * Inverter temperature
  * Motor winding temperature
* Apply torque derating:

  ```
  Torque_limit = f(T_temperature)
  ```
* If temperature exceeds critical threshold:

  * Trigger safe shutdown within 100 ms

---

# 3. Safety Requirements (ISO 26262 Aligned)

### SR-01: Overcurrent Protection

* Detect overcurrent within:

  * ≤ 10 µs
* Action:

  * Immediate PWM shutdown
  * Transition to safe state

---

### SR-02: Sensor Plausibility Check

* Validate:

  * Dual current sensors
  * Rotor position sensors
* If deviation:

  ```
  |Sensor1 - Sensor2| > Threshold → Fault
  ```
* Enter limp-home mode

---

### SR-03: Safe State Handling

* Safe state definition:

  * Torque = 0 Nm
  * PWM disabled
* Transition time:

  * ≤ 50 ms after critical fault

---

# 4. Diagnostic Requirements

### DR-01: Fault Classification

* Faults shall be categorized:

  * Critical (shutdown required)
  * Non-critical (derating allowed)

---

### DR-02: DTC Logging

* Log faults using:

  * UDS protocol (ISO 14229)
* Store:

  * Timestamp
  * Operating conditions
  * Fault code

---

### DR-03: Communication Monitoring

* Monitor CAN messages:

  * Timeout detection: ≤ 100 ms
* If timeout:

  * Apply fallback torque = 0

---

# 5. Timing & Real-Time Constraints

### TR-01: Task Scheduling

* Fast control loop:

  * 100 µs (10 kHz)
* Medium loop:

  * 1 ms
* Slow loop:

  * 10 ms

---

### TR-02: Execution Time Constraint

* Worst-case execution time (WCET):

  * ≤ 70% of task period

---

# 6. Interface Requirements

### IR-01: Inputs

* Torque request (Nm)
* Motor speed (RPM)
* Phase currents (A)
* DC bus voltage (V)

### IR-02: Outputs

* PWM duty cycles
* Diagnostic status
* Torque feedback

---

# 7. State Machine Requirements

States:

* INIT
* READY
* RUN
* FAULT
* SHUTDOWN

Transitions:

* INIT → READY (after self-checks)
* READY → RUN (valid torque request)
* RUN → FAULT (any critical fault)
* FAULT → SHUTDOWN

---

# 8. Code-Based Constraints (CBD-specific)

### CR-01: MISRA Compliance

* Code shall comply with:

  * MISRA C:2012 guidelines

---

### CR-02: Memory Constraints

* Stack usage:

  * ≤ 4 KB per task
* No dynamic memory allocation allowed

---

### CR-03: Fixed-Point Implementation

* Floating-point shall not be used in:

  * Real-time control loops
* Use Q-format (e.g., Q15, Q31)

---

### CR-04: Testability

* Each module shall support:

  * Unit testing (e.g., GoogleTest)
  * SIL/PIL validation

---

# 9. Edge Case Requirements (Advanced)

### ER-01: Low Voltage Operation

* If DC bus voltage < threshold:

  * Reduce torque proportionally

---

### ER-02: High-Speed Operation

* Above base speed:

  * Enable field weakening

---

### ER-03: Zero Speed Control

* Ensure smooth torque at:

  * 0 RPM (no jerks)

---

# 10. Example Pseudo-Code Snippet

```c
// Torque arbitration
float finalTorque = MIN(driverTorque, safetyLimit);
finalTorque = MIN(finalTorque, thermalLimit);

// Rate limiter
float delta = finalTorque - prevTorque;
if (delta > MAX_DELTA)
    finalTorque = prevTorque + MAX_DELTA;

// Fault handling
if (overCurrent == TRUE) {
    disablePWM();
    state = FAULT;
}
```

---

# Confidence & Transparency

* These requirements are based on **standard EV inverter control architectures**, common industry practices, and publicly known safety frameworks like ISO 26262.
* Exact numerical thresholds (e.g., timings, limits) may vary by OEM and hardware.
* I cannot confirm proprietary OEM-specific requirement values, but the structure and complexity level reflect real automotive development programs with high confidence.
