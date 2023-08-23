//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_UTILS_COLORS_H
#define MICRO_UTILS_COLORS_H

#include "../core/types.h"
#include "../mathematics/linear.h"

namespace micro::color {
    namespace rgba {
        static constexpr math::vector4<core::f32> Black = {0.f, 0.f, 0.f, 1.f};
        static constexpr math::vector4<core::f32> White = {1.f, 1.f, 1.f, 1.f};
        static constexpr math::vector4<core::f32> Red   = {1.f, 0.f, 0.f, 1.f};
        static constexpr math::vector4<core::f32> Green = {0.f, 1.f, 0.f, 1.f};
        static constexpr math::vector4<core::f32> Blue  = {0.f, 0.f, 1.f, 1.f};
    }

    namespace rgb {
        static constexpr math::vector3<core::f32> Black = {0.f, 0.f, 0.f};
        static constexpr math::vector3<core::f32> White = {1.f, 1.f, 1.f};
        static constexpr math::vector3<core::f32> Red   = {1.f, 0.f, 0.f};
        static constexpr math::vector3<core::f32> Green = {0.f, 1.f, 0.f};
        static constexpr math::vector3<core::f32> Blue  = {0.f, 0.f, 1.f};
    }

    fn i32ToRgba(core::i32 i) -> math::vector4<core::f32> {
        return {
            static_cast<core::f32>((i >> 0) & 0xff) / 255.0f,
            static_cast<core::f32>((i >> 8) & 0xff) / 255.0f,
            static_cast<core::f32>((i >> 16) & 0xff) / 255.0f,
            static_cast<core::f32>((i >> 24) & 0xff) / 255.0f
        };
    }

    fn i32ToRgb(core::i32 i) -> math::vector3<core::f32> {
        return {
            static_cast<core::f32>((i >> 0) & 0xff) / 255.0f,
            static_cast<core::f32>((i >> 8) & 0xff) / 255.0f,
            static_cast<core::f32>((i >> 16) & 0xff) / 255.0f
        };
    }
}

#endif //MICRO_UTILS_COLORS_H
