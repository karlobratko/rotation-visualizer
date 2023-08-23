//
// Created by kbratko on 7/25/2023.
//

#ifndef MICRO_UTILS_OBJECT_H
#define MICRO_UTILS_OBJECT_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

#include "../graphics/vertex.h"
#include "../mathematics/linear.h"

namespace micro::obj {
    template<std::unsigned_integral T>
    auto load(core::cstring                              path,
              std::vector<graphics::Vertex> &            vertices,
              std::vector<T> &                           indices,
              core::Consumer<std::string const &> const &onError = [](auto const &) {}) -> void {
        std::ifstream is{path, std::ios::in};
        if (!is)
            onError("Unable to open the file!");

        std::vector<math::vector4<core::f32>>      positionBuff;
        std::vector<math::vector3<core::f32>>      uvBuff;
        std::vector<math::vector3<core::f32>>      normalBuff;
        std::vector<std::vector<math::vector3<T>>> idxBuff{};

        std::string line;
        while (std::getline(is, line)) {
            std::istringstream iss{line};

            std::string type{};
            iss >> type;

            // ignore comments
            if (type == "#") continue;
                // vertex data
            else if (type == "v") {
                math::vector4<core::f32> vertex{};
                iss >> vertex.x >> vertex.y >> vertex.z;
                if (iss.eof())
                    vertex.w = 1.f;
                else
                    iss >> vertex.w;

                positionBuff.push_back(vertex);
            }
            else if (type == "vt") {
                math::vector3<core::f32> uv{};
                iss >> uv.s;
                if (iss.eof()) {
                    uv.t = 0.f;
                    uv.p = 0.f;
                }
                else {
                    iss >> uv.t;
                    if (iss.eof())
                        uv.p = 0.f;
                    else
                        iss >> uv.p;
                }

                uvBuff.push_back(uv);
            }
            else if (type == "vn") {
                math::vector3<core::f32> normal{};
                iss >> normal.x >> normal.y >> normal.z;

                normalBuff.push_back(normal);
            }
            else if (type == "f") {
                std::vector<math::vector3<T>> inline_idxs{};
                if (uvBuff.empty() && normalBuff.empty()) {
                    math::vector3<T> idx{};
                    do {
                        iss >> idx.x;
                        inline_idxs.push_back(idx);
                    }
                    while (!iss.eof());
                }
                else if (normalBuff.empty()) {
                    math::vector3<T> idx{};
                    char             delim;
                    do {
                        iss >> idx.x >> delim >> idx.y;
                        inline_idxs.push_back(idx);
                    }
                    while (!iss.eof());
                }
                else if (uvBuff.empty()) {
                    math::vector3<T> idx{};
                    char             delim;
                    do {
                        iss >> idx.x >> delim >> delim >> idx.z;
                        inline_idxs.push_back(idx);
                    }
                    while (!iss.eof());
                }
                else {
                    math::vector3<T> idx{};
                    char             delim;
                    do {
                        iss >> idx.x >> delim >> idx.y >> delim >> idx.z;
                        inline_idxs.push_back(idx);
                    }
                    while (!iss.eof());
                }

                idxBuff.push_back(inline_idxs);
            }
        }

        is.close();

        if (uvBuff.empty() && normalBuff.empty()) {
            std::transform(
                positionBuff.begin(), positionBuff.end(),
                std::back_inserter(vertices),
                graphics::Vertex::from_position
            );
            for (auto const &inline_idxs : idxBuff) { for (auto const &idx : inline_idxs) { indices.push_back(idx.x - 1); } }
        }
        else {
            auto indexTriangulationLoop = [&](std::function<void(math::vector3<T> const &)> const &callback) {
                for (auto const &inline_idxs : idxBuff) {
                    auto start = std::begin(inline_idxs);
                    for (auto it = start;
                         it != std::end(inline_idxs) && std::next(it, 2) < std::end(inline_idxs);
                         it = std::next(it)) {
                        callback(*start);
                        callback(*std::next(it, 1));
                        callback(*std::next(it, 2));
                    }
                }
            };

            if (normalBuff.empty())
                indexTriangulationLoop(
                    [&](math::vector3<T> const &idx) {
                        graphics::Vertex vertex{
                            positionBuff.at(idx.x - 1),
                            uvBuff.at(idx.y - 1),
                            math::vector3<core::f32>{0},
                            math::vector3<core::f32>{0}
                        };
                        auto iter = std::find(vertices.begin(), vertices.end(), vertex);
                        if (iter == vertices.end()) {
                            vertices.emplace_back(vertex);
                            indices.push_back(vertices.size() - 1);
                        }
                        else
                            indices.push_back(std::distance(vertices.begin(), iter));
                    }
                );
            else if (uvBuff.empty())
                indexTriangulationLoop(
                    [&](math::vector3<T> const &idx) {
                        graphics::Vertex vertex{
                            positionBuff.at(idx.x - 1),
                            math::vector3<core::f32>{0},
                            normalBuff.at(idx.z - 1),
                            math::vector3<core::f32>{0}
                        };
                        auto iter = std::find(vertices.begin(), vertices.end(), vertex);
                        if (iter == vertices.end()) {
                            vertices.emplace_back(vertex);
                            indices.push_back(vertices.size() - 1);
                        }
                        else
                            indices.push_back(std::distance(vertices.begin(), iter));
                    }
                );
            else
                indexTriangulationLoop(
                    [&](math::vector3<T> const &idx) {
                        graphics::Vertex vertex{
                            positionBuff.at(idx.x - 1),
                            uvBuff.at(idx.y - 1),
                            normalBuff.at(idx.z - 1),
                            math::vector3<core::f32>{0}
                        };
                        auto iter = std::find(vertices.begin(), vertices.end(), vertex);
                        if (iter == vertices.end()) {
                            vertices.emplace_back(vertex);
                            indices.push_back(vertices.size() - 1);
                        }
                        else
                            indices.push_back(std::distance(vertices.begin(), iter));
                    }
                );
        }
    }
}

#endif //MICRO_UTILS_OBJECT_H
