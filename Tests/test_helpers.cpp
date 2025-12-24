#include "Helpers.h"
#include <cmath>
#include <gtest/gtest.h>

TEST(HelpersTest, LerpBasic)
{
    // Test basic linear interpolation
    EXPECT_FLOAT_EQ(CS::lerp(0.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(CS::lerp(0.0f, 1.0f, 1.0f), 1.0f);
    EXPECT_FLOAT_EQ(CS::lerp(0.0f, 1.0f, 0.5f), 0.5f);
}

TEST(HelpersTest, LerpIntermediate)
{
    // Test at 25% and 75%
    EXPECT_FLOAT_EQ(CS::lerp(0.0f, 100.0f, 0.25f), 25.0f);
    EXPECT_FLOAT_EQ(CS::lerp(0.0f, 100.0f, 0.75f), 75.0f);
}

TEST(HelpersTest, LerpNegativeValues)
{
    // Test with negative numbers
    EXPECT_FLOAT_EQ(CS::lerp(-1.0f, 1.0f, 0.5f), 0.0f);
    EXPECT_FLOAT_EQ(CS::lerp(-10.0f, 10.0f, 0.25f), -5.0f);
}

TEST(HelpersTest, SmoothParameterConvergence)
{
    // Test that smoothing converges toward target
    float current = 0.0f;
    float target = 1.0f;
    float coeff = 0.1f;

    // Apply smoothing multiple times
    for (int i = 0; i < 100; i++)
    {
        current = CS::smoothParameter(current, target, coeff);
    }

    // Should be very close to target after many iterations
    EXPECT_NEAR(current, target, 0.01f);
}

TEST(HelpersTest, SmoothParameterDirection)
{
    // Test smoothing moves in correct direction
    float current = 0.0f;
    float target = 1.0f;

    float smoothed = CS::smoothParameter(current, target, CS::kParamCoeff_Fine);

    EXPECT_GT(smoothed, current);
    EXPECT_LT(smoothed, target);
}

TEST(HelpersTest, SmoothParameterReversed)
{
    // Test smoothing from high to low
    float current = 1.0f;
    float target = 0.0f;

    float smoothed = CS::smoothParameter(current, target, CS::kParamCoeff_Fine);

    EXPECT_LT(smoothed, current);
    EXPECT_GT(smoothed, target);
}

TEST(HelpersTest, JmapBasic)
{
    // Test basic range mapping
    EXPECT_FLOAT_EQ(CS::jmap(0.0f, 0.0f, 1.0f, 0.0f, 100.0f), 0.0f);
    EXPECT_FLOAT_EQ(CS::jmap(1.0f, 0.0f, 1.0f, 0.0f, 100.0f), 100.0f);
    EXPECT_FLOAT_EQ(CS::jmap(0.5f, 0.0f, 1.0f, 0.0f, 100.0f), 50.0f);
}

TEST(HelpersTest, JmapNegativeRange)
{
    // Test mapping from negative range
    EXPECT_FLOAT_EQ(CS::jmap(0.0f, -1.0f, 1.0f, 0.0f, 100.0f), 50.0f);
    EXPECT_FLOAT_EQ(CS::jmap(-1.0f, -1.0f, 1.0f, 0.0f, 100.0f), 0.0f);
    EXPECT_FLOAT_EQ(CS::jmap(1.0f, -1.0f, 1.0f, 0.0f, 100.0f), 100.0f);
}

TEST(HelpersTest, JmapReverseMapping)
{
    // Test mapping to reverse range
    EXPECT_FLOAT_EQ(CS::jmap(0.0f, 0.0f, 1.0f, 100.0f, 0.0f), 100.0f);
    EXPECT_FLOAT_EQ(CS::jmap(1.0f, 0.0f, 1.0f, 100.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(CS::jmap(0.5f, 0.0f, 1.0f, 100.0f, 0.0f), 50.0f);
}

TEST(HelpersTest, JmapArbitraryRanges)
{
    // Test with arbitrary ranges (LFO depth to milliseconds)
    float lfoValue = 0.5f; // 50% depth
    float mapped = CS::jmap(lfoValue, 0.0f, 1.0f, 0.001f, 0.005f);

    EXPECT_NEAR(mapped, 0.003f, 0.0001f); // Should be 3ms
}
