#include "tic.h"
#include "tic_c_api.h"

extern "C" {

void* tic_create()
{
    return static_cast<void*>(new TIC());
}

void tic_destroy(void* handle)
{
    if (handle) delete static_cast<TIC*>(handle);
}

void tic_step(void* handle, const TIC_Input* in, TIC_Output* out)
{
    if (!handle || !in || !out) return;

    TIC* obj = static_cast<TIC*>(handle);
    TIC::Input cpp_in;
    cpp_in.driverTorque = in->driverTorque;
    cpp_in.safetyLimit = in->safetyLimit;
    cpp_in.thermalLimit = in->thermalLimit;
    cpp_in.motorSpeed = in->motorSpeed;
    cpp_in.phaseCurrent = in->phaseCurrent;
    cpp_in.temperature = in->temperature;

    TIC::Output cpp_out;
    obj->step(cpp_in, cpp_out);

    out->finalTorque = cpp_out.finalTorque;
    out->faultActive = cpp_out.faultActive;
    for (int i = 0; i < 3; ++i) out->pwmDuty[i] = cpp_out.pwmDuty[i];
}

} // extern "C"
