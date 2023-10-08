#pragma once

namespace CS
{
    template <typename Type>
    Type lerp(Type sample_x, Type sample_x1, Type phase)
    {
        return (1 - phase) * sample_x + phase * sample_x1;
    }

    template <typename Type>
    Type smoothParameter(Type inParameterSmoothed, Type inNewParameter)
    {
        return inParameterSmoothed -
               0.0001 * (inParameterSmoothed - inNewParameter);
    }

    template <typename Type>
    Type jmap(Type sourceValue, Type sourceRangeMin, Type sourceRangeMax, Type targetRangeMin, Type targetRangeMax)
    {
        return targetRangeMin + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
    }
}