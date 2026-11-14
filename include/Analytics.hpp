#pragma once
#include <vector>

template <typename T>
T minimum(const std::vector<T>& values) {
    T result = values[0];
    for (const auto& value : values) {
        if (value < result) {
            result = value;
        }
    }
    return result;
}

template <typename T>
T maximum(const std::vector<T>& values) {
    T result = values[0];
    for (const auto& value : values) {
        if (value > result) {
            result = value;
        }
    }
    return result;
}

template <typename T>
double average(const std ::vector<T>& values) {
    double total = 0.0;
    for (const auto& value : values) {
        total += value;
    }
    double count = values.size();
    return total / count;
}