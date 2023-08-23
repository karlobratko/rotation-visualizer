//
// Created by kbratko on 7/26/2023.
//

#ifndef MICRO_PERFORMANCE_BASE_H
#define MICRO_PERFORMANCE_BASE_H

#include <chrono>
#include <type_traits>

#include "../core/types.h"

namespace micro::perf {
    template<typename>
    struct is_duration : std::false_type { };

    template<typename R, typename P>
    struct is_duration<std::chrono::duration<R, P>> : std::true_type { };

    template<typename T>
    inline constexpr bool is_duration_v = is_duration<T>::value;
}

#endif //MICRO_PERFORMANCE_BASE_H
