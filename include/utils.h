//
// Created by kbratko on 8/16/2023.
//

#ifndef FINAL_UTILS_H
#define FINAL_UTILS_H

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "micro-engine/micro.h"

#include "state.h"

using namespace micro;
using namespace micro::core;
using namespace micro::graphics;

auto dateTimeToString() -> std::string {
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d%H%M%S");
    return oss.str();
}

auto printAndExit(std::string const &message) -> void {
    cfatal << message << std::endl;
    std::exit(EXIT_FAILURE);
}

auto loadModel(cstring                                    path,
               ModelState &                               model, BoundingBoxState &boundingBox,
               core::Consumer<std::string const &> const &onError) -> void {
    std::vector<Vertex> vertices{};
    std::vector<u32>    indices{};
    obj::load<u32>(path, vertices, indices, onError);

    model.vertices = Buffer{Buffer::Target::Vertex};
    model.vertices
         .bind()
         .fill(vertices)
         .unbind();
    model.indices = Buffer{Buffer::Target::Index};
    model.indices
         .bind()
         .fill(indices)
         .unbind();

    model.model = Model{};
    model.model
         .bind()
         .buffer<Vertex, &Vertex::position, vector4<f32>::dimensions, f32>(
             model.shader.attribute(Application::Shaders::Vertex::Attributes::Position),
             model.vertices
         )
         .index<u32>(model.indices)
         .unbind();

    boundingBox.box = BoundingBox::from(vertices);

    boundingBox.vertices = Buffer{Buffer::Target::Vertex};
    boundingBox.vertices
               .bind()
               .fill(boundingBox.box.vertices())
               .unbind();
    boundingBox.indices = Buffer{Buffer::Target::Index};
    boundingBox.indices
               .bind()
               .fill(boundingBox.box.indices<u8>())
               .unbind();

    boundingBox.model = Model{};
    boundingBox.model
               .bind()
               .buffer<vector3<f32>, f32>(
                   boundingBox.shader.attribute(Application::Shaders::Vertex::Attributes::Position),
                   boundingBox.vertices
               )
               .index<u8>(boundingBox.indices)
               .unbind();
}

#endif //FINAL_UTILS_H
