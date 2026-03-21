#ifndef TIC_H
#define TIC_H

#include <array>

class TIC
{
public:
    /* ===== STATE ===== */
    enum State
    {
        INIT = 0,
        READY,
        RUN,
        FAULT,
        SHUTDOWN
    };

    /* ===== INPUT ===== */
    struct Input
    {
        float driverTorque;
        float safetyLimit;
        float thermalLimit;
        float motorSpeed;
        float phaseCurrent;
        float temperature;
    };

    /* ===== OUTPUT ===== */
    struct Output
    {
        float finalTorque;
        std::array<float, 3> pwmDuty;
        bool faultActive;
    };

    /* ===== CONSTRUCTOR ===== */
    TIC();

    /* ===== MAIN FUNCTION ===== */
    void step(const Input& in, Output& out);

private:
    /* ===== INTERNAL STATE ===== */
    State state;
    float prevTorque;

    /* ===== CONSTANTS ===== */
    static constexpr float MAX_TORQUE = 300.0f;
    static constexpr float MAX_DELTA_TORQUE = 5.0f;
    static constexpr float OVERCURRENT_LIMIT = 200.0f;
    static constexpr float TEMP_LIMIT = 120.0f;

    /* ===== HELPERS ===== */
    float min3(float a, float b, float c);
    float rateLimiter(float target, float prev);
    bool checkOverCurrent(float current);
    bool checkOverTemp(float temp);
    void generatePWM(float torque, std::array<float, 3>& duty);

friend class TIC_Test_Min3_Test;
friend class TIC_Test_RateLimiter_Test;
friend class TIC_Test_CheckOverCurrent_Test;
friend class TIC_Test_CheckOverTemp_Test;
friend class TIC_Test_GeneratePWM_Test;

};

#endif