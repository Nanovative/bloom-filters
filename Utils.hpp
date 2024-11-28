#pragma once

#include <cmath>

namespace BloomFilterUtils
{
    // Calculate the optimal number of hash functions (k)
    inline int CalculateOptimalK(int m, int n)
    {
        // k = (m / n) * ln(2)
        return static_cast<int>(std::round((m / static_cast<double>(n)) * std::log(2)));
    }

    // Calculate the optimal size of the Bloom filter (m)
    inline int CalculateOptimalM(int n, double p)
    {
        // m = - (n * ln(p)) / (ln(2) ^ 2)
        return static_cast<int>(std::round(-(n * std::log(p)) / (std::log(2) * std::log(2))));
    }
}
