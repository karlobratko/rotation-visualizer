//
// Created by kbratko on 8/16/2023.
//

#ifndef FINAL_PIPELINE_H
#define FINAL_PIPELINE_H

#include "micro-engine/micro.h"

#include "constants.h"
#include "rotation.h"
#include "state.h"

using namespace micro;
using namespace micro::context;

constexpr auto scaleXYZ     = vector3<f32>::from_scalar(1.f);
constexpr auto translateXYZ = vector3<f32>::from_scalar(0.f);

constexpr auto fov      = radians(45.f);
constexpr auto cameraUp = vector3<f32>{0.0f, 1.0f, 0.0f};
constexpr auto zNear    = 0.1f;

auto pipeline(State &state) {
    view.target(color::rgb::Black);

    auto const modelCenter    = state.boundingBox.box.center();
    auto const modelMaxRadius = state.boundingBox.box.maxRadius();

    auto const centerXYZ = -modelCenter;

    auto const cameraZDistance = modelMaxRadius / math::tan(fov / 2.f);
    auto const cameraPosition  = vector3<f32>{0.0f, 0.0f, cameraZDistance};
    auto const cameraTarget    = modelCenter + centerXYZ;

    auto const zFar = cameraZDistance + modelMaxRadius * 2;

    auto const width  = static_cast<f32>(view.viewport[0]);
    auto const height = static_cast<f32>(view.viewport[1]);

    matrix4x4<f32> R = matrix4x4<f32>::identity();
    if (state.ui.model.ready && state.ui.model.boundingBoxReady) {
        auto begin = state.ui.rotation.modeRotations[state.ui.rotation.current.mode].begin();
        auto end   = state.ui.rotation.modeRotations[state.ui.rotation.current.mode].begin();
        if (!state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty())
            std::advance(end, state.ui.rotation.modeRotationIndex + 1);

        state.ui.rotation.timeNs = perf::benchmark<std::chrono::high_resolution_clock, std::chrono::nanoseconds>(
            [&]() {
                switch (state.ui.rotation.current.mode) {
                    case RotationMode::Euler: {
                        auto rotationsAcc = std::accumulate(
                            begin, end,
                            matrix4x4<f32>::identity(),
                            [](matrix4x4<f32> const &m, Rotation const &p) { return matrix4x4<f32>::from_euler(p.compound.x, p.compound.y, p.compound.z) * m; }
                        );
                        R = matrix4x4<f32>::from_euler(state.ui.rotation.current.compound.x,
                                                       state.ui.rotation.current.compound.y,
                                                       state.ui.rotation.current.compound.z) * rotationsAcc;
                        break;
                    }
                    case RotationMode::Matrix: {
                        auto rotationsAcc = std::accumulate(
                            begin, end,
                            matrix4x4<f32>::identity(),
                            [](matrix4x4<f32> const &m, Rotation const &p) { return math::rotate(m, p.simple.angle, p.simple.axis); }
                        );
                        R = state.ui.rotation.current.simple.axis != vector3<f32>{0.f}
                                ? math::rotate(rotationsAcc, state.ui.rotation.current.simple.angle,
                                               state.ui.rotation.current.simple.axis)
                                : rotationsAcc;
                        break;
                    }
                    case RotationMode::Quaternion: {
                        auto rotationsAcc = std::accumulate(
                            begin, end,
                            quaternion<f32>::real(1.f),
                            [](quaternion<f32> const &q, Rotation const &p) { return quaternion<f32>::from_rotation(p.simple.angle, p.simple.axis) * q; }
                        );
                        R = matrix4x4<f32>::from_quaternion(
                            state.ui.rotation.current.simple.axis != vector3<f32>{0.f}
                                ? quaternion<f32>::from_rotation(state.ui.rotation.current.simple.angle,
                                                                 state.ui.rotation.current.simple.axis) * rotationsAcc
                                : rotationsAcc
                        );
                        break;
                    }
                }
            }
        );
    }

    auto const C = translate(matrix4x4<f32>::identity(), centerXYZ);
    auto const S = scale(matrix4x4<f32>::identity(), scaleXYZ);
    auto const T = translate(matrix4x4<f32>::identity(), translateXYZ);
    auto const M = T * R * S * C;

    auto const V = matrix4x4<f32>::lookAt(cameraPosition, cameraTarget, cameraUp);

    auto const P = matrix4x4<f32>::perspective(fov, width > 0.f && height > 0.f ? width / height : 1, zNear, zFar);

    if (state.ui.model.ready && state.ui.model.draw) {
        state.model.shader
             .bind()
             .uniform(Application::Shaders::Vertex::Uniforms::Color, state.model.color)
             .uniform(Application::Shaders::Vertex::Uniforms::Model, M)
             .uniform(Application::Shaders::Vertex::Uniforms::View, V)
             .uniform(Application::Shaders::Vertex::Uniforms::Projection, P);

        state.model.model
             .bind()
             .render()
             .unbind();

        state.model.shader.unbind();
    }

    if (state.ui.model.boundingBoxReady && state.ui.model.drawBoundingBox) {
        state.boundingBox.shader
             .bind()
             .uniform(Application::Shaders::Vertex::Uniforms::Model, M)
             .uniform(Application::Shaders::Vertex::Uniforms::View, V)
             .uniform(Application::Shaders::Vertex::Uniforms::Projection, P);

        Model::polygon(micro::graphics::Model::Face::FrontAndBack, micro::graphics::Model::Polygon::Line);
        state.boundingBox.model
             .bind()
             .render()
             .unbind();
        Model::polygon(micro::graphics::Model::Face::FrontAndBack, micro::graphics::Model::Polygon::Fill);

        state.boundingBox.shader.unbind();
    }
}

#endif //FINAL_PIPELINE_H
