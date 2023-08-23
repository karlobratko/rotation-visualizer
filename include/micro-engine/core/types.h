//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_CORE_TYPES_H
#define MICRO_CORE_TYPES_H

#include <cmath>
#include <cstdint>
#include <functional>
#include <string>

#ifndef fn
#define fn auto
#endif

namespace micro::core {
    template<typename T, typename R> using Function = std::function<R(T)>;
    template<typename T1, typename T2, typename R> using BiFunction = std::function<R(T1, T2)>;
    using Runnable = std::function<void()>;
    template<typename R> using Supplier = std::function<R()>;
    template<typename T> using Consumer = Function<T, void>;
    template<typename T> using Predicate = Function<T, bool>;
    template<typename T> using Handler = Function<T, T>;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using f32 = std::float_t;
    using f64 = std::double_t;

    using cstring = char const *;

    using isize = std::intptr_t;
    using usize = std::size_t;

#pragma pack(push, 1)
    template<typename Member, std::size_t O>
    struct Pad {
        char   pad[O];
        Member m;
    };
#pragma pack(pop)

    template<typename Member>
    struct Pad<Member, 0> {
        Member m;
    };

    template<typename Base, typename Member, std::size_t O>
    struct MakeUnion {
        union U {
            char           c;
            Base           base;
            Pad<Member, O> pad;

            constexpr U() noexcept: c{} {};
        };

        constexpr static U u{};
    };

    template<typename Member, typename Base, typename Orig>
    struct offset_of_impl {
        template<std::size_t off, auto union_part = &MakeUnion<Base, Member, off>::u>
        static constexpr auto offset2(Member Orig::*member) -> std::ptrdiff_t {
            if constexpr (off > sizeof(Base))
                throw 1;
            else {
                auto const diff1 = &((static_cast<Orig const *>(&union_part->base))->*member);
                auto const diff2 = &union_part->pad.m;
                if (diff1 > diff2) {
                    constexpr auto MIN = sizeof(Member) < alignof(Orig) ? sizeof(Member) : alignof(Orig);
                    return offset2<off + MIN>(member);
                }
                else
                    return off;
            }
        }
    };

    template<class Member, class Base>
    auto get_types(Member Base::*) -> std::tuple<Member, Base>;

    template<class TheBase = void, class TT>
    inline constexpr auto offset_of(TT member) -> std::ptrdiff_t {
        using T = decltype(get_types(std::declval<TT>()));
        using Member = std::tuple_element_t<0, T>;
        using Orig = std::tuple_element_t<1, T>;
        using Base = std::conditional_t<std::is_void_v<TheBase>, Orig, TheBase>;
        return offset_of_impl<Member, Base, Orig>::template offset2<0>(member);
    }

    template<auto member, class TheBase = void>
    inline constexpr auto offset_of() -> std::ptrdiff_t { return offset_of<TheBase>(member); }
}

#endif //MICRO_CORE_TYPES_H
