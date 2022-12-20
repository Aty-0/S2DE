#pragma once
namespace S2DE::Math
{
    template<class T>
    static T Normalize(T input, T max, T min)
    {
        return (input - min) / (max - min);
    }
}