//
// Created by kbratko on 7/29/2023.
//

#ifndef FINAL_ROTATION_H
#define FINAL_ROTATION_H

#include <iomanip>
#include <sstream>

#include "micro-engine/micro.h"

using namespace micro;
using namespace micro::core;
using namespace micro::math;

enum class RotationMode {
    Euler,
    Matrix,
    Quaternion
};

static fn toString(RotationMode mode) -> std::string {
    switch (mode) {
        case RotationMode::Euler:
            return "Euler";
        case RotationMode::Matrix:
            return "Matrix";
        case RotationMode::Quaternion:
            return "Quaternion";
    }
}

struct Rotation {
    RotationMode mode;

    union {
        // Euler
        vector3<f32> compound;

        // Matrix & Quaternion
        struct {
            f32          angle;
            vector3<f32> axis;
        }                simple;
    };

    constexpr explicit Rotation(RotationMode mode);

    constexpr explicit Rotation(RotationMode mode, f32 angle, vector3<f32> axis);

    constexpr explicit Rotation(RotationMode mode, vector3<f32> angles);

    constexpr Rotation(Rotation const &rotation) : mode{rotation.mode} {
        switch (mode) {
            case RotationMode::Euler:
                compound = rotation.compound;
            case RotationMode::Matrix:
            case RotationMode::Quaternion:
                simple = rotation.simple;
        }
    }

    auto operator=(Rotation const &rotation) -> Rotation & {
        mode = rotation.mode;
        switch (mode) {
            case RotationMode::Euler:
                compound = rotation.compound;
            case RotationMode::Matrix:
            case RotationMode::Quaternion:
                simple = rotation.simple;
        }

        return *this;
    }

    [[nodiscard]] auto isZero() const -> bool {
        switch (mode) {
            case RotationMode::Euler:
                return false;
            case RotationMode::Matrix:
            case RotationMode::Quaternion:
                return simple.axis == vector3<f32>{0.f};
        }
    }

    [[nodiscard]] auto toString() const -> std::string;
};

constexpr Rotation::Rotation(RotationMode mode_) {
    mode = mode_;
    switch (mode) {
        default:
        case RotationMode::Euler:
            compound = vector3<f32>{0.f};
            break;
        case RotationMode::Matrix:
        case RotationMode::Quaternion:
            simple.angle = 0.f;
            simple.axis = vector3<f32>{0.f};
            break;
    }
}

constexpr Rotation::Rotation(RotationMode mode_, f32 angle_, vector3<f32> axis_)
    : mode{mode_}, simple{angle_, axis_} {}

constexpr Rotation::Rotation(RotationMode mode_, vector3<f32> angles)
    : mode{mode_}, compound{angles} {}

auto Rotation::toString() const -> std::string {
    std::ostringstream oss{};
    oss << std::fixed;
    switch (mode) {
        default:
        case RotationMode::Euler:
            oss << std::setprecision(2)
                << ::toString(mode)
                << "{ "
                << ".alpha: " << math::degrees(compound.x) << " deg (" << compound.x << " rad)" << ", "
                << ".beta: " << math::degrees(compound.y) << " deg (" << compound.y << " rad)" << ", "
                << ".gamma: " << math::degrees(compound.z) << " deg (" << compound.z << " rad) "
                << "}";
            return oss.str();
        case RotationMode::Matrix:
        case RotationMode::Quaternion:
            auto norm = math::normalize(simple.axis);
            oss << std::setprecision(2)
                << ::toString(mode)
                << "{ "
                << ".angle: " << math::degrees(simple.angle) << " deg (" << simple.angle << " rad)" << ", "
                << std::setprecision(3)
                << ".axis: [" << simple.axis.x << ", " << simple.axis.y << ", " << simple.axis.z << "] "
                << "(norm [" << norm.x << ", " << norm.y << ", " << norm.z << "]) "
                << "}";
            return oss.str();
    }
}

#endif //FINAL_ROTATION_H
