#pragma once

// SIMD abstraction layer - ARM NEON for now, extensible to SSE/AVX later

#if defined(__ARM_NEON) || defined(__aarch64__)
#define SIMD_NEON 1
#include <arm_neon.h>

namespace SIMD
{
using Vec4f = float32x4_t;

// Constants
static constexpr int kVecSize = 4;

// Load/Store
inline Vec4f load(const float* ptr)
{
    return vld1q_f32(ptr);
}
inline void store(float* ptr, Vec4f v)
{
    vst1q_f32(ptr, v);
}

// Set all lanes to same value
inline Vec4f set1(float value)
{
    return vdupq_n_f32(value);
}

// Arithmetic
inline Vec4f add(Vec4f a, Vec4f b)
{
    return vaddq_f32(a, b);
}
inline Vec4f sub(Vec4f a, Vec4f b)
{
    return vsubq_f32(a, b);
}
inline Vec4f mul(Vec4f a, Vec4f b)
{
    return vmulq_f32(a, b);
}

// Fused multiply-add: a + (b * c)
inline Vec4f madd(Vec4f a, Vec4f b, Vec4f c)
{
    return vmlaq_f32(a, b, c);
}

// Fused multiply-sub: a - (b * c)
inline Vec4f msub(Vec4f a, Vec4f b, Vec4f c)
{
    return vmlsq_f32(a, b, c);
}
} // namespace SIMD

#elif defined(__SSE__) || defined(__x86_64__) || defined(_M_X64)
#define SIMD_SSE 1
#include <xmmintrin.h>

namespace SIMD
{
using Vec4f = __m128;

static constexpr int kVecSize = 4;

inline Vec4f load(const float* ptr)
{
    return _mm_loadu_ps(ptr);
}
inline void store(float* ptr, Vec4f v)
{
    _mm_storeu_ps(ptr, v);
}
inline Vec4f set1(float value)
{
    return _mm_set1_ps(value);
}
inline Vec4f add(Vec4f a, Vec4f b)
{
    return _mm_add_ps(a, b);
}
inline Vec4f sub(Vec4f a, Vec4f b)
{
    return _mm_sub_ps(a, b);
}
inline Vec4f mul(Vec4f a, Vec4f b)
{
    return _mm_mul_ps(a, b);
}

// SSE doesn't have native FMA, emulate it
inline Vec4f madd(Vec4f a, Vec4f b, Vec4f c)
{
    return _mm_add_ps(a, _mm_mul_ps(b, c));
}
inline Vec4f msub(Vec4f a, Vec4f b, Vec4f c)
{
    return _mm_sub_ps(a, _mm_mul_ps(b, c));
}
} // namespace SIMD

#else
#define SIMD_SCALAR 1

namespace SIMD
{
// Scalar fallback for platforms without SIMD
struct Vec4f
{
    float v[4];
};

static constexpr int kVecSize = 4;

inline Vec4f load(const float* ptr)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = ptr[i];
    return result;
}

inline void store(float* ptr, Vec4f v)
{
    for (int i = 0; i < 4; ++i)
        ptr[i] = v.v[i];
}

inline Vec4f set1(float value)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = value;
    return result;
}

inline Vec4f add(Vec4f a, Vec4f b)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = a.v[i] + b.v[i];
    return result;
}

inline Vec4f sub(Vec4f a, Vec4f b)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = a.v[i] - b.v[i];
    return result;
}

inline Vec4f mul(Vec4f a, Vec4f b)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = a.v[i] * b.v[i];
    return result;
}

inline Vec4f madd(Vec4f a, Vec4f b, Vec4f c)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = a.v[i] + b.v[i] * c.v[i];
    return result;
}

inline Vec4f msub(Vec4f a, Vec4f b, Vec4f c)
{
    Vec4f result;
    for (int i = 0; i < 4; ++i)
        result.v[i] = a.v[i] - b.v[i] * c.v[i];
    return result;
}
} // namespace SIMD
#endif
