#include "tic.h"

/* ===== CONSTRUCTOR ===== */
TIC::TIC()
{
    state = INIT;
    prevTorque = 0.0f;
}

/* ===== HELPERS ===== */
float TIC::min3(float a, float b, float c)
{
    float m = (a < b) ? a : b;
    return (m < c) ? m : c;
}

float TIC::rateLimiter(float target, float prev)
{
    float delta = target - prev;

    if (delta > MAX_DELTA_TORQUE)
        return prev + MAX_DELTA_TORQUE;
    else if (delta < -MAX_DELTA_TORQUE)
        return prev - MAX_DELTA_TORQUE;

    return target;
}

bool TIC::checkOverCurrent(float current)
{
    return current > OVERCURRENT_LIMIT;
}

bool TIC::checkOverTemp(float temp)
{
    return temp > TEMP_LIMIT;
}

void TIC::generatePWM(float torque, std::array<float, 3>& duty)
{
    float base = torque / MAX_TORQUE;

    if (base > 1.0f) base = 1.0f;
    if (base < -1.0f) base = -1.0f;

    duty[0] = 0.5f + 0.5f * base;
    duty[1] = 0.5f - 0.25f * base;
    duty[2] = 0.5f - 0.25f * base;
}

/* ===== MAIN STEP ===== */
void TIC::step(const Input& in, Output& out)
{
    /* ALWAYS initialize outputs (critical fix) */
    out.finalTorque = 0.0f;
    out.pwmDuty = {0.0f, 0.0f, 0.0f};
    out.faultActive = false;

    switch (state)
    {
        case INIT:
            state = READY;
            break;

        case READY:
            state = RUN;
            break;

        case RUN:
        {
            if (checkOverCurrent(in.phaseCurrent) ||
                checkOverTemp(in.temperature))
            {
                state = FAULT;
                break;
            }

            float torque = min3(
                in.driverTorque,
                in.safetyLimit,
                in.thermalLimit
            );

            torque = rateLimiter(torque, prevTorque);

            prevTorque = torque;
            out.finalTorque = torque;

            generatePWM(torque, out.pwmDuty);
            break;
        }

        case FAULT:
            out.faultActive = true;
            state = SHUTDOWN;
            break;

        case SHUTDOWN:
        default:
            break;
    }
}