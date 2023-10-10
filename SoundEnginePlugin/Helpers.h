#pragma once

namespace CS
{
    static constexpr float kParamCoeff_Generic = 0.04f;
    static constexpr float kParamCoeff_Fine = 0.0002;

    template <typename Type>
    inline Type lerp(Type sample_x, Type sample_x1, Type phase)
    {
        return (1 - phase) * sample_x + phase * sample_x1;
    }

    template <typename Type>
    inline Type smoothParameter(Type inParameterSmoothed, Type inNewParameter, AkReal32 smoothingCoeff)
    {
        return inParameterSmoothed -
               smoothingCoeff * (inParameterSmoothed - inNewParameter);
    }

    template <typename Type>
    inline Type jmap(Type sourceValue, Type sourceRangeMin, Type sourceRangeMax, Type targetRangeMin, Type targetRangeMax)
    {
        return targetRangeMin + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
    }
}