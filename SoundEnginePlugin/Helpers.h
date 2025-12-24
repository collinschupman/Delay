#pragma once

namespace CS
{
static constexpr float kParamCoeff_Generic = 0.04f;
static constexpr float kParamCoeff_Fine = 0.0002;

template <typename Type>
[[nodiscard]] constexpr Type lerp(Type sample_x, Type sample_x1, Type phase) noexcept
{
    return (1 - phase) * sample_x + phase * sample_x1;
}

template <typename Type>
[[nodiscard]] constexpr Type smoothParameter(Type inParameterSmoothed, Type inNewParameter,
                                             float smoothingCoeff) noexcept
{
    return inParameterSmoothed - smoothingCoeff * (inParameterSmoothed - inNewParameter);
}

template <typename Type>
[[nodiscard]] constexpr Type jmap(Type sourceValue, Type sourceRangeMin, Type sourceRangeMax,
                                  Type targetRangeMin, Type targetRangeMax) noexcept
{
    const Type sourceRange = sourceRangeMax - sourceRangeMin;
    // Avoid division by zero - return target min if source range is zero
    if (sourceRange == Type(0))
        return targetRangeMin;

    return targetRangeMin +
           ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / sourceRange;
}
} // namespace CS