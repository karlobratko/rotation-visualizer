//
// Created by kbratko on 8/16/2023.
//

#ifndef FINAL_CONSTANTS_H
#define FINAL_CONSTANTS_H

#include "micro-engine/micro.h"

using namespace micro::core;

namespace Application {
    namespace Window {
        static constexpr cstring Title  = "Rotation";
        static constexpr i32     Width  = 800;
        static constexpr i32     Height = 600;
    }

    namespace Shaders {
        namespace Vertex {
            namespace Attributes {
                static constexpr cstring Position = "a_position";
                static constexpr cstring Texture  = "a_uv";
                static constexpr cstring Normal   = "a_normal";
                static constexpr cstring Color    = "a_color";
            }

            namespace Uniforms {
                static constexpr cstring Model      = "u_model";
                static constexpr cstring View       = "u_view";
                static constexpr cstring Projection = "u_projection";
                static constexpr cstring Color      = "u_color";
            }

            static constexpr cstring Model = R"(
                #version 330 core
                uniform mat4 u_model;
                uniform mat4 u_view;
                uniform mat4 u_projection;

                in vec3 a_position;

                void main(void) {
                    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
                }
            )";

            static constexpr cstring BoundingBox = R"(
                #version 330 core
                uniform mat4 u_model;
                uniform mat4 u_view;
                uniform mat4 u_projection;

                in vec3 a_position;

                void main(void) {
                    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
                }
            )";
        }

        namespace Fragment {
            static constexpr cstring Model = R"(
                #version 330 core

                uniform vec4 u_color;

                out vec4 FragColor;

                void main(void) {
                    FragColor = u_color;
                }
            )";

            static constexpr cstring BoundingBox = R"(
                #version 330 core

                out vec4 FragColor;

                void main(void) {
                    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            )";
        }
    }
}

#endif //FINAL_CONSTANTS_H
