#include <iostream>
#include "tic.h"

int main()
{
    TIC tic;

    TIC::Input input;
    input.driverTorque = 150.0f;
    input.safetyLimit = 200.0f;
    input.thermalLimit = 180.0f;
    input.motorSpeed = 3000.0f;
    input.phaseCurrent = 100.0f;
    input.temperature = 60.0f;

    TIC::Output output;

    for (int i = 0; i < 10; ++i)
    {
        tic.step(input, output);

        std::cout << "Step " << i << ":\n";
        std::cout << "Torque: " << output.finalTorque << "\n";
        std::cout << "PWM: ["
                  << output.pwmDuty[0] << ", "
                  << output.pwmDuty[1] << ", "
                  << output.pwmDuty[2] << "]\n";
        std::cout << "Fault: " << output.faultActive << "\n\n";
    }

    return 0;
}