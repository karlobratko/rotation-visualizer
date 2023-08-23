//
// Created by kbratko on 7/26/2023.
//

#ifndef MICRO_PERFORMANCE_STOPWATCH_H
#define MICRO_PERFORMANCE_STOPWATCH_H

#include <atomic>
#include <chrono>

#include "base.h"
#include "../core/types.h"

namespace micro::perf {
    template<typename C = std::chrono::high_resolution_clock> requires std::chrono::is_clock_v<C>
    class stopwatch {
    public:
        using clock_type = C;

        static fn start() -> stopwatch;

        template<typename D = clock_type::duration>
            requires is_duration_v<D>
        fn elapsed() const -> D::rep;

    private:
        typename clock_type::time_point start_point = C::now();
    };

    template<typename C>
        requires std::chrono::is_clock_v<C>
    auto stopwatch<C>::start() -> stopwatch { return stopwatch(); }

    template<typename C>
        requires std::chrono::is_clock_v<C>
    template<typename D>
        requires is_duration_v<D>
    auto stopwatch<C>::elapsed() const -> D::rep {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto counted_time = std::chrono::duration_cast<D>(C::now() - start_point).count();
        std::atomic_thread_fence(std::memory_order_relaxed);
        return counted_time;
    }

    using percise_stopwatch = stopwatch<>;
    using system_stopwatch = stopwatch<std::chrono::system_clock>;
    using monotonic_stopwatch = stopwatch<std::chrono::steady_clock>;

    template<typename C = std::chrono::high_resolution_clock, typename D = C::duration>
        requires std::chrono::is_clock_v<C> && is_duration_v<D>
    fn benchmark(core::Runnable const &function, core::Consumer<typename D::rep> const &consumer) -> void {
        stopwatch<C> sw{};
        function();
        consumer(sw.template elapsed<D>());
    }

    template<typename C = std::chrono::high_resolution_clock, typename D = C::duration>
        requires std::chrono::is_clock_v<C> && is_duration_v<D>
    fn benchmark(core::Runnable const &function) -> C::duration::rep {
        stopwatch<C> sw{};
        function();
        return sw.template elapsed<D>();
    }
}
#endif //MICRO_PERFORMANCE_STOPWATCH_H
