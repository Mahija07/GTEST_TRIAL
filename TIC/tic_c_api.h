#ifndef TIC_C_API_H
#define TIC_C_API_H

#ifdef _WIN32
#ifdef TIC_EXPORTS
#define TIC_API __declspec(dllexport)
#else
#define TIC_API __declspec(dllimport)
#endif
#else
#define TIC_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TIC_Input {
    float driverTorque;
    float safetyLimit;
    float thermalLimit;
    float motorSpeed;
    float phaseCurrent;
    float temperature;
} TIC_Input;

typedef struct TIC_Output {
    float finalTorque;
    float pwmDuty[3];
    bool faultActive;
} TIC_Output;

TIC_API void* tic_create();
TIC_API void tic_destroy(void* handle);
TIC_API void tic_step(void* handle, const TIC_Input* in, TIC_Output* out);

#ifdef __cplusplus
}
#endif

#endif // TIC_C_API_H
