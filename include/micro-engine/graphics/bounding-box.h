//
// Created by kbratko on 7/27/2023.
//

#ifndef MICRO_GRAPHICS_BOUNDING_BOX_H
#define MICRO_GRAPHICS_BOUNDING_BOX_H

#include <numeric>
#include <vector>

#include "../core/types.h"
#include "../mathematics/linear.h"

namespace micro::graphics {
    struct BoundingBox {
        core::minmax<core::f32> x{};
        core::minmax<core::f32> y{};
        core::minmax<core::f32> z{};

        [[nodiscard]] constexpr fn center() const -> math::vector3<core::f32> {
            return math::vector3<core::f32>{
                x.mean(),
                y.mean(),
                z.mean()
            };
        }

        [[nodiscard]] fn vertices() const -> std::vector<math::vector3<core::f32>> {
            return std::vector<math::vector3<core::f32>>{
                math::vector3<core::f32>{x.max, y.min, z.max},
                math::vector3<core::f32>{x.max, y.min, z.min},
                math::vector3<core::f32>{x.min, y.min, z.max},
                math::vector3<core::f32>{x.min, y.min, z.min},
                math::vector3<core::f32>{x.max, y.max, z.max},
                math::vector3<core::f32>{x.max, y.max, z.min},
                math::vector3<core::f32>{x.min, y.max, z.max},
                math::vector3<core::f32>{x.min, y.max, z.min}
            };
        }

        template<typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] fn indices() const -> std::vector<T> {
            return std::vector<T>{
                0,
                4,
                6,
                0,
                6,
                2,
                3,
                2,
                6,
                3,
                6,
                7,
                7,
                6,
                4,
                7,
                4,
                5,
                5,
                1,
                3,
                5,
                3,
                7,
                1,
                0,
                2,
                1,
                2,
                3,
                5,
                4,
                0,
                5,
                0,
                1
            };
        }

        [[nodiscard]] fn maxRadius() const -> core::f32 {
            auto c  = center();
            auto vs = vertices();

            return std::transform_reduce(
                vs.begin(), vs.end(),
                std::numeric_limits<core::f32>::min(),
                [](core::f32 const                  f1, core::f32 const f2) { return math::max(f1, f2); },
                [&](math::vector3<core::f32> const &vec) { return math::distance(vec, c); }
            );
        }

        [[nodiscard]] fn minRadius() const -> core::f32 {
            auto c  = center();
            auto vs = vertices();

            return std::transform_reduce(
                vs.begin(), vs.end(),
                std::numeric_limits<core::f32>::max(),
                [](core::f32 const                  f1, core::f32 const f2) { return math::min(f1, f2); },
                [&](math::vector3<core::f32> const &vec) { return math::distance(vec, c); }
            );
        }

        static fn from(std::vector<math::vector3<core::f32>> const &positions) -> BoundingBox {
            BoundingBox boundingBox{};

            for (auto const &position : positions) {
                boundingBox.x.assign(position.x);
                boundingBox.y.assign(position.y);
                boundingBox.z.assign(position.z);
            }

            return boundingBox;
        }

        static fn from(std::vector<Vertex> const &vertices) -> BoundingBox {
            BoundingBox boundingBox{};

            for (auto const &vertex : vertices) {
                boundingBox.x.assign(vertex.position.x);
                boundingBox.y.assign(vertex.position.y);
                boundingBox.z.assign(vertex.position.z);
            }

            return boundingBox;
        }
    };
}

#endif //MICRO_GRAPHICS_BOUNDING_BOX_H
