//
// Created by kbratko on 7/25/2023.
//

#ifndef MICRO_GRAPHICS_VERTEX_H
#define MICRO_GRAPHICS_VERTEX_H

#include "../core/types.h"
#include "../mathematics/linear.h"

namespace micro::graphics {
    struct Vertex {
        math::vector4<core::f32> position{};
        math::vector3<core::f32> uv{};
        math::vector3<core::f32> normals{};
        math::vector3<core::f32> color{};

        constexpr explicit Vertex() = default;

        constexpr explicit Vertex(math::vector4<core::f32> const &position);

        constexpr explicit Vertex(math::vector4<core::f32> const &position,
                                  math::vector3<core::f32> const &uv,
                                  math::vector3<core::f32> const &normals,
                                  math::vector3<core::f32> const &color);

        static constexpr fn from_position(math::vector4<core::f32> const &position);

        static constexpr fn from(math::vector4<core::f32> const &position,
                                 math::vector3<core::f32> const &uv,
                                 math::vector3<core::f32> const &normals,
                                 math::vector3<core::f32> const &color);

        constexpr friend fn operator==(Vertex const &v1, Vertex const &v2) -> bool;

        constexpr friend fn operator!=(Vertex const &v1, Vertex const &v2) -> bool;
    };

    constexpr fn operator==(Vertex const &v1, Vertex const &v2) -> bool {
        if (&v1 == &v2) return true;

        return v1.position == v2.position &&
               v1.uv == v2.uv &&
               v1.normals == v2.normals &&
               v1.color == v2.color;
    }

    constexpr fn operator!=(Vertex const &v1, Vertex const &v2) -> bool { return !(v1 == v2); }

    constexpr Vertex::Vertex(math::vector4<core::f32> const &_position) : position{_position} {}

    constexpr Vertex::Vertex(math::vector4<core::f32> const &_position,
                             math::vector3<core::f32> const &_uv,
                             math::vector3<core::f32> const &_normals,
                             math::vector3<core::f32> const &_color)
        : position{_position}, uv{_uv}, normals{_normals}, color{_color} {}

    constexpr fn Vertex::from_position(math::vector4<core::f32> const &position) { return Vertex{position}; }

    constexpr fn Vertex::from(math::vector4<core::f32> const &position,
                              math::vector3<core::f32> const &uv,
                              math::vector3<core::f32> const &normals,
                              math::vector3<core::f32> const &color) { return Vertex{position, uv, normals, color}; }
}

#endif //MICRO_GRAPHICS_VERTEX_H
