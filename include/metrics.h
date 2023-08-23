//
// Created by kbratko on 7/31/2023.
//

#ifndef FINAL_METRICS_H
#define FINAL_METRICS_H

#include <chrono>

#include <micro-engine/micro.h>

using namespace micro;
using namespace micro::core;

struct BenchmarkMetric {
    usize                         vertices;
    usize                         indices;
    usize                         rotations;
    std::chrono::nanoseconds::rep time;

    friend fn operator<<(std::ostream &os, BenchmarkMetric const &bm) -> std::ostream &;
};

fn operator<<(std::ostream &os, BenchmarkMetric const &bm) -> std::ostream & { return os << bm.vertices << ',' << bm.indices << ',' << bm.rotations << ',' << bm.time << '\n'; }

#endif //FINAL_METRICS_H
