#include "LFO.h"
#include <cmath>
#include <gtest/gtest.h>

class LFOTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        lfo = std::make_unique<LFO>();
        sampleRate = 48000;
    }

    std::unique_ptr<LFO> lfo;
    uint32_t sampleRate;
};

TEST_F(LFOTest, InitialPhase)
{
    // First output should be at phase 0 (sin(0) = 0)
    float output = lfo->get(1.0f, 0.0f, sampleRate);
    EXPECT_NEAR(output, 0.0f, 0.01f);
}

TEST_F(LFOTest, OutputRange)
{
    // LFO output should be between -1 and 1
    for (int i = 0; i < 1000; i++)
    {
        float output = lfo->get(10.0f, 0.0f, sampleRate);
        EXPECT_GE(output, -1.0f);
        EXPECT_LE(output, 1.0f);
    }
}

TEST_F(LFOTest, PhaseProgression)
{
    // At 1 Hz rate, after sampleRate samples, should complete one cycle
    float rate = 1.0f;
    float firstSample = lfo->get(rate, 0.0f, sampleRate);

    // Advance through almost one complete cycle
    for (uint32_t i = 1; i < sampleRate; i++)
    {
        lfo->get(rate, 0.0f, sampleRate);
    }

    float lastSample = lfo->get(rate, 0.0f, sampleRate);

    // Should be back near the starting value
    EXPECT_NEAR(firstSample, lastSample, 0.1f);
}

TEST_F(LFOTest, PhaseOffset)
{
    LFO lfo1, lfo2;

    // Get value with no offset
    float output1 = lfo1.get(1.0f, 0.0f, sampleRate);

    // Get value with 180 degree offset (0.5 phase offset)
    float output2 = lfo2.get(1.0f, 0.5f, sampleRate);

    // Outputs should be opposite (inverted)
    EXPECT_NEAR(output1, -output2, 0.01f);
}

TEST_F(LFOTest, FrequencyChange)
{
    // Higher frequency should produce faster oscillation
    LFO lfoSlow, lfoFast;

    // Need enough samples to capture multiple cycles for 1Hz (need at least 1 second = sampleRate
    // samples)
    int numSamples = sampleRate * 2; // 2 seconds worth to get good crossings

    // Count samples for slow LFO (1 Hz)
    float slowPrev = lfoSlow.get(1.0f, 0.0f, sampleRate);
    int slowCrossings = 0;
    for (int i = 1; i < numSamples; i++)
    {
        float slowCurr = lfoSlow.get(1.0f, 0.0f, sampleRate);
        if ((slowPrev < 0 && slowCurr >= 0) || (slowPrev >= 0 && slowCurr < 0))
            slowCrossings++;
        slowPrev = slowCurr;
    }

    // Count samples for fast LFO (10 Hz)
    float fastPrev = lfoFast.get(10.0f, 0.0f, sampleRate);
    int fastCrossings = 0;
    for (int i = 1; i < numSamples; i++)
    {
        float fastCurr = lfoFast.get(10.0f, 0.0f, sampleRate);
        if ((fastPrev < 0 && fastCurr >= 0) || (fastPrev >= 0 && fastCurr < 0))
            fastCrossings++;
        fastPrev = fastCurr;
    }

    // Fast LFO (10 Hz) should have significantly more zero crossings than slow (1 Hz)
    EXPECT_GT(fastCrossings, slowCrossings);
    EXPECT_GE(slowCrossings, 1); // Should have at least one crossing
}

TEST_F(LFOTest, SineWaveShape)
{
    // Verify that output follows sine wave pattern
    float rate = 1.0f;

    // Get value at phase 0.25 (90 degrees, should be ~1)
    LFO lfoQuarter;
    for (uint32_t i = 0; i < sampleRate / 4; i++)
    {
        lfoQuarter.get(rate, 0.0f, sampleRate);
    }
    float outputAt90 = lfoQuarter.get(rate, 0.0f, sampleRate);

    EXPECT_NEAR(outputAt90, 1.0f, 0.1f);
}

TEST_F(LFOTest, PhaseWrapAround)
{
    // Test phase offset wrapping (> 1.0)
    float output1 = lfo->get(1.0f, 0.0f, sampleRate);

    LFO lfo2;
    float output2 = lfo2.get(1.0f, 1.0f, sampleRate); // Should wrap to 0

    // Should produce same result
    EXPECT_NEAR(output1, output2, 0.01f);
}
