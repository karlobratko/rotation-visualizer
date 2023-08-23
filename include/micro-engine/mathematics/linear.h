//
// Created by kbratko on 7/20/2023.
//

#ifndef MICRO_MATHEMATICS_LINEAR_H
#define MICRO_MATHEMATICS_LINEAR_H

#include <cassert>

#include "../core/types.h"

namespace micro::math {
    template<typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template<typename T>
    concept floating_point = std::is_floating_point_v<T>;

    // vector declaration
    template<core::usize L, arithmetic T>
    struct vector;

    // vector dimensions specializations
    template<arithmetic T> using vector1 = vector<1, T>;
    template<arithmetic T> using vector2 = vector<2, T>;
    template<arithmetic T> using vector3 = vector<3, T>;
    template<arithmetic T> using vector4 = vector<4, T>;

    // matrix declaration
    template<core::usize C, core::usize R, arithmetic T>
    struct matrix;

    // matrix rows/cols specializations
    template<arithmetic T> using matrix2x2 = matrix<2, 2, T>;
    template<arithmetic T> using matrix3x3 = matrix<3, 3, T>;
    template<arithmetic T> using matrix4x4 = matrix<4, 4, T>;

    // quaternion declaration
    template<floating_point T>
    struct quaternion;

    namespace internal::functional {
        template<core::usize L, arithmetic R, arithmetic T>
        struct vector_functor1;

        template<arithmetic R, arithmetic T>
        struct vector_functor1<1, R, T> {
            static fn compute(R (*f)(T), vector<1, T> const &vec) -> vector<1, R> { return {f(vec.x)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor1<2, R, T> {
            static fn compute(R (*f)(T), vector<2, T> const &vec) -> vector<2, R> { return {f(vec.x), f(vec.y)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor1<3, R, T> {
            static fn compute(R (*f)(T), vector<3, T> const &vec) -> vector<3, R> { return {f(vec.x), f(vec.y), f(vec.z)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor1<4, R, T> {
            static fn compute(R (*f)(T), vector<4, T> const &vec) -> vector<4, R> { return {f(vec.x), f(vec.y), f(vec.z), f(vec.w)}; }
        };

        template<core::usize L, arithmetic R, arithmetic T>
        struct vector_functor2;

        template<arithmetic R, arithmetic T>
        struct vector_functor2<1, R, T> {
            static fn compute(R (*f)(T, T), vector<1, T> const &vec1, vector<1, T> const &vec2) -> vector<1, R> { return {f(vec1.x, vec2.x)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor2<2, R, T> {
            static fn compute(R (*f)(T, T), vector<2, T> const &vec1, vector<2, T> const &vec2) -> vector<2, R> { return {f(vec1.x, vec2.x), f(vec1.y, vec2.y)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor2<3, R, T> {
            static fn compute(R (*f)(T, T), vector<3, T> const &vec1, vector<3, T> const &vec2) -> vector<3, R> { return {f(vec1.x, vec2.x), f(vec1.y, vec2.y), f(vec1.z, vec2.z)}; }
        };

        template<arithmetic R, arithmetic T>
        struct vector_functor2<4, R, T> {
            static fn compute(R (*f)(T, T), vector<4, T> const &vec1, vector<4, T> const &vec2) -> vector<4, R> { return {f(vec1.x, vec2.x), f(vec1.y, vec2.y), f(vec1.z, vec2.z), f(vec1.w, vec2.w)}; }
        };
    }

    template<floating_point T>
    fn sqrt(T value) -> T { return std::sqrt(value); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_sqrt {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(sqrt, vec); }
        };
    };

    template<core::usize L, floating_point T>
    fn sqrt(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_sqrt<L, T>::compute(vec); }

    template<floating_point T>
    fn pow(T base, T exponent) -> T { return std::pow(base, exponent); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_pow {
            static fn compute(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return functional::vector_functor2<L, T, T>::compute(pow, vec1, vec2); }
        };
    }

    template<core::usize L, floating_point T>
    fn pow(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return internal::vector_pow<L, T>::compute(vec1, vec2); }

    template<floating_point T>
    fn exp2(T value) -> T { return std::exp2(value); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_exp2 {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(exp2, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn exp2(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_exp2<L, T>::compute(vec); }

    template<floating_point T>
    fn exp(T value) -> T { return std::exp(value); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_exp {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(exp, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn exp(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_exp<L, T>::compute(vec); }

    template<floating_point T>
    fn log2(T value) -> T { return std::log2(value); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_log2 {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(log2, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn log2(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_log2<L, T>::compute(vec); }

    template<floating_point T>
    fn log(T base) -> T { return std::log(base); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_log {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(log, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn log(vector<L, T> const &vec1) -> vector<L, T> { return internal::vector_log<L, T>::compute(vec1); }

    template<arithmetic T>
    constexpr fn abs(T x) -> T { return x < 0 ? -x : x; }

    namespace internal {
        template<core::usize L, arithmetic T>
        struct vector_abs {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(abs, vec); }
        };
    }

    template<core::usize L, arithmetic T>
    fn abs(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_abs<L, T>::compute(vec); }

    template<arithmetic T>
    constexpr fn max(T x, T y) -> T { return x < y ? y : x; }

    template<arithmetic T>
    constexpr fn min(T x, T y) -> T { return y < x ? y : x; }

    namespace internal {
        template<core::usize L, arithmetic T>
        struct vector_max {
            static fn compute(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return functional::vector_functor2<L, T, T>::compute(max, vec1, vec2); }
        };
    }

    template<core::usize L, arithmetic T>
    fn max(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return internal::vector_max<L, T>::compute(vec1, vec2); }

    namespace internal {
        template<core::usize L, arithmetic T>
        struct vector_min {
            static fn compute(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return functional::vector_functor2<L, T, T>::compute(min, vec1, vec2); }
        };
    }

    template<core::usize L, arithmetic T>
    fn min(vector<L, T> const &vec1, vector<L, T> const &vec2) -> vector<L, T> { return internal::vector_min<L, T>::compute(vec1, vec2); }

    template<floating_point T>
    fn floor(T val) -> T { return std::floor(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_floor {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(floor, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn floor(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_floor<L, T>::compute(vec); }

    template<floating_point T>
    fn ceil(T val) -> T { return std::ceil(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_ceil {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(ceil, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn ceil(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_ceil<L, T>::compute(vec); }

    template<floating_point T>
    fn trunc(T val) -> T { return std::trunc(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_trunc {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(trunc, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn trunc(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_trunc<L, T>::compute(vec); }

    template<floating_point T>
    fn round(T val) -> T { return std::round(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_round {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(round, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn round(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_round<L, T>::compute(vec); }

    template<floating_point T>
    fn sin(T val) -> T { return std::sin(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_sin {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(sin, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn sin(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_sin<L, T>::compute(vec); }

    template<floating_point T>
    fn cos(T val) -> T { return std::cos(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_cos {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(cos, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn cos(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_cos<L, T>::compute(vec); }

    template<floating_point T>
    fn tan(T val) -> T { return std::tan(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_tan {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(tan, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn tan(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_tan<L, T>::compute(vec); }

    template<floating_point T>
    fn asin(T val) -> T { return std::asin(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_asin {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(asin, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn asin(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_asin<L, T>::compute(vec); }

    template<floating_point T>
    fn acos(T val) -> T { return std::acos(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_acos {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(acos, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn acos(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_acos<L, T>::compute(vec); }

    template<floating_point T>
    fn atan(T val) -> T { return std::atan(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_atan {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(atan, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn atan(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_atan<L, T>::compute(vec); }

    template<floating_point T>
    fn sinh(T val) -> T { return std::sinh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_sinh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(sinh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn sinh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_sinh<L, T>::compute(vec); }

    template<floating_point T>
    fn cosh(T val) -> T { return std::cosh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_cosh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(cosh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn cosh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_cosh<L, T>::compute(vec); }

    template<floating_point T>
    fn tanh(T val) -> T { return std::tanh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_tanh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(tanh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn tanh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_tanh<L, T>::compute(vec); }

    template<floating_point T>
    fn asinh(T val) -> T { return std::asinh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_asinh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(asinh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn asinh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_asinh<L, T>::compute(vec); }

    template<floating_point T>
    fn acosh(T val) -> T { return std::acosh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_acosh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(acosh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn acosh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_acosh<L, T>::compute(vec); }

    template<floating_point T>
    fn atanh(T val) -> T { return std::atanh(val); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_atanh {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(atanh, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn atanh(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_atanh<L, T>::compute(vec); }

    template<floating_point T>
    constexpr fn radians(T degrees) -> T { return degrees * static_cast<T>(0.01745329251994329576923690768489); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_radians {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(radians, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn radians(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_radians<L, T>::compute(vec); }

    template<floating_point T>
    constexpr fn degrees(T radians) -> T { return radians * static_cast<T>(57.295779513082320876798154814105); }

    namespace internal {
        template<core::usize L, floating_point T>
        struct vector_degrees {
            static fn compute(vector<L, T> const &vec) -> vector<L, T> { return functional::vector_functor1<L, T, T>::compute(degrees, vec); }
        };
    }

    template<core::usize L, floating_point T>
    fn degrees(vector<L, T> const &vec) -> vector<L, T> { return internal::vector_degrees<L, T>::compute(vec); }

    namespace internal {
        template<core::usize L, arithmetic T>
        struct vector_dot;

        template<arithmetic T>
        struct vector_dot<1, T> {
            static constexpr fn compute(vector<1, T> const &a, vector<1, T> const &b) -> T { return a.x * b.x; }
        };

        template<arithmetic T>
        struct vector_dot<2, T> {
            static constexpr fn compute(vector<2, T> const &a, vector<2, T> const &b) -> T { return a.x * b.x + a.y * b.y; }
        };

        template<arithmetic T>
        struct vector_dot<3, T> {
            static constexpr fn compute(vector<3, T> const &a, vector<3, T> const &b) -> T { return a.x * b.x + a.y * b.y + a.z * b.z; }
        };

        template<arithmetic T>
        struct vector_dot<4, T> {
            static constexpr fn compute(vector<4, T> const &a, vector<4, T> const &b) -> T { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
        };
    }

    template<core::usize L, arithmetic T>
    constexpr fn dot(vector<L, T> const &x, vector<L, T> const &y) -> T { return internal::vector_dot<L, T>::compute(x, y); }

    template<core::usize L, floating_point T>
    fn magnitude(vector<L, T> const &vec) -> T { return sqrt(dot(vec, vec)); }

    template<core::usize L, floating_point T>
    fn distance(vector<L, T> const &vec1, vector<L, T> const &vec2) -> T { return magnitude(vec2 - vec1); }

    namespace internal {
        template<core::usize L, arithmetic T>
        struct vector_cross;

        template<arithmetic T>
        struct vector_cross<3, T> {
            static constexpr fn compute(vector<3, T> const &a, vector<3, T> const &b) -> vector<3, T> {
                return {
                    a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x
                };
            }
        };
    }

    template<core::usize L, arithmetic T>
    constexpr fn cross(vector<L, T> const &x, vector<L, T> const &y) -> vector<3, T> { return internal::vector_cross<L, T>::compute(x, y); }

    template<core::usize L, floating_point T>
    fn normalize(vector<L, T> const &vec) -> vector<L, T> { return vec / magnitude(vec); }

    template<core::usize L, arithmetic T>
    constexpr fn faceforward(vector<L, T> const &to_orient, vector<L, T> const &incident,
                             vector<L, T> const &normal) -> vector<L, T> { return dot(normal, incident) < static_cast<T>(0) ? to_orient : -to_orient; }

    template<core::usize L, arithmetic T>
    constexpr fn reflect(vector<L, T> const &to_reflect, vector<L, T> const &normal) -> vector<L, T> { return to_reflect - normal * dot(normal, to_reflect) * static_cast<T>(2); }

    template<core::usize L, arithmetic T>
    fn refract(vector<L, T> const &incident, vector<L, T> const &normal, T eta) -> vector<L, T> {
        auto dotVal = dot(normal, incident);
        auto k      = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - dotVal * dotVal);

        return k >= static_cast<T>(0)
                   ? eta * incident - (eta * dotVal + sqrt(k)) * normal
                   : vector<L, T>{0};
    }

    template<core::usize L, arithmetic T>
    constexpr fn clamp(vector<L, T> const &vec, vector<L, T> const &min_val, vector<L, T> const &max_val) -> vector<L, T> { return min(max(vec, min_val), max_val); }

    // vector1 definition
    template<arithmetic T>
    struct vector<1, T> {
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type dimensions = 1;

        using type = vector<dimensions, value_type>;

        // members and member access operators
        union {
            value_type x, r, s;
        };

        constexpr fn operator[](size_type i) -> value_type & {
            assert(i >= 0 && i < this->dimensions);
            return x;
        }

        constexpr fn operator[](size_type i) const -> value_type const & {
            assert(i >= 0 && i < this->dimensions);
            return x;
        }

        // constructors
        constexpr vector() = default;

        constexpr vector(type const &vec) : x{vec.x} {};

        constexpr explicit vector(value_type scalar) : x{scalar} {}

        constexpr explicit vector(vector<2, value_type> const &vec) : x{vec.x} {}

        constexpr explicit vector(vector<3, value_type> const &vec) : x{vec.x} {}

        constexpr explicit vector(vector<4, value_type> const &vec) : x{vec.x} {}

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_vector2(vector<2, value_type> const &vec) -> type { return type{vec}; }

        static constexpr fn from_vector3(vector<3, value_type> const &vec) -> type { return type{vec}; }

        static constexpr fn from_vector4(vector<4, value_type> const &vec) -> type { return type{vec}; }

        // unary operators
        constexpr fn operator=(type const &vec) -> type & {
            x = vec.x;
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-(this->x)}; }

        constexpr fn operator+=(type const &vec) -> type & {
            this->x += vec.x;
            return *this;
        }

        constexpr fn operator-=(type const &vec) -> type & {
            this->x -= vec.x;
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->x *= scalar;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->x /= scalar;
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->x);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->x);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type vec{*this};
            ++(*this);
            return vec;
        }

        constexpr fn operator--(int) -> type {
            type vec{*this};
            --(*this);
            return vec;
        }

        // binary operators
        constexpr fn operator+(type const &vec) const -> type { return {this->x + vec.x}; }

        constexpr fn operator-(type const &vec) const -> type { return {this->x - vec.x}; }

        constexpr fn operator*(value_type scalar) const -> type { return {this->x * scalar}; }

        constexpr fn operator/(value_type scalar) const -> type { return {this->x / scalar}; }

        // boolean operators
        constexpr fn operator==(type const &vec) const -> bool { return this->x == vec.x; }

        constexpr fn operator!=(type const &vec) const -> bool { return this->x != vec.x; }
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, vector<1, T> const &vec) -> vector<1, T> { return {vec.x * scalar}; }

    // vector2 definition
    template<arithmetic T>
    struct vector<2, T> {
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type dimensions = 2;

        using type = vector<dimensions, value_type>;

        // members and member access operators
        union {
            value_type x, r, s;
        };

        union {
            value_type y, g, t;
        };

        constexpr fn operator[](size_type i) -> value_type & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
            }
        }

        constexpr fn operator[](size_type i) const -> value_type const & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
            }
        }

        // constructors
        constexpr vector() = default;

        constexpr vector(type const &vec) : x{vec.x}, y{vec.y} {}

        constexpr explicit vector(value_type scalar) : x{scalar}, y{scalar} {}

        constexpr vector(value_type a, value_type b) : x{a}, y{b} {}

        constexpr vector(vector<1, value_type> const &vec, value_type scalar) : x{vec.x}, y{scalar} {}

        constexpr vector(value_type scalar, vector<1, value_type> const &vec) : x{scalar}, y{vec.x} {}

        constexpr vector(vector<1, value_type> const &vec1, vector<1, value_type> const &vec2)
            : x{vec1.x}, y{vec2.x} {}

        constexpr explicit vector(vector<3, value_type> const &vec) : x{vec.x}, y{vec.y} {}

        constexpr explicit vector(vector<4, value_type> const &vec) : x{vec.x}, y{vec.y} {}

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type a, value_type b) -> type { return type{a, b}; }

        static constexpr fn from_vector3(vector<3, value_type> const &vec) -> type { return type{vec}; }

        static constexpr fn from_vector4(vector<4, value_type> const &vec) -> type { return type{vec}; }

        // unary operators
        constexpr fn operator=(type const &vec) -> type & {
            x = vec.x;
            y = vec.y;
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-(this->x), -(this->y)}; }

        constexpr fn operator+=(type const &vec) -> type & {
            this->x += vec.x;
            this->y += vec.y;
            return *this;
        }

        constexpr fn operator-=(type const &vec) -> type & {
            this->x -= vec.x;
            this->y -= vec.y;
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->x);
            ++(this->y);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->x);
            --(this->y);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type vec{*this};
            ++(*this);
            return vec;
        }

        constexpr fn operator--(int) -> type {
            type vec{*this};
            --(*this);
            return vec;
        }

        // binary operators
        constexpr fn operator+(type const &vec) const -> type { return {this->x + vec.x, this->y + vec.y}; }

        constexpr fn operator-(type const &vec) const -> type { return {this->x - vec.x, this->y - vec.y}; }

        constexpr fn operator*(value_type scalar) const -> type { return {this->x * scalar, this->y * scalar}; }

        constexpr fn operator/(value_type scalar) const -> type { return {this->x / scalar, this->y / scalar}; }

        // boolean operators
        constexpr fn operator==(type const &vec) const -> bool { return this->x == vec.x && this->y == vec.y; }

        constexpr fn operator!=(type const &vec) const -> bool { return this->x != vec.x || this->y != vec.y; }
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, vector<2, T> const &vec) -> vector<2, T> { return {vec.x * scalar, vec.y * scalar}; }

    // vector3 definition
    template<arithmetic T>
    struct vector<3, T> {
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type dimensions = 3;

        using type = vector<dimensions, value_type>;

        // members and member access operators
        union {
            value_type x, r, s;
        };

        union {
            value_type y, g, t;
        };

        union {
            value_type z, b, p;
        };

        constexpr fn operator[](size_type i) -> value_type & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
            }
        }

        constexpr fn operator[](size_type i) const -> value_type const & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
            }
        }

        // constructors
        constexpr vector() = default;

        constexpr vector(type const &vec) : x{vec.x}, y{vec.y}, z{vec.z} {}

        constexpr explicit vector(value_type scalar) : x{scalar}, y{scalar}, z{scalar} {}

        constexpr explicit vector(vector<1, value_type> const &vec) : x{vec.x}, y{vec.x}, z{vec.x} {}

        constexpr vector(value_type a, value_type b, value_type c) : x{a}, y{b}, z{c} {}

        constexpr vector(value_type a, value_type b, vector<1, value_type> const &vec)
            : x{a}, y{b}, z{vec.x} {}

        constexpr vector(value_type a, vector<1, value_type> const &vec, value_type b)
            : x{a}, y{vec.x}, z{b} {}

        constexpr vector(vector<1, value_type> const &vec, value_type a, value_type b)
            : x{vec.x}, y{a}, z{b} {}

        constexpr vector(vector<1, value_type> const &vec1, vector<1, value_type> const &vec2, value_type a)
            : x{vec1.x}, y{vec2.x}, z{a} {}

        constexpr vector(vector<1, value_type> const &vec1, value_type a, vector<1, value_type> const &vec2)
            : x{vec1.x}, y{a}, z{vec2.x} {}

        constexpr vector(value_type a, vector<1, value_type> const &vec1, vector<1, value_type> const &vec2)
            : x{a}, y{vec1.x}, z{vec2.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3)
            : x{vec1.x}, y{vec2.x}, z{vec3.x} {}

        constexpr vector(vector<2, value_type> const &vec, value_type scalar)
            : x{vec.x}, y{vec.y}, z{scalar} {}

        constexpr vector(vector<2, value_type> const &vec1, vector<1, value_type> const &vec2)
            : x{vec1.x}, y{vec1.y}, z{vec2.x} {}

        constexpr vector(value_type scalar, vector<2, value_type> const &vec)
            : x{scalar}, y{vec.x}, z{vec.y} {}

        constexpr vector(vector<1, value_type> const &vec1, vector<2, value_type> const &vec2)
            : x{vec1.x}, y{vec2.x}, z{vec2.y} {}

        constexpr explicit vector(vector<4, value_type> const &vec) : x{vec.x}, y{vec.y}, z{vec.z} {}

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type a, value_type b, value_type c) -> type { return type{a, b, c}; }

        static constexpr fn from_vector4(vector<4, value_type> const &vec) -> type { return type{vec}; }

        template<floating_point U>
        static constexpr fn from_quaternion(quaternion<U> const &quat) -> type { return type{quat.x, quat.y, quat.z}; }

        template<floating_point U>
        static constexpr fn from_euler(quaternion<U> const &quat) -> type {
            return type{
                asin(
                    clamp(static_cast<T>(-2) * (quat.x * quat.z - quat.w * quat.y), static_cast<T>(-1),
                          static_cast<T>(1))),
                static_cast<T>(atan(static_cast<T>(2) * (quat.y * quat.z + quat.w * quat.x),
                                    quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z)),
                static_cast<T>(atan(static_cast<T>(2) * (quat.x * quat.y + quat.w * quat.z),
                                    quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z))
            };
        }

        // unary operators
        constexpr fn operator=(type const &vec) -> type & {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-(this->x), -(this->y), -(this->z)}; }

        constexpr fn operator+=(type const &vec) -> type & {
            this->x += vec.x;
            this->y += vec.y;
            this->z += vec.z;
            return *this;
        }

        constexpr fn operator-=(type const &vec) -> type & {
            this->x -= vec.x;
            this->y -= vec.y;
            this->z -= vec.z;
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->x *= scalar;
            this->y *= scalar;
            this->z *= scalar;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->x /= scalar;
            this->y /= scalar;
            this->z /= scalar;
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->x);
            ++(this->y);
            ++(this->z);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->x);
            --(this->y);
            --(this->z);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type vec{*this};
            ++(*this);
            return vec;
        }

        constexpr fn operator--(int) -> type {
            type vec{*this};
            --(*this);
            return vec;
        }

        // binary operators
        constexpr fn operator+(type const &vec) const -> type { return {this->x + vec.x, this->y + vec.y, this->z + vec.z}; }

        constexpr fn operator-(type const &vec) const -> type { return {this->x - vec.x, this->y - vec.y, this->z - vec.z}; }

        constexpr fn operator*(value_type scalar) const -> type { return {this->x * scalar, this->y * scalar, this->z * scalar}; }

        constexpr fn operator/(value_type scalar) const -> type { return {this->x / scalar, this->y / scalar, this->z / scalar}; }

        // boolean operators
        constexpr fn operator==(type const &vec) const -> bool { return this->x == vec.x && this->y == vec.y && this->z == vec.z; }

        constexpr fn operator!=(type const &vec) const -> bool { return this->x != vec.x || this->y != vec.y || this->z != vec.z; }
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, vector<3, T> const &vec) -> vector<3, T> { return {vec.x * scalar, vec.y * scalar, vec.z * scalar}; }

    // vector4 definition
    template<arithmetic T>
    struct vector<4, T> {
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type dimensions = 4;

        using type = vector<dimensions, value_type>;

        // members and member access operators
        union {
            value_type x, r, s;
        };

        union {
            value_type y, g, t;
        };

        union {
            value_type z, b, p;
        };

        union {
            value_type w, a, q;
        };

        constexpr fn operator[](size_type i) -> value_type & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                case 3:
                    return w;
            }
        }

        constexpr fn operator[](size_type i) const -> value_type const & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                case 3:
                    return w;
            }
        }

        // constructors
        constexpr vector() = default;

        constexpr vector(type const &vec) : x{vec.x}, y{vec.y}, z{vec.z}, w{vec.w} {}

        constexpr explicit vector(value_type scalar) : x{scalar}, y{scalar}, z{scalar}, w{scalar} {}

        constexpr explicit vector(vector<1, value_type> const &vec) : x{vec.x}, y{vec.x}, z{vec.x}, w{vec.x} {}

        constexpr vector(value_type a, value_type b, value_type c, value_type d) : x{a}, y{b}, z{c}, w{d} {}

        constexpr vector(value_type a, value_type b, value_type c, vector<1, value_type> const &vec)
            : x{a}, y{b}, z{c}, w{vec.x} {}

        constexpr vector(value_type a, value_type b, vector<1, value_type> const &vec, value_type c)
            : x{a}, y{b}, z{vec.x}, w{c} {}

        constexpr vector(value_type a, vector<1, value_type> const &vec, value_type b, value_type c)
            : x{a}, y{vec.x}, z{b}, w{c} {}

        constexpr vector(vector<1, value_type> const &vec, value_type a, value_type b, value_type c)
            : x{vec.x}, y{a}, z{b}, w{c} {}

        constexpr vector(value_type                   a,
                         value_type                   b,
                         vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2)
            : x{a}, y{b}, z{vec1.x}, w{vec2.x} {}

        constexpr vector(value_type                   a,
                         vector<1, value_type> const &vec1,
                         value_type                   b,
                         vector<1, value_type> const &vec2)
            : x{a}, y{vec1.x}, z{b}, w{vec2.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         value_type                   a,
                         value_type                   b,
                         vector<1, value_type> const &vec2)
            : x{vec1.x}, y{a}, z{b}, w{vec2.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         value_type                   a,
                         vector<1, value_type> const &vec2,
                         value_type                   b)
            : x{vec1.x}, y{a}, z{vec2.x}, w{b} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         value_type                   a,
                         value_type                   b)
            : x{vec1.x}, y{vec2.x}, z{a}, w{b} {}

        constexpr vector(value_type                   a,
                         vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         value_type                   b)
            : x{a}, y{vec1.x}, z{vec2.x}, w{b} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3,
                         value_type                   a)
            : x{vec1.x}, y{vec2.x}, z{vec3.x}, w{a} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         value_type                   a,
                         vector<1, value_type> const &vec3)
            : x{vec1.x}, y{vec2.x}, z{a}, w{vec3.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         value_type                   a,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3)
            : x{vec1.x}, y{a}, z{vec2.x}, w{vec3.x} {}

        constexpr vector(value_type                   a,
                         vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3)
            : x{a}, y{vec1.x}, z{vec2.x}, w{vec3.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3,
                         vector<1, value_type> const &vec4)
            : x{vec1.x}, y{vec2.x}, z{vec3.x}, w{vec4.x} {}

        constexpr vector(vector<2, value_type> const &vec, value_type a, value_type b)
            : x{vec.x}, y{vec.y}, z{a}, w{b} {}

        constexpr vector(vector<2, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         value_type                   a)
            : x{vec1.x}, y{vec1.y}, z{vec2.x}, w{a} {}

        constexpr vector(vector<2, value_type> const &vec1,
                         value_type                   a,
                         vector<1, value_type> const &vec2)
            : x{vec1.x}, y{vec1.y}, z{a}, w{vec2.x} {}

        constexpr vector(vector<2, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<1, value_type> const &vec3)
            : x{vec1.x}, y{vec1.y}, z{vec2.x}, w{vec3.x} {}

        constexpr vector(value_type a, vector<2, value_type> const &vec, value_type b)
            : x{a}, y{vec.x}, z{vec.y}, w{b} {}

        constexpr vector(value_type                   a,
                         vector<2, value_type> const &vec1,
                         vector<1, value_type> const &vec2)
            : x{a}, y{vec1.x}, z{vec1.y}, w{vec2.x} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<2, value_type> const &vec2,
                         value_type                   a)
            : x{vec1.x}, y{vec2.x}, z{vec2.y}, w{a} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<2, value_type> const &vec2,
                         vector<1, value_type> const &vec3)
            : x{vec1.x}, y{vec2.x}, z{vec2.y}, w{vec3.x} {}

        constexpr vector(value_type a, value_type b, vector<2, value_type> const &vec)
            : x{a}, y{b}, z{vec.x}, w{vec.y} {}

        constexpr vector(value_type                   a,
                         vector<1, value_type> const &vec1,
                         vector<2, value_type> const &vec2)
            : x{a}, y{vec1.x}, z{vec2.x}, w{vec2.y} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         value_type                   a,
                         vector<2, value_type> const &vec2)
            : x{vec1.x}, y{a}, z{vec2.x}, w{vec2.y} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<1, value_type> const &vec2,
                         vector<2, value_type> const &vec3)
            : x{vec1.x}, y{vec2.x}, z{vec3.x}, w{vec3.y} {}

        constexpr vector(vector<2, value_type> const &vec1,
                         vector<2, value_type> const &vec2)
            : x{vec1.x}, y{vec1.y}, z{vec2.x}, w{vec2.y} {}

        constexpr vector(vector<3, value_type> const &vec,
                         value_type                   a)
            : x{vec.x}, y{vec.y}, z{vec.z}, w{a} {}

        constexpr vector(vector<3, value_type> const &vec1,
                         vector<1, value_type> const &vec2)
            : x{vec1.x}, y{vec1.y}, z{vec1.z}, w{vec2.x} {}

        constexpr vector(value_type                   a,
                         vector<3, value_type> const &vec)
            : x{a}, y{vec.x}, z{vec.y}, w{vec.z} {}

        constexpr vector(vector<1, value_type> const &vec1,
                         vector<3, value_type> const &vec2)
            : x{vec1.x}, y{vec2.x}, z{vec2.y}, w{vec2.z} {}

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type a, value_type b, value_type c, value_type d) -> type { return type{a, b, c, c}; }

        // unary operators
        constexpr fn operator=(type const &vec) -> type & {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            w = vec.w;
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-(this->x), -(this->y), -(this->z), -(this->w)}; }

        constexpr fn operator+=(type const &vec) -> type & {
            this->x += vec.x;
            this->y += vec.y;
            this->z += vec.z;
            this->w += vec.w;
            return *this;
        }

        constexpr fn operator-=(type const &vec) -> type & {
            this->x -= vec.x;
            this->y -= vec.y;
            this->z -= vec.z;
            this->w -= vec.w;
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->x *= scalar;
            this->y *= scalar;
            this->z *= scalar;
            this->w *= scalar;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->x /= scalar;
            this->y /= scalar;
            this->z /= scalar;
            this->w /= scalar;
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->x);
            ++(this->y);
            ++(this->z);
            ++(this->w);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->x);
            --(this->y);
            --(this->z);
            --(this->w);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type vec{*this};
            ++(*this);
            return vec;
        }

        constexpr fn operator--(int) -> type {
            type vec{*this};
            --(*this);
            return vec;
        }

        // binary operators
        constexpr fn operator+(type const &vec) const -> type { return {this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w}; }

        constexpr fn operator-(type const &vec) const -> type { return {this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w}; }

        constexpr fn operator*(value_type scalar) const -> type { return {this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar}; }

        constexpr fn operator/(value_type scalar) const -> type { return {this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar}; }

        // boolean operators
        constexpr fn operator==(type const &vec) const -> bool { return this->x == vec.x && this->y == vec.y && this->z == vec.z && this->w == vec.w; }

        constexpr fn operator!=(type const &vec) const -> bool { return this->x != vec.x || this->y != vec.y || this->z != vec.z || this->w != vec.w; }
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, vector<4, T> const &vec) -> vector<4, T> { return {vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar}; }

    namespace internal {
        template<core::usize C, core::usize R, arithmetic T>
        struct matrix_transpose;

        template<arithmetic T>
        struct matrix_transpose<2, 2, T> {
            static constexpr fn compute(matrix<2, 2, T> const &mat) -> matrix<2, 2, T> { return {mat[0][0], mat[1][0], mat[0][1], mat[1][1]}; }
        };

        template<arithmetic T>
        struct matrix_transpose<3, 3, T> {
            static constexpr fn compute(matrix<3, 3, T> const &mat) -> matrix<3, 3, T> {
                return {
                    mat[0][0],
                    mat[1][0],
                    mat[2][0],
                    mat[0][1],
                    mat[1][1],
                    mat[2][1],
                    mat[0][2],
                    mat[1][2],
                    mat[2][2],
                };
            }
        };

        template<arithmetic T>
        struct matrix_transpose<4, 4, T> {
            static constexpr fn compute(matrix<3, 3, T> const &mat) -> matrix<3, 3, T> {
                return {
                    mat[0][0],
                    mat[1][0],
                    mat[2][0],
                    mat[3][0],
                    mat[0][1],
                    mat[1][1],
                    mat[2][1],
                    mat[3][1],
                    mat[0][2],
                    mat[1][2],
                    mat[2][2],
                    mat[3][2],
                    mat[0][3],
                    mat[1][3],
                    mat[2][3],
                    mat[3][3]
                };
            }
        };
    }

    template<core::usize C, core::usize R, arithmetic T>
    constexpr fn transpose(matrix<C, R, T> const &mat) -> matrix<C, R, T> { return internal::matrix_transpose<C, R, T>::compute(mat); }

    namespace internal {
        template<core::usize C, core::usize R, arithmetic T>
        struct matrix_determinant;

        template<arithmetic T>
        struct matrix_determinant<2, 2, T> {
            static constexpr fn compute(matrix<2, 2, T> const &mat) -> T { return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]; }
        };

        template<arithmetic T>
        struct matrix_determinant<3, 3, T> {
            static constexpr fn compute(matrix<3, 3, T> const &mat) -> T {
                return mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
                       mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) +
                       mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]);
            }
        };

        template<arithmetic T>
        struct matrix_determinant<4, 4, T> {
            static constexpr fn compute(matrix<4, 4, T> const &mat) -> T {
                auto det00of2x2 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
                auto det01of2x2 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
                auto det02of2x2 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
                auto det03of2x2 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
                auto det04of2x2 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
                auto det05of2x2 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];

                return
                    mat[0][0] * (mat[1][1] * det00of2x2 - mat[1][2] * det01of2x2 + mat[1][3] * det02of2x2) -
                    mat[0][1] * (mat[1][0] * det00of2x2 - mat[1][2] * det03of2x2 + mat[1][3] * det04of2x2) +
                    mat[0][2] * (mat[1][0] * det01of2x2 - mat[1][1] * det03of2x2 + mat[1][3] * det05of2x2) -
                    mat[0][3] * (mat[1][0] * det02of2x2 - mat[1][1] * det04of2x2 + mat[1][2] * det05of2x2);
            }
        };
    }

    template<core::usize C, core::usize R, arithmetic T>
    constexpr fn determinant(matrix<C, R, T> const &mat) -> T { return internal::matrix_determinant<C, R, T>::compute(mat); }

    namespace internal {
        template<core::usize C, core::usize R, floating_point T>
        struct matrix_inverse;

        template<floating_point T>
        struct matrix_inverse<2, 2, T> {
            static constexpr fn compute(matrix<2, 2, T> const &mat) -> matrix<2, 2, T> {
                auto det = determinant(mat);

                return {
                    mat[1][1] / det,
                    -(mat[0][1]) / det,
                    -(mat[1][0]) / det,
                    mat[0][0] / det
                };
            }
        };

        template<floating_point T>
        struct matrix_inverse<3, 3, T> {
            static constexpr fn compute(matrix<3, 3, T> const &mat) -> matrix<3, 3, T> {
                auto det = determinant(mat);

                auto A00 = mat[0][0];
                auto A01 = mat[0][1];
                auto A02 = mat[0][2];
                auto A10 = mat[1][0];
                auto A11 = mat[1][1];
                auto A12 = mat[1][2];
                auto A20 = mat[2][0];
                auto A21 = mat[2][1];
                auto A22 = mat[2][2];

                return {
                    (A11 * A22 - A21 * A12) * det,
                    -(A01 * A22 - A21 * A02) * det,
                    (A01 * A12 - A11 * A02) * det,
                    -(A10 * A22 - A20 * A12) * det,
                    (A00 * A22 - A20 * A02) * det,
                    -(A00 * A12 - A10 * A02) * det,
                    (A10 * A21 - A20 * A11) * det,
                    -(A00 * A21 - A20 * A01) * det,
                    (A00 * A11 - A10 * A01) * det
                };
            }
        };

        template<floating_point T>
        class matrix_inverse<4, 4, T> {
        private:
            static constexpr fn vector4_product(vector<4, T> const &a, vector<4, T> const &b) -> vector<4, T> { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }

        public:
            static constexpr fn compute(matrix<4, 4, T> const &mat) -> matrix<4, 4, T> {
                auto det00of2x2 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
                auto det01of2x2 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
                auto det02of2x2 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];

                auto det03of2x2 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
                auto det04of2x2 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
                auto det05of2x2 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];

                auto det06of2x2 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
                auto det07of2x2 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
                auto det08of2x2 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];

                auto det09of2x2 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
                auto det10of2x2 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
                auto det11of2x2 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];

                auto det12of2x2 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
                auto det13of2x2 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
                auto det14of2x2 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];

                auto det15of2x2 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
                auto det16of2x2 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
                auto det17of2x2 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

                vector<4, T> fac0{det00of2x2, det00of2x2, det01of2x2, det02of2x2};
                vector<4, T> fac1{det03of2x2, det03of2x2, det04of2x2, det05of2x2};
                vector<4, T> fac2{det06of2x2, det06of2x2, det07of2x2, det08of2x2};
                vector<4, T> fac3{det09of2x2, det09of2x2, det10of2x2, det11of2x2};
                vector<4, T> fac4{det12of2x2, det12of2x2, det13of2x2, det14of2x2};
                vector<4, T> fac5{det15of2x2, det15of2x2, det16of2x2, det17of2x2};

                vector<4, T> vec0{mat[1][0], mat[0][0], mat[0][0], mat[0][0]};
                vector<4, T> vec1{mat[1][1], mat[0][1], mat[0][1], mat[0][1]};
                vector<4, T> vec2{mat[1][2], mat[0][2], mat[0][2], mat[0][2]};
                vector<4, T> vec3{mat[1][3], mat[0][3], mat[0][3], mat[0][3]};

                vector<4, T> inv0{
                    vector4_product(vec1, fac0) -
                    vector4_product(vec2, fac1) +
                    vector4_product(vec3, fac2)
                };
                vector<4, T> inv1{
                    vector4_product(vec0, fac0) -
                    vector4_product(vec2, fac3) +
                    vector4_product(vec3, fac4)
                };
                vector<4, T> inv2{
                    vector4_product(vec0, fac1) -
                    vector4_product(vec1, fac3) +
                    vector4_product(vec3, fac5)
                };
                vector<4, T> inv3{
                    vector4_product(vec0, fac2) -
                    vector4_product(vec1, fac4) +
                    vector4_product(vec2, fac5)
                };

                vector<4, T>    signA{+1, -1, +1, -1};
                vector<4, T>    signB{-1, +1, -1, +1};
                matrix<4, 4, T> imat{
                    vector4_product(inv0, signA),
                    vector4_product(inv1, signB),
                    vector4_product(inv2, signA),
                    vector4_product(inv3, signB)
                };

                vector<4, T> row0(imat[0][0], imat[1][0], imat[2][0], imat[3][0]);

                vector<4, T> dot0(vector4_product(mat[0], row0));
                auto         det = (dot0.x + dot0.y) + (dot0.z + dot0.w);

                return imat / det;
            }
        };
    }

    template<core::usize C, core::usize R, floating_point T>
    constexpr fn inverse(matrix<C, R, T> const &mat) -> matrix<C, R, T> { return internal::matrix_inverse<C, R, T>::compute(mat); }

    namespace internal {
        template<core::usize C, core::usize R, typename T>
        struct quaternion_to_matrix;

        template<typename T>
        struct quaternion_to_matrix<3, 3, T> {
            static constexpr fn compute(quaternion<T> const &quat) -> matrix<3, 3, T> {
                auto sx = quat.s * quat.x;
                auto sy = quat.s * quat.y;
                auto sz = quat.s * quat.z;
                auto xy = quat.x * quat.y;
                auto yz = quat.y * quat.z;
                auto zx = quat.z * quat.x;
                auto xx = quat.x * quat.x;
                auto yy = quat.y * quat.y;
                auto zz = quat.z * quat.z;

                return matrix<3, 3, T>{
                    T(1) - T(2) * (yy + zz),
                    T(2) * (xy + sz),
                    T(2) * (zx - sy),
                    T(2) * (xy - sz),
                    T(1) - T(2) * (xx + zz),
                    T(2) * (yz + sx),
                    T(2) * (zx + sy),
                    T(2) * (yz - sx),
                    T(1) - T(2) * (xx + yy)
                };
            }
        };

        template<typename T>
        struct quaternion_to_matrix<4, 4, T> {
            static constexpr fn compute(quaternion<T> const &quat) -> matrix<4, 4, T> { return matrix<4, 4, T>{quaternion_to_matrix<3, 3, T>::compute(quat)}; }
        };
    }

    template<floating_point T>
    constexpr fn rotate(matrix<4, 4, T> const &mat, T angle, vector<3, T> const &vec) -> matrix<4, 4, T> {
        auto c = cos(angle);
        auto s = sin(angle);

        vector<3, T> axis{normalize(vec)};
        vector<3, T> temp{(static_cast<T>(1) - c) * axis};

        matrix<4, 4, T> rot{};
        rot[0][0] = c + temp[0] * axis[0];
        rot[0][1] = temp[0] * axis[1] + s * axis[2];
        rot[0][2] = temp[0] * axis[2] - s * axis[1];

        rot[1][0] = temp[1] * axis[0] - s * axis[2];
        rot[1][1] = c + temp[1] * axis[1];
        rot[1][2] = temp[1] * axis[2] + s * axis[0];

        rot[2][0] = temp[2] * axis[0] + s * axis[1];
        rot[2][1] = temp[2] * axis[1] - s * axis[0];
        rot[2][2] = c + temp[2] * axis[2];

        matrix<4, 4, T> res{};
        res[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
        res[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
        res[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
        res[3] = mat[3];

        return res;
    }

    template<arithmetic T>
    constexpr fn translate(matrix<4, 4, T> const &mat, vector<3, T> const &vec) -> matrix<4, 4, T> {
        matrix<4, 4, T> res{mat};
        res[3] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3];
        return res;
    }

    template<typename T>
    constexpr fn scale(matrix<4, 4, T> const &mat, vector<3, T> const &vec) -> matrix<4, 4, T> {
        return {
            mat[0] * vec[0],
            mat[1] * vec[1],
            mat[2] * vec[2],
            mat[3]
        };
    }

    // matrix2x2 definition
    template<arithmetic T>
    class matrix<2, 2, T> {
    public:
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type columns = 2;
        static constexpr size_type rows    = 2;

        using column_type = vector<rows, value_type>;
        using row_type = vector<columns, value_type>;

        using type = matrix<columns, rows, value_type>;

        // member access operators
        constexpr fn operator[](size_type i) -> column_type & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        constexpr fn operator[](size_type i) const -> column_type const & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        // constructors
        constexpr matrix() = default;

        constexpr matrix(type const &mat)
            : values{
                mat[0],
                mat[1]
            } {}

        explicit constexpr matrix(value_type scalar)
            : values{
                column_type{scalar, 0},
                column_type{0, scalar}
            } {}

        constexpr matrix(value_type x0, value_type y0,
                         value_type x1, value_type y1)
            : values{
                column_type{x0, y0},
                column_type{x1, y1}
            } {}

        constexpr matrix(column_type const &vec0, column_type const &vec1)
            : values{
                vec0,
                vec1
            } {}

        explicit constexpr matrix(matrix<3, 3, value_type> const &mat)
            : values{
                column_type{mat[0]},
                column_type{mat[1]}
            } {}

        explicit constexpr matrix(matrix<4, 4, value_type> const &mat)
            : values{
                column_type{mat[0]},
                column_type{mat[1]}
            } {}

        static constexpr fn identity() -> type { return type{1}; }

        static constexpr fn diagonal(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type x0, value_type y0,
                                         value_type x1, value_type y1) -> type { return type{x0, y0, x1, y1}; }

        static constexpr fn from_columns(column_type const &vec0, column_type const &vec1) -> type { return type{vec0, vec1}; }

        static constexpr fn from_rows(row_type const &vec0, row_type const &vec1) -> type { return type{vec0.x, vec1.x, vec0.y, vec1.y}; }

        static constexpr fn from_matrix3x3(matrix<3, 3, value_type> const &mat) -> type { return type{mat}; }

        static constexpr fn from_matrix4x4(matrix<4, 4, value_type> const &mat) -> type { return type{mat}; }

        // unary operators
        constexpr fn operator=(type const &mat) -> type & {
            if (this == &mat)
                return *this;

            this->values[0] = mat[0];
            this->values[1] = mat[1];
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-((*this)[0]), -((*this)[1])}; }

        constexpr fn operator+=(type const &mat) -> type & {
            this->values[0] += mat[0];
            this->values[1] += mat[1];
            return *this;
        }

        constexpr fn operator-=(type const &mat) -> type & {
            this->values[0] -= mat[0];
            this->values[1] -= mat[1];
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->values[0] *= scalar;
            this->values[1] *= scalar;
            return *this;
        }

        constexpr fn operator*=(type const &mat) -> type & {
            *this = *this * mat;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->values[0] /= scalar;
            this->values[1] /= scalar;
            return *this;
        }

        constexpr fn operator/=(type const &mat) -> type & {
            *this *= inverse(mat);
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->values[0]);
            ++(this->values[1]);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->values[0]);
            --(this->values[1]);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type mat{*this};
            ++(*this);
            return mat;
        }

        constexpr fn operator--(int) -> type {
            type mat{*this};
            --(*this);
            return mat;
        }

        // binary operators
        constexpr fn operator+(type const &mat) const -> type { return {(*this)[0] + mat[0], (*this)[1] + mat[1]}; }

        constexpr fn operator-(type const &mat) const -> type { return {(*this)[0] - mat[0], (*this)[1] - mat[1]}; }

        constexpr fn operator*(value_type scalar) const -> type { return {(*this)[0] * scalar, (*this)[1] * scalar}; }

        constexpr fn operator*(row_type const &vec) const -> column_type { return {(*this)[0][0] * vec.x + (*this)[1][0] * vec.y, (*this)[0][1] * vec.x + (*this)[1][1] * vec.y}; }

        constexpr fn operator*(type const &mat) const -> type {
            auto A0 = (*this)[0];
            auto A1 = (*this)[1];

            auto B0 = mat[0];
            auto B1 = mat[1];

            return {
                A0 * B0[0] + A1 * B0[1],
                A0 * B1[0] + A1 * B1[1],
            };
        }

        constexpr fn operator/(value_type scalar) const -> type { return {(*this)[0] / scalar, (*this)[1] / scalar}; }

        constexpr fn operator/(type const &mat) const -> type {
            type tmp{*this};
            return tmp /= mat;
        }

        // boolean operators
        constexpr fn operator==(type const &mat) const -> bool { return (*this)[0] == mat[0] && (*this)[1] == mat[1]; }

        constexpr fn operator!=(type const &mat) const -> bool { return (*this)[0] != mat[0] || (*this)[1] != mat[1]; }

    private:
        // members
        column_type values[columns];
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, matrix<2, 2, T> const &mat) -> matrix<2, 2, T> { return {mat[0] * scalar, mat[1] * scalar}; }

    // matrix3x3 definition
    template<arithmetic T>
    class matrix<3, 3, T> {
    public:
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type columns = 3;
        static constexpr size_type rows    = 3;

        using column_type = vector<rows, value_type>;
        using row_type = vector<columns, value_type>;

        using type = matrix<columns, rows, value_type>;

        // member access operators
        constexpr fn operator[](size_type i) -> column_type & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        constexpr fn operator[](size_type i) const -> column_type const & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        // constructors
        constexpr matrix() = default;

        constexpr matrix(type const &mat)
            : values{
                mat[0],
                mat[1],
                mat[2]
            } {}

        explicit constexpr matrix(value_type scalar)
            : values{
                column_type{scalar, 0, 0},
                column_type{0, scalar, 0},
                column_type{0, 0, scalar}
            } {}

        constexpr matrix(value_type x0, value_type y0, value_type z0,
                         value_type x1, value_type y1, value_type z1,
                         value_type x2, value_type y2, value_type z2)
            : values{
                column_type{x0, y0, z0},
                column_type{x1, y1, z1},
                column_type{x2, y2, z2}
            } {}

        constexpr matrix(column_type const &vec0, column_type const &vec1, column_type const &vec2)
            : values{
                vec0,
                vec1,
                vec2
            } {}

        explicit constexpr matrix(matrix<2, 2, value_type> const &mat)
            : values{
                column_type{mat[0], 0},
                column_type{mat[1], 0},
                column_type{0, 0, 1}
            } {}

        explicit constexpr matrix(matrix<4, 4, value_type> const &mat)
            : values{
                column_type{mat[0]},
                column_type{mat[1]},
                column_type{mat[2]}
            } {}

        static constexpr fn identity() -> type { return type{1}; }

        static constexpr fn diagonal(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type x0, value_type y0, value_type z0,
                                         value_type x1, value_type y1, value_type z1,
                                         value_type x2, value_type y2, value_type z2) -> type { return type{x0, y0, z0, x1, y1, z1, x2, y2, z2}; }

        static constexpr fn from_columns(column_type const &vec0, column_type const &vec1, column_type const &vec2) -> type { return type{vec0, vec1, vec2}; }

        static constexpr fn from_rows(row_type const &vec0, row_type const &vec1, row_type const &vec2) -> type { return type{vec0.x, vec1.x, vec2.x, vec0.y, vec1.y, vec2.y, vec0.z, vec1.z, vec2.z}; }

        static constexpr fn from_matrix2x2(matrix<2, 2, value_type> const &mat) -> type { return type{mat}; }

        static constexpr fn from_matrix4x4(matrix<4, 4, value_type> const &mat) -> type { return type{mat}; }

        static constexpr fn from_quaternion(quaternion<T> const &quat) -> type { return internal::quaternion_to_matrix<columns, rows, value_type>::compute(quat); }

        // unary operators
        constexpr fn operator=(type const &mat) -> type & {
            if (this == &mat)
                return *this;

            this->values[0] = mat[0];
            this->values[1] = mat[1];
            this->values[2] = mat[2];
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-((*this)[0]), -((*this)[1]), -((*this)[2])}; }

        constexpr fn operator+=(type const &mat) -> type & {
            this->values[0] += mat[0];
            this->values[1] += mat[1];
            this->values[2] += mat[2];
            return *this;
        }

        constexpr fn operator-=(type const &mat) -> type & {
            this->values[0] -= mat[0];
            this->values[1] -= mat[1];
            this->values[2] -= mat[2];
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->values[0] *= scalar;
            this->values[1] *= scalar;
            this->values[2] *= scalar;
            return *this;
        }

        constexpr fn operator*=(type const &mat) -> type & {
            *this = *this * mat;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->values[0] /= scalar;
            this->values[1] /= scalar;
            this->values[2] /= scalar;
            return *this;
        }

        constexpr fn operator/=(type const &mat) -> type & {
            *this *= inverse(mat);
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->values[0]);
            ++(this->values[1]);
            ++(this->values[2]);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->values[0]);
            --(this->values[1]);
            --(this->values[2]);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type mat{*this};
            ++(*this);
            return mat;
        }

        constexpr fn operator--(int) -> type {
            type mat{*this};
            --(*this);
            return mat;
        }

        // binary operators
        constexpr fn operator+(type const &mat) const -> type { return {(*this)[0] + mat[0], (*this)[1] + mat[1], (*this)[2] + mat[2]}; }

        constexpr fn operator-(type const &mat) const -> type { return {(*this)[0] - mat[0], (*this)[1] - mat[1], (*this)[2] - mat[2]}; }

        constexpr fn operator*(value_type scalar) const -> type { return {(*this)[0] * scalar, (*this)[1] * scalar, (*this)[2] * scalar}; }

        constexpr fn operator*(row_type const &vec) const -> column_type {
            return {
                (*this)[0][0] * vec.x + (*this)[1][0] * vec.y + (*this)[2][0] * vec.z,
                (*this)[0][1] * vec.x + (*this)[1][1] * vec.y + (*this)[2][1] * vec.z,
                (*this)[0][2] * vec.x + (*this)[1][2] * vec.y + (*this)[2][2] * vec.z
            };
        }

        constexpr fn operator*(type const &mat) const -> type {
            auto A0 = (*this)[0];
            auto A1 = (*this)[1];
            auto A2 = (*this)[2];

            auto B0 = mat[0];
            auto B1 = mat[1];
            auto B2 = mat[2];

            return {
                A0 * B0[0] + A1 * B0[1] + A2 * B0[2],
                A0 * B1[0] + A1 * B1[1] + A2 * B1[2],
                A0 * B2[0] + A1 * B2[1] + A2 * B2[2],
            };
        }

        constexpr fn operator/(value_type scalar) const -> type { return {(*this)[0] / scalar, (*this)[1] / scalar, (*this)[2] / scalar}; }

        constexpr fn operator/(type const &mat) const -> type {
            type tmp{*this};
            return tmp /= mat;
        }

        // boolean operators
        constexpr fn operator==(type const &mat) const -> bool { return (*this)[0] == mat[0] && (*this)[1] == mat[1] && (*this)[2] == mat[2]; }

        constexpr fn operator!=(type const &mat) const -> bool { return (*this)[0] != mat[0] || (*this)[1] != mat[1] || (*this)[2] != mat[2]; }

    private:
        // members
        column_type values[columns];
    };

    template<arithmetic T>
    constexpr fn operator*(T scalar, matrix<3, 3, T> const &mat) -> matrix<3, 3, T> { return {mat[0] * scalar, mat[1] * scalar, mat[2] * scalar}; }

    // matrix4x4 definition
    template<arithmetic T>
    class matrix<4, 4, T> {
    public:
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type columns = 4;
        static constexpr size_type rows    = 4;

        using column_type = vector<rows, value_type>;
        using row_type = vector<columns, value_type>;

        using type = matrix<columns, rows, value_type>;

        // member access operators
        constexpr fn operator[](size_type i) -> column_type & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        constexpr fn operator[](size_type i) const -> column_type const & {
            assert(i >= 0 && i < this->columns);
            return this->values[i];
        }

        // constructors
        constexpr matrix() = default;

        constexpr matrix(type const &mat)
            : values{
                mat[0],
                mat[1],
                mat[2],
                mat[3]
            } {}

        explicit constexpr matrix(value_type scalar)
            : values{
                column_type{scalar, 0, 0, 0},
                column_type{0, scalar, 0, 0},
                column_type{0, 0, scalar, 0},
                column_type{0, 0, 0, scalar}
            } {}

        constexpr matrix(value_type x0, value_type y0, value_type z0, value_type w0,
                         value_type x1, value_type y1, value_type z1, value_type w1,
                         value_type x2, value_type y2, value_type z2, value_type w2,
                         value_type x3, value_type y3, value_type z3, value_type w3)
            : values{
                column_type{x0, y0, z0, w0},
                column_type{x1, y1, z1, w1},
                column_type{x2, y2, z2, w2},
                column_type{x3, y3, z3, w3}
            } {}

        constexpr matrix(column_type const &vec0,
                         column_type const &vec1,
                         column_type const &vec2,
                         column_type const &vec3)
            : values{
                vec0,
                vec1,
                vec2,
                vec3
            } {}

        explicit constexpr matrix(matrix<2, 2, value_type> const &mat)
            : values{
                column_type{mat[0], 0, 0},
                column_type{mat[1], 0, 0},
                column_type{0, 0, 1, 0},
                column_type{0, 0, 0, 1}
            } {}

        explicit constexpr matrix(matrix<3, 3, value_type> const &mat)
            : values{
                column_type{mat[0], 0},
                column_type{mat[1], 0},
                column_type{mat[2], 0},
                column_type{0, 0, 0, 1}
            } {}

        static constexpr fn identity() -> type { return type{1}; }

        static constexpr fn diagonal(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type x0, value_type y0, value_type z0, value_type w0,
                                         value_type x1, value_type y1, value_type z1, value_type w1,
                                         value_type x2, value_type y2, value_type z2, value_type w2,
                                         value_type x3, value_type y3, value_type z3, value_type w3) -> type { return type{x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3}; }

        static constexpr fn from_columns(column_type const &vec0,
                                         column_type const &vec1,
                                         column_type const &vec2,
                                         column_type const &vec3) -> type { return type{vec0, vec1, vec2, vec3}; }

        static constexpr fn from_rows(row_type const &vec0,
                                      row_type const &vec1,
                                      row_type const &vec2,
                                      row_type const &vec3) -> type {
            return type{
                vec0.x,
                vec1.x,
                vec2.x,
                vec3.x,
                vec0.y,
                vec1.y,
                vec2.y,
                vec3.y,
                vec0.z,
                vec1.z,
                vec2.z,
                vec3.z,
                vec0.w,
                vec1.w,
                vec2.w,
                vec3.w
            };
        }

        static constexpr fn from_matrix2x2(matrix<2, 2, value_type> const &mat) -> type { return type{mat}; }

        static constexpr fn from_matrix3x3(matrix<4, 4, value_type> const &mat) -> type { return type{mat}; }

        static constexpr fn from_quaternion(quaternion<T> const &quat) -> type { return internal::quaternion_to_matrix<columns, rows, value_type>::compute(quat); }

        static fn from_euler(value_type const &x, value_type const &y, value_type const &z) {
            value_type c1 = math::cos(-x);
            value_type c2 = math::cos(-y);
            value_type c3 = math::cos(-z);
            value_type s1 = math::sin(-x);
            value_type s2 = math::sin(-y);
            value_type s3 = math::sin(-z);

            type res;
            res[0][0] = c2 * c3;
            res[0][1] = -c1 * s3 + s1 * s2 * c3;
            res[0][2] = s1 * s3 + c1 * s2 * c3;
            res[0][3] = static_cast<value_type>(0);
            res[1][0] = c2 * s3;
            res[1][1] = c1 * c3 + s1 * s2 * s3;
            res[1][2] = -s1 * c3 + c1 * s2 * s3;
            res[1][3] = static_cast<value_type>(0);
            res[2][0] = -s2;
            res[2][1] = s1 * c2;
            res[2][2] = c1 * c2;
            res[2][3] = static_cast<value_type>(0);
            res[3][0] = static_cast<value_type>(0);
            res[3][1] = static_cast<value_type>(0);
            res[3][2] = static_cast<value_type>(0);
            res[3][3] = static_cast<value_type>(1);
            return res;
        }

        static constexpr fn ortho(value_type left,
                                  value_type right,
                                  value_type bottom,
                                  value_type top,
                                  value_type zNear,
                                  value_type zFar) -> type {
            type res{1};
            res[0][0] = static_cast<T>(2) / (right - left);
            res[1][1] = static_cast<T>(2) / (top - bottom);
            res[2][2] = -static_cast<T>(2) / (zFar - zNear);
            res[3][0] = -(right + left) / (right - left);
            res[3][1] = -(top + bottom) / (top - bottom);
            res[3][2] = -(zFar + zNear) / (zFar - zNear);
            return res;
        }

        static constexpr fn perspective(value_type fovY, value_type aspect, value_type zNear, value_type zFar) -> type {
            assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

            T const tanHalfFovY = tan(fovY / static_cast<T>(2));

            type res(static_cast<T>(0));
            res[0][0] = static_cast<T>(1) / (aspect * tanHalfFovY);
            res[1][1] = static_cast<T>(1) / (tanHalfFovY);
            res[2][2] = -(zFar + zNear) / (zFar - zNear);
            res[2][3] = -static_cast<T>(1);
            res[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
            return res;
        }

        static constexpr fn lookAt(vector<3, T> const &position, vector<3, T> const &target, vector<3, T> const &up) -> type {
            vector<3, T> f{normalize(target - position)};
            vector<3, T> s{normalize(cross(f, up))};
            vector<3, T> u{cross(s, f)};

            type res(1);
            res[0][0] = s.x;
            res[1][0] = s.y;
            res[2][0] = s.z;
            res[0][1] = u.x;
            res[1][1] = u.y;
            res[2][1] = u.z;
            res[0][2] = -f.x;
            res[1][2] = -f.y;
            res[2][2] = -f.z;
            res[3][0] = -dot(s, position);
            res[3][1] = -dot(u, position);
            res[3][2] = dot(f, position);
            return res;
        }

        // unary operators
        constexpr fn operator=(type const &mat) -> type & {
            if (this == &mat)
                return *this;

            this->values[0] = mat[0];
            this->values[1] = mat[1];
            this->values[2] = mat[2];
            this->values[3] = mat[3];
            return *this;
        }

        constexpr fn operator+() const -> type { return this; }

        constexpr fn operator-() const -> type { return {-((*this)[0]), -((*this)[1]), -((*this)[2]), -((*this)[3])}; }

        constexpr fn operator+=(type const &mat) -> type & {
            this->values[0] += mat[0];
            this->values[1] += mat[1];
            this->values[2] += mat[2];
            this->values[3] += mat[3];
            return *this;
        }

        constexpr fn operator-=(type const &mat) -> type & {
            this->values[0] -= mat[0];
            this->values[1] -= mat[1];
            this->values[2] -= mat[2];
            this->values[3] -= mat[3];
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->values[0] *= scalar;
            this->values[1] *= scalar;
            this->values[2] *= scalar;
            this->values[3] *= scalar;
            return *this;
        }

        constexpr fn operator*=(type const &mat) -> type & {
            *this = *this * mat;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->values[0] /= scalar;
            this->values[1] /= scalar;
            this->values[2] /= scalar;
            this->values[3] /= scalar;
            return *this;
        }

        constexpr fn operator/=(type const &mat) -> type & {
            *this *= inverse(mat);
            return *this;
        }

        // increment and decrement operators
        constexpr fn operator++() -> type & {
            ++(this->values[0]);
            ++(this->values[1]);
            ++(this->values[2]);
            ++(this->values[3]);
            return *this;
        }

        constexpr fn operator--() -> type & {
            --(this->values[0]);
            --(this->values[1]);
            --(this->values[2]);
            --(this->values[3]);
            return *this;
        }

        constexpr fn operator++(int) -> type {
            type mat{*this};
            ++(*this);
            return mat;
        }

        constexpr fn operator--(int) -> type {
            type mat{*this};
            --(*this);
            return mat;
        }

        // binary operators
        constexpr fn operator+(type const &mat) const -> type { return {(*this)[0] + mat[0], (*this)[1] + mat[1], (*this)[2] + mat[2], (*this)[3] + mat[3]}; }

        constexpr fn operator-(type const &mat) const -> type { return {(*this)[0] - mat[0], (*this)[1] - mat[1], (*this)[2] - mat[2], (*this)[3] - mat[3]}; }

        constexpr fn operator*(value_type scalar) const -> type { return {(*this)[0] * scalar, (*this)[1] * scalar, (*this)[2] * scalar, (*this)[3] * scalar}; }

        constexpr fn operator*(row_type const &vec) const -> column_type {
            return {
                (*this)[0][0] * vec.x + (*this)[1][0] * vec.y + (*this)[2][0] * vec.z + (*this)[3][0] * vec.w,
                (*this)[0][1] * vec.x + (*this)[1][1] * vec.y + (*this)[2][1] * vec.z + (*this)[3][1] * vec.w,
                (*this)[0][2] * vec.x + (*this)[1][2] * vec.y + (*this)[2][2] * vec.z + (*this)[3][2] * vec.w,
                (*this)[0][3] * vec.x + (*this)[1][3] * vec.y + (*this)[2][3] * vec.z + (*this)[3][3] * vec.w
            };
        }

        constexpr fn operator*(type const &mat) const -> type {
            auto A0 = (*this)[0];
            auto A1 = (*this)[1];
            auto A2 = (*this)[2];
            auto A3 = (*this)[3];

            auto B0 = mat[0];
            auto B1 = mat[1];
            auto B2 = mat[2];
            auto B3 = mat[3];

            return {
                A0 * B0[0] + A1 * B0[1] + A2 * B0[2] + A3 * B0[3],
                A0 * B1[0] + A1 * B1[1] + A2 * B1[2] + A3 * B1[3],
                A0 * B2[0] + A1 * B2[1] + A2 * B2[2] + A3 * B2[3],
                A0 * B3[0] + A1 * B3[1] + A2 * B3[2] + A3 * B3[3]
            };
        }

        constexpr fn operator/(value_type scalar) const -> type { return {(*this)[0] / scalar, (*this)[1] / scalar, (*this)[2] / scalar, (*this)[3] / scalar}; }

        constexpr fn operator/(type const &mat) const -> type {
            type tmp{*this};
            return tmp /= mat;
        }

        // boolean operators
        constexpr fn operator==(type const &mat) const -> bool { return (*this)[0] == mat[0] && (*this)[1] == mat[1] && (*this)[2] == mat[2] && (*this)[3] == mat[3]; }

        constexpr fn operator!=(type const &mat) const -> bool { return (*this)[0] != mat[0] || (*this)[1] != mat[1] || (*this)[2] != mat[2] || (*this)[3] != mat[3]; }

    private:
        // members
        column_type values[columns];
    };

    template<floating_point T>
    constexpr fn operator*(T                      scalar,
                           matrix<4, 4, T> const &mat) -> matrix<4, 4, T> { return {mat[0] * scalar, mat[1] * scalar, mat[2] * scalar, mat[3] * scalar}; }

    template<arithmetic T>
    constexpr fn dot(quaternion<T> const &x, quaternion<T> const &y) -> T { return x.s * y.s + x.x * y.x + x.y * y.y + x.z * y.z; }

    template<floating_point T>
    fn magnitude(quaternion<T> const &quat) -> T { return sqrt(dot(quat, quat)); }

    template<floating_point T>
    fn normalize(quaternion<T> const &quat) -> quaternion<T> { return quat / magnitude(quat); }

    template<floating_point T>
    constexpr fn conjugate(quaternion<T> const &quat) -> quaternion<T> { return {quat.w, -quat.x, -quat.y, -quat.z}; }

    template<floating_point T>
    constexpr fn inverse(quaternion<T> const &quat) -> quaternion<T> { return conjugate(quat) / dot(quat, quat); }

    template<floating_point T>
    constexpr fn cross(quaternion<T> const &quat, vector<3, T> const &vec) -> vector<3, T> { return quat * vec; }

    template<floating_point T>
    constexpr fn cross(vector<3, T> const &vec, quaternion<T> const &quat) -> vector<3, T> { return inverse(quat) * vec; }

    template<floating_point T>
    constexpr fn rotate(quaternion<T> const &quat, vector<3, T> const &vec) -> vector<3, T> { return quat * vec; }

    template<floating_point T>
    constexpr fn rotate(quaternion<T> const &quat, vector<4, T> const &vec) -> vector<4, T> { return quat * vec; }

    namespace internal {
        template<core::usize C, core::usize R, typename T>
        struct matrix_to_quaternion;

        template<floating_point T>
        struct matrix_to_quaternion<3, 3, T> {
            static constexpr fn compute(matrix<3, 3, T> const &mat) -> quaternion<T> {
                auto fourSSquaredMinus1 = mat[0][0] + mat[1][1] + mat[2][2];
                auto fourXSquaredMinus1 = mat[0][0] - mat[1][1] - mat[2][2];
                auto fourYSquaredMinus1 = mat[1][1] - mat[0][0] - mat[2][2];
                auto fourZSquaredMinus1 = mat[2][2] - mat[0][0] - mat[1][1];

                core::u32 biggestIndex             = 0;
                auto      fourBiggestSquaredMinus1 = fourSSquaredMinus1;
                if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
                    fourBiggestSquaredMinus1 = fourXSquaredMinus1;
                    biggestIndex             = 1;
                }
                if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
                    fourBiggestSquaredMinus1 = fourYSquaredMinus1;
                    biggestIndex             = 2;
                }
                if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
                    fourBiggestSquaredMinus1 = fourZSquaredMinus1;
                    biggestIndex             = 3;
                }

                auto biggestVal = sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
                auto mult       = static_cast<T>(0.25) / biggestVal;

                switch (biggestIndex) {
                    default:
                    case 0:
                        return quaternion<T>{
                            biggestVal,
                            (mat[1][2] - mat[2][1]) * mult,
                            (mat[2][0] - mat[0][2]) * mult,
                            (mat[0][1] - mat[1][0]) * mult
                        };
                    case 1:
                        return quaternion<T>{
                            (mat[1][2] - mat[2][1]) * mult,
                            biggestVal,
                            (mat[0][1] + mat[1][0]) * mult,
                            (mat[2][0] + mat[0][2]) * mult
                        };
                    case 2:
                        return quaternion<T>{
                            (mat[2][0] - mat[0][2]) * mult,
                            (mat[0][1] + mat[1][0]) * mult,
                            biggestVal,
                            (mat[1][2] + mat[2][1]) * mult
                        };
                    case 3:
                        return quaternion<T>{
                            (mat[0][1] - mat[1][0]) * mult,
                            (mat[2][0] + mat[0][2]) * mult,
                            (mat[1][2] + mat[2][1]) * mult,
                            biggestVal
                        };
                }
            }
        };

        template<floating_point T>
        struct matrix_to_quaternion<4, 4, T> {
            static constexpr fn compute(matrix<4, 4, T> const &mat) -> quaternion<T> { return matrix_to_quaternion<3, 3, T>::compute(matrix<3, 3, T>{mat}); }
        };
    }

    // quaternion definition
    template<floating_point T>
    struct quaternion {
        using size_type = core::usize;
        using value_type = T;

        static constexpr size_type dimensions = 4;

        using scalar_type = value_type;
        using vector_type = vector<3, value_type>;

        using type = quaternion<value_type>;

        // members and member access operators
        T s, x, y, z;

        constexpr fn operator[](size_type i) -> value_type & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return s;
                case 1:
                    return x;
                case 2:
                    return y;
                case 3:
                    return z;
            }
        }

        constexpr fn operator[](size_type i) const -> value_type const & {
            assert(i >= 0 && i < this->dimensions);
            switch (i) {
                default:
                case 0:
                    return s;
                case 1:
                    return x;
                case 2:
                    return y;
                case 3:
                    return z;
            }
        }

        // constructors
        constexpr quaternion() = default;

        constexpr quaternion(type const &quat) = default;

        constexpr explicit quaternion(value_type scalar)
            : s{scalar}, x{0}, y{0}, z{0} {}

        constexpr quaternion(value_type scalar, vector<3, value_type> const &vector)
            : s{scalar}, x{vector.x}, y{vector.y}, z{vector.z} {}

        constexpr quaternion(value_type scalar, value_type vx, value_type vy, value_type vz)
            : s{scalar}, x{vx}, y{vy}, z{vz} {}

        constexpr explicit quaternion(vector<3, value_type> const &axis1, vector<3, value_type> const &axis2) {
            auto                  norm_uv     = sqrt(dot(axis1, axis1) * dot(axis2, axis2));
            auto                  scalar_part = norm_uv + dot(axis1, axis2);
            vector<3, value_type> vector_part{};

            if (scalar_part < static_cast<value_type>(1.e-6f) * norm_uv) {
                scalar_part = static_cast<value_type>(0);
                vector_part = abs(axis1.x) > abs(axis1.z)
                                  ? vector<3, value_type>{-axis1.y, axis1.x, static_cast<value_type>(0)}
                                  : vector<3, value_type>{static_cast<value_type>(0), -axis1.z, axis1.y};
            }
            else
                vector_part = cross(axis1, axis2);

            *this = normalize(type{scalar_part, vector_part});
        }

        constexpr explicit quaternion(vector<3, value_type> const &euler_angles) {
            auto cos_val = cos(euler_angles * static_cast<value_type>(0.5));
            auto sin_val = sin(euler_angles * static_cast<value_type>(0.5));

            this->s = cos_val.x * cos_val.y * cos_val.z + sin_val.x * sin_val.y * sin_val.z;
            this->x = sin_val.x * cos_val.y * cos_val.z - cos_val.x * sin_val.y * sin_val.z;
            this->y = cos_val.x * sin_val.y * cos_val.z + sin_val.x * cos_val.y * sin_val.z;
            this->z = cos_val.x * cos_val.y * sin_val.z - sin_val.x * sin_val.y * cos_val.z;
        }

        constexpr explicit quaternion(matrix<3, 3, value_type> const &mat) { *this = internal::matrix_to_quaternion<3, 3, T>::compute(mat); }

        constexpr explicit quaternion(matrix<4, 4, value_type> const &mat) { *this = internal::matrix_to_quaternion<4, 4, T>::compute(mat); }

        static constexpr fn real(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn pure(vector<3, T> const &vec) -> type { return type{0, vec}; }

        static constexpr fn pure(value_type vx, value_type vy, value_type vz) -> type { return type{0, vx, vy, vz}; }

        static constexpr fn unit(vector<3, T> const &vec) -> type { return type{0, normalize(vec)}; }

        static constexpr fn unit(value_type vx, value_type vy, value_type vz) -> type { return type{0, normalize(vector<3, T>{vx, vy, vz})}; }

        static constexpr fn unit_norm(value_type scalar, vector<3, T> const &vec) -> type { return normalize(quaternion<T>{scalar, vec}); }

        static constexpr fn unit_norm(value_type scalar, value_type vx, value_type vy, value_type vz) -> type { return normalize(quaternion<T>{scalar, vx, vy, vz}); }

        static constexpr fn from_scalar(value_type scalar) -> type { return type{scalar}; }

        static constexpr fn from_scalars(value_type scalar, value_type vx, value_type vy, value_type vz) -> type { return type{scalar, vx, vy, vz}; }

        static constexpr fn from_parts(value_type scalar, vector<3, T> const &vec) -> type { return type{scalar, vec}; }

        static constexpr fn from_normalized_axes(vector<3, T> const &vec1, vector<3, T> const &vec2) -> type { return type{vec1, vec2}; }

        static constexpr fn from_euler(vector<3, T> const &vec) -> type { return type{vec}; }

        static constexpr fn from_matrix3x3(matrix<3, 3, T> const &mat) -> type { return type{mat}; }

        static constexpr fn from_matrix4x4(matrix<4, 4, T> const &mat) -> type { return type{mat}; }

        static constexpr fn from_rotation(T angle, vector<3, T> const &axis) -> type { return quaternion{cos(angle / static_cast<T>(2)), normalize(axis) * sin(angle / static_cast<T>(2))}; }

        constexpr explicit operator matrix<3, 3, value_type>() const { return matrix<3, 3, value_type>::from_quaternion(*this); }

        constexpr explicit operator matrix<4, 4, value_type>() const { return matrix<4, 4, value_type>::from_quaternion(*this); }

        constexpr fn operator=(type const &quat) -> type & = default;

        constexpr fn operator+=(type const &quat) -> type & {
            this->s += quat.s;
            this->x += quat.x;
            this->y += quat.y;
            this->z += quat.z;
            return *this;
        }

        constexpr fn operator-=(type const &quat) -> type & {
            this->s -= quat.s;
            this->x -= quat.x;
            this->y -= quat.y;
            this->z -= quat.z;
            return *this;
        }

        constexpr fn operator*=(type const &quat) -> type & {
            this->s = this->s * quat.s - this->x * quat.x - this->y * quat.y - this->z * quat.z;
            this->x = this->s * quat.x + this->x * quat.s + this->y * quat.z - this->z * quat.y;
            this->y = this->s * quat.y + this->y * quat.s + this->z * quat.x - this->x * quat.z;
            this->z = this->s * quat.z + this->z * quat.s + this->x * quat.y - this->y * quat.x;
            return *this;
        }

        constexpr fn operator*=(value_type scalar) -> type & {
            this->s *= scalar;
            this->x *= scalar;
            this->y *= scalar;
            this->z *= scalar;
            return *this;
        }

        constexpr fn operator/=(value_type scalar) -> type & {
            this->s /= scalar;
            this->x /= scalar;
            this->y /= scalar;
            this->z /= scalar;
            return *this;
        }

        constexpr fn operator+() const -> type { return *this; }

        constexpr fn operator-() const -> type { return {-this->s, -this->x, -this->y, -this->z}; }

        constexpr fn operator+(type const &quat) const -> type { return {this->s + quat.s, this->x + quat.x, this->y + quat.y, this->z + quat.z}; }

        constexpr fn operator-(type const &quat) const -> type { return {this->s - quat.s, this->x - quat.x, this->y - quat.y, this->z - quat.z}; }

        constexpr fn operator*(type const &quat) const -> type {
            return {
                this->s * quat.s - this->x * quat.x - this->y * quat.y - this->z * quat.z,
                this->s * quat.x + this->x * quat.s + this->y * quat.z - this->z * quat.y,
                this->s * quat.y + this->y * quat.s + this->z * quat.x - this->x * quat.z,
                this->s * quat.z + this->z * quat.s + this->x * quat.y - this->y * quat.x
            };
        }

        constexpr fn operator*(vector<3, value_type> const &vec) const -> vector<3, value_type> {
            vector<3, value_type> u(this->x, this->y, this->z);
            vector<3, value_type> uv(cross(u, vec));
            vector<3, value_type> uuv(cross(u, uv));

            return vec + ((uv * this->w) + uuv) * static_cast<value_type>(2);
        }

        constexpr fn operator*(vector<4, value_type> const &vec) const -> vector<4, value_type> { return {*this * vector<3, value_type>(vec), vec.s}; }

        constexpr fn operator*(value_type scalar) const -> type { return {this->s * scalar, this->x * scalar, this->y * scalar, this->z * scalar}; }

        constexpr fn operator/(value_type scalar) const -> type { return {this->s / scalar, this->x / scalar, this->y / scalar, this->z / scalar}; }

        constexpr fn operator==(type const &quat) const -> bool { return this->x == quat.x && this->y == quat.y && this->z == quat.z && this->s == quat.s; }

        constexpr fn operator!=(type const &quat) const -> bool { return this->x != quat.x || this->y != quat.y || this->z != quat.z || this->s != quat.s; }
    };

    template<floating_point T>
    constexpr fn operator*(vector<3, T> const &vec, quaternion<T> const &quat) -> vector<3, T> { return inverse(quat) * vec; }

    template<floating_point T>
    constexpr fn operator*(vector<4, T> const &vec, quaternion<T> const &quat) -> vector<4, T> { return inverse(quat) * vec; }

    template<floating_point T>
    constexpr fn operator*(T
                           scalar,
                           quaternion<T> const &quat
    ) -> vector<4, T> {
        return
            quat * scalar;
    }

    template<arithmetic T>
    auto value_ptr(vector<2, T> const &v) -> const T * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(vector<2, T> &v) -> T * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(vector<3, T> const &v) -> T const * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(vector<3, T> &v) -> T * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(vector<4, T> const &v) -> const T * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(vector<4, T> &v) -> T * { return &(v.x); }

    template<arithmetic T>
    auto value_ptr(matrix<2, 2, T> const &m) -> const T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(matrix<2, 2, T> &m) -> T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(matrix<3, 3, T> const &m) -> const T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(matrix<3, 3, T> &m) -> T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(matrix<4, 4, T> const &m) -> const T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(matrix<4, 4, T> &m) -> T * { return &(m[0].x); }

    template<arithmetic T>
    auto value_ptr(quaternion<T> const &q) -> T const * { return &(q[0]); }

    template<floating_point T>
    auto value_ptr(quaternion<T> &q) -> T * { return &(q[0]); }
}

#endif //MICRO_MATHEMATICS_LINEAR_H
