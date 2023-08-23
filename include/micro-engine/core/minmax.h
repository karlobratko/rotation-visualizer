//
// Created by kbratko on 7/27/2023.
//

#ifndef MICRO_CORE_MINMAX_H
#define MICRO_CORE_MINMAX_H

#include "types.h"
#include "../mathematics/linear.h"

namespace micro::core {
    template<typename T> requires std::is_arithmetic_v<T>
    struct minmax {
        using value_type = T;

        value_type min;
        value_type max;

        explicit constexpr minmax();

        explicit constexpr minmax(value_type min, value_type max);

        constexpr fn mean() const -> value_type;

        fn assign(value_type value) -> void;
    };

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr minmax<T>::minmax()
        : min{std::numeric_limits<value_type>::max()}, max{std::numeric_limits<value_type>::min()} {}

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr minmax<T>::minmax(value_type _min, value_type _max) : min{_min}, max{_max} {}

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr fn minmax<T>::mean() const -> value_type {
        if constexpr (std::is_integral_v<T>)
            return static_cast<value_type>(math::round((min + max) * 0.5));
        else
            return (min + max) * static_cast<value_type>(0.5);
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    fn minmax<T>::assign(value_type value) -> void {
        if (value < min)
            min = value;
        if (value > max)
            max = value;
    }
}

#endif //MICRO_CORE_MINMAX_H
