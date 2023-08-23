//
// Created by kbratko on 7/26/2023.
//

#ifndef MICRO_UTILS_TIME_H
#define MICRO_UTILS_TIME_H

#include <glfw/glfw3.h>

#include "../core/types.h"

namespace micro::utils {
    template<typename T>
        requires std::is_floating_point_v<T>
    fn time() -> T { return static_cast<T>(glfwGetTime()); }
}

#endif //MICRO_UTILS_TIME_H
