#include "Delayline.h"
#include <cmath>
#include <gtest/gtest.h>

class DelaylineTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sampleRate = 48000.0f;
        maxDelayTime = 1.0f;
        delayline = std::make_unique<Delayline>(sampleRate, maxDelayTime);
    }

    float sampleRate;
    float maxDelayTime;
    std::unique_ptr<Delayline> delayline;
};

TEST_F(DelaylineTest, WriteAndReadWithDelay)
{
    // Write a pattern: zeros, test value, more zeros
    for (int i = 0; i < 20; i++)
    {
        delayline->write(0.0f);
        delayline->updateWriteHead();
    }

    // Write the test value at position 20
    float testValue = 0.75f;
    delayline->write(testValue);
    delayline->updateWriteHead();

    // Write 9 more zeros (positions 21-29)
    for (int i = 0; i < 9; i++)
    {
        delayline->write(0.0f);
        delayline->updateWriteHead();
    }

    // Now we're at position 30, looking back 10 samples should give us position 20
    delayline->updateReadHead(10.0f);
    float readValue = delayline->read();

    // Should read the test value we wrote at position 20
    EXPECT_NEAR(readValue, testValue, 0.01f);
}

TEST_F(DelaylineTest, LinearInterpolation)
{
    // Write a ramp signal
    for (int i = 0; i < 100; i++)
    {
        delayline->write(static_cast<float>(i) / 100.0f);
        delayline->updateWriteHead();
    }

    // Read with fractional delay (should interpolate)
    delayline->updateReadHead(10.5f); // 10.5 samples delay
    float readValue = delayline->read();

    // Should interpolate between samples
    float expected = (89.0f + 90.0f) / 200.0f; // Approximate interpolation
    EXPECT_NEAR(readValue, expected, 0.05f);
}

TEST_F(DelaylineTest, ZeroDelay)
{
    // Write several samples first
    for (int i = 0; i < 5; i++)
    {
        delayline->write(0.0f);
        delayline->updateWriteHead();
    }

    // Write test value
    float testValue = 0.5f;
    delayline->write(testValue);
    delayline->updateWriteHead();

    // Read with minimal delay (1 sample)
    delayline->updateReadHead(1.0f);
    float readValue = delayline->read();

    // Should get very close to the written value
    EXPECT_NEAR(readValue, testValue, 0.1f);
}

TEST_F(DelaylineTest, MaxDelayTime)
{
    // Write samples for full buffer length
    size_t bufferLength = static_cast<size_t>(sampleRate * maxDelayTime);

    for (size_t i = 0; i < bufferLength; i++)
    {
        delayline->write(static_cast<float>(i) / static_cast<float>(bufferLength));
        delayline->updateWriteHead();
    }

    // Read at maximum delay
    delayline->updateReadHead(bufferLength - 10.0f);
    EXPECT_NO_THROW(delayline->read());
}
