#include "../SoundEnginePlugin/SimdOps.h"
#include <cmath>
#include <gtest/gtest.h>

class SimdTest : public ::testing::Test
{
protected:
    static constexpr float kEpsilon = 1e-6f;

    void ExpectVecNear(const float* result, const float* expected, int count = SIMD::kVecSize)
    {
        for (int i = 0; i < count; ++i)
        {
            EXPECT_NEAR(result[i], expected[i], kEpsilon) << "Mismatch at index " << i;
        }
    }
};

TEST_F(SimdTest, VecSizeIsFour)
{
    EXPECT_EQ(SIMD::kVecSize, 4);
}

TEST_F(SimdTest, LoadStore)
{
    alignas(16) float input[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float output[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    SIMD::Vec4f vec = SIMD::load(input);
    SIMD::store(output, vec);

    ExpectVecNear(output, input);
}

TEST_F(SimdTest, Set1Broadcast)
{
    alignas(16) float output[4];

    SIMD::Vec4f vec = SIMD::set1(5.0f);
    SIMD::store(output, vec);

    float expected[4] = {5.0f, 5.0f, 5.0f, 5.0f};
    ExpectVecNear(output, expected);
}

TEST_F(SimdTest, Add)
{
    alignas(16) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float b[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecResult = SIMD::add(vecA, vecB);
    SIMD::store(result, vecResult);

    float expected[4] = {11.0f, 22.0f, 33.0f, 44.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, Subtract)
{
    alignas(16) float a[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    alignas(16) float b[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecResult = SIMD::sub(vecA, vecB);
    SIMD::store(result, vecResult);

    float expected[4] = {9.0f, 18.0f, 27.0f, 36.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, Multiply)
{
    alignas(16) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float b[4] = {2.0f, 3.0f, 4.0f, 5.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecResult = SIMD::mul(vecA, vecB);
    SIMD::store(result, vecResult);

    float expected[4] = {2.0f, 6.0f, 12.0f, 20.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, MultiplyAdd)
{
    // Test: a + (b * c)
    alignas(16) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float b[4] = {2.0f, 3.0f, 4.0f, 5.0f};
    alignas(16) float c[4] = {10.0f, 10.0f, 10.0f, 10.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecC = SIMD::load(c);
    SIMD::Vec4f vecResult = SIMD::madd(vecA, vecB, vecC);
    SIMD::store(result, vecResult);

    float expected[4] = {21.0f, 32.0f, 43.0f, 54.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, MultiplySub)
{
    // Test: a - (b * c)
    alignas(16) float a[4] = {100.0f, 100.0f, 100.0f, 100.0f};
    alignas(16) float b[4] = {2.0f, 3.0f, 4.0f, 5.0f};
    alignas(16) float c[4] = {10.0f, 10.0f, 10.0f, 10.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecC = SIMD::load(c);
    SIMD::Vec4f vecResult = SIMD::msub(vecA, vecB, vecC);
    SIMD::store(result, vecResult);

    float expected[4] = {80.0f, 70.0f, 60.0f, 50.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, NegativeValues)
{
    alignas(16) float a[4] = {-1.0f, -2.0f, -3.0f, -4.0f};
    alignas(16) float b[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecResult = SIMD::add(vecA, vecB);
    SIMD::store(result, vecResult);

    float expected[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, ZeroValues)
{
    alignas(16) float a[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float b[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecResult = SIMD::mul(vecA, vecB);
    SIMD::store(result, vecResult);

    float expected[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, ChainedOperations)
{
    // Test: ((a + b) * c) - d
    alignas(16) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float b[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    alignas(16) float c[4] = {2.0f, 2.0f, 2.0f, 2.0f};
    alignas(16) float d[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float result[4];

    SIMD::Vec4f vecA = SIMD::load(a);
    SIMD::Vec4f vecB = SIMD::load(b);
    SIMD::Vec4f vecC = SIMD::load(c);
    SIMD::Vec4f vecD = SIMD::load(d);

    SIMD::Vec4f vecTemp = SIMD::add(vecA, vecB);      // a + b
    vecTemp = SIMD::mul(vecTemp, vecC);               // * c
    SIMD::Vec4f vecResult = SIMD::sub(vecTemp, vecD); // - d
    SIMD::store(result, vecResult);

    float expected[4] = {3.0f, 4.0f, 5.0f, 6.0f};
    ExpectVecNear(result, expected);
}

TEST_F(SimdTest, AudioMixExample)
{
    // Simulate audio mixing: output = wet * wetGain + dry * dryGain
    alignas(16) float wet[4] = {0.5f, 0.6f, 0.7f, 0.8f};
    alignas(16) float dry[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    alignas(16) float result[4];

    float wetGain = 0.7f;
    float dryGain = 0.3f;

    SIMD::Vec4f vecWet = SIMD::load(wet);
    SIMD::Vec4f vecDry = SIMD::load(dry);
    SIMD::Vec4f vecWetGain = SIMD::set1(wetGain);
    SIMD::Vec4f vecDryGain = SIMD::set1(dryGain);

    // output = wet * wetGain
    SIMD::Vec4f vecResult = SIMD::mul(vecWet, vecWetGain);
    // output = output + (dry * dryGain)
    vecResult = SIMD::madd(vecResult, vecDry, vecDryGain);
    SIMD::store(result, vecResult);

    float expected[4] = {0.5f * 0.7f + 1.0f * 0.3f, 0.6f * 0.7f + 1.0f * 0.3f,
                         0.7f * 0.7f + 1.0f * 0.3f, 0.8f * 0.7f + 1.0f * 0.3f};
    ExpectVecNear(result, expected);
}
