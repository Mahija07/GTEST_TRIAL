#include <gtest/gtest.h>
#include "tic.h"

// Helper to drive state machine into RUN state
static void enterRunState(TIC &tic, TIC::Output &out) {
    TIC::Input in = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    tic.step(in, out);  // INIT -> READY
    tic.step(in, out);  // READY -> RUN
}

TEST(TIC_Test, ConstructorAndInitialRun) {
    TIC tic;
    TIC::Output out;
    enterRunState(tic, out);

    TIC::Input in = {100.0f, 200.0f, 150.0f, 0.0f, 0.0f, 0.0f};
    tic.step(in, out);
    EXPECT_FLOAT_EQ(out.finalTorque, 5.0f); // rate limiter from 0
}

TEST(TIC_Test, Min3AndRateLimiting) {
    TIC tic;
    TIC::Output out;
    enterRunState(tic, out);

    TIC::Input in = {100.0f, 200.0f, 300.0f, 0.0f, 0.0f, 0.0f};
    tic.step(in, out);
    EXPECT_FLOAT_EQ(out.finalTorque, 5.0f);

    in.driverTorque = 300.0f;
    in.safetyLimit = 120.0f;
    in.thermalLimit = 400.0f;
    tic.step(in, out);
    // target is 120, prev=5 -> limited to 10
    EXPECT_FLOAT_EQ(out.finalTorque, 10.0f);
}

TEST(TIC_Test, FaultOverCurrent) {
    TIC tic;
    TIC::Output out;
    enterRunState(tic, out);

    TIC::Input in = {100.0f, 200.0f, 200.0f, 0.0f, 210.0f, 0.0f};
    tic.step(in, out);
    EXPECT_FALSE(out.faultActive); // first transition to FAULT does not set flag

    tic.step(in, out);  // FAULT -> SHUTDOWN
    EXPECT_TRUE(out.faultActive);
}

TEST(TIC_Test, FaultOverTemperature) {
    TIC tic;
    TIC::Output out;
    enterRunState(tic, out);

    TIC::Input in = {100.0f, 200.0f, 200.0f, 0.0f, 0.0f, 130.0f};
    tic.step(in, out);
    EXPECT_FALSE(out.faultActive);

    tic.step(in, out);
    EXPECT_TRUE(out.faultActive);
}

TEST(TIC_Test, PWMInRun) {
    TIC tic;
    TIC::Output out;
    enterRunState(tic, out);

    TIC::Input in = {300.0f, 300.0f, 300.0f, 0.0f, 0.0f, 0.0f};
    // Increase through rate limiter until full output
    for (int i = 0; i < 60; ++i) {
        tic.step(in, out);
    }

    EXPECT_FLOAT_EQ(out.finalTorque, 300.0f);
    EXPECT_FLOAT_EQ(out.pwmDuty[0], 1.0f);
    EXPECT_FLOAT_EQ(out.pwmDuty[1], 0.25f);
    EXPECT_FLOAT_EQ(out.pwmDuty[2], 0.25f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
