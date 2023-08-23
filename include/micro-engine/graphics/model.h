//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_GRAPHICS_MODEL_H
#define MICRO_GRAPHICS_MODEL_H

#include <glad/glad.h>

#include <unordered_set>

#include "buffer.h"
#include "../core/types.h"
#include "../utils/conversion.h"

namespace micro::graphics {
    class Model {
    public:
        enum class Primitive : GLenum {
            Point         = GL_POINTS,
            Line          = GL_LINES,
            LineLoop      = GL_LINE_LOOP,
            LineStrip     = GL_LINE_STRIP,
            Triangle      = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP
        };

        enum class Face : GLenum {
            FrontAndBack = GL_FRONT_AND_BACK,
            Front        = GL_FRONT,
            Back         = GL_BACK
        };

        enum class Polygon : GLenum {
            Point = GL_POINT,
            Line  = GL_LINE,
            Fill  = GL_FILL
        };

        GLuint                     id = 0;
        std::unordered_set<GLuint> bindings{};

        Model();

        Model(Model const &model) = delete;

        fn operator=(Model const &model) -> Model & = delete;

        Model(Model &&model) noexcept;

        fn operator=(Model &&model) noexcept -> Model &;

        ~Model();

        fn bind() -> Model &;

        fn unbind() -> Model &;

        fn dispose() -> void;

        fn buffer(GLuint  binding,
                  Buffer &buf,
                  GLenum  componentType,
                  GLint   size,
                  bool    normalized = false,
                  GLsizei stride     = 0,
                  GLsizei offset     = 0) -> Model &;

        template<typename T, typename U>
            requires std::is_arithmetic_v<U>
        fn buffer(GLuint  binding,
                  Buffer &buf,
                  bool    normalized = false,
                  GLsizei offset     = 0) -> Model &;

        template<typename T>
            requires std::is_arithmetic_v<T>
        fn buffer(GLuint  binding,
                  Buffer &buf,
                  GLint   size,
                  bool    normalized = false,
                  GLsizei stride     = 0,
                  GLsizei offset     = 0) -> Model &;

        template<typename T, typename U>
            requires std::is_arithmetic_v<U>
        fn buffer(GLuint  binding,
                  Buffer &buf,
                  GLint   size,
                  bool    normalized = false,
                  GLsizei offset     = 0) -> Model &;

        template<typename T, auto P, typename U>
            requires std::is_arithmetic_v<U>
        fn buffer(GLuint binding, Buffer &buf, GLint size, bool normalized = false) -> Model &;

        template<typename T, auto P, GLint size, typename U>
            requires std::is_arithmetic_v<U>
        fn buffer(GLuint binding, Buffer &buf, bool normalized = false) -> Model &;

        template<std::unsigned_integral U>
        fn index(Buffer &buf) -> Model &;

        fn render(Primitive mode = Primitive::Triangle) -> Model &;

        static fn polygon(Face face, Polygon mode) -> void;

    private:
        Buffer *    ebo           = nullptr;
        GLenum      eboValueType  = 0;
        core::usize verticesCount = 0;
    };

    Model::Model() : bindings{}, ebo{nullptr}, eboValueType{0}, verticesCount{0} {
        glGenVertexArrays(1, &id);
    }

    Model::Model(Model &&model) noexcept
        : id{std::exchange(model.id, 0)},
          ebo{std::exchange(model.ebo, nullptr)},
          eboValueType{std::exchange(model.eboValueType, 0)},
          verticesCount{std::exchange(model.verticesCount, 0)} {
        bindings = model.bindings;
        model.bindings.clear();
    }

    fn Model::operator=(Model &&model) noexcept -> Model & {
        if (this == &model)
            return *this;

        id            = std::exchange(model.id, 0);
        bindings      = std::exchange(model.bindings, std::unordered_set<core::u32>{});
        ebo           = std::exchange(model.ebo, nullptr);
        eboValueType  = std::exchange(model.eboValueType, 0);
        verticesCount = std::exchange(model.verticesCount, 0);

        return *this;
    }

    Model::~Model() { dispose(); }

    fn Model::bind() -> Model & {
        glBindVertexArray(id);

        return *this;
    }

    fn Model::unbind() -> Model & {
        glBindVertexArray(0);

        return *this;
    }

    fn Model::dispose() -> void {
        if (id != 0) {
            bind();
            for (auto &b : bindings) { glDisableVertexAttribArray(b); }
            unbind();

            glDeleteVertexArrays(1, &id);

            id            = 0;
            verticesCount = 0;
            bindings.clear();
            ebo          = nullptr;
            eboValueType = 0;
        }
    }

    template<typename T, typename E>
        requires std::is_arithmetic_v<E>
    fn Model::buffer(GLuint _binding, Buffer &_buf, bool _normalized, GLsizei _offset) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              sizeof(T) / sizeof(E),
                              utils::typeToGLenum<E>(),
                              _normalized ? GL_TRUE : GL_FALSE,
                              sizeof(T),
                              reinterpret_cast<void const *>(_offset * sizeof(E)));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size / (sizeof(T) / sizeof(E));

        bindings.insert(_binding);

        return *this;
    }

    auto Model::buffer(GLuint  _binding, Buffer &_buf, GLenum _componentType, GLint _nrComponents, bool _normalized, GLsizei _stride,
                       GLsizei _offset) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              _nrComponents,
                              _componentType,
                              _normalized ? GL_TRUE : GL_FALSE,
                              _stride,
                              reinterpret_cast<void const *>(_offset));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size;

        bindings.insert(_binding);

        return *this;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    fn Model::buffer(GLuint  _binding,
                     Buffer &_buf,
                     GLint   _size,
                     bool    _normalized,
                     GLsizei _stride,
                     GLsizei _offset) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              _size,
                              utils::typeToGLenum<T>(),
                              _normalized ? GL_TRUE : GL_FALSE,
                              _stride * sizeof(T),
                              reinterpret_cast<void const *>(_offset * sizeof(T)));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size / _stride;

        bindings.insert(_binding);

        return *this;
    }

    template<typename T, typename E>
        requires std::is_arithmetic_v<E>
    fn Model::buffer(GLuint _binding, Buffer &_buf, GLint _size, bool _normalized, GLsizei _offset) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              _size,
                              utils::typeToGLenum<E>(),
                              _normalized ? GL_TRUE : GL_FALSE,
                              sizeof(T),
                              reinterpret_cast<void const *>(_offset));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size;

        bindings.insert(_binding);

        return *this;
    }

    template<typename T, auto P, typename E>
        requires std::is_arithmetic_v<E>
    auto Model::buffer(GLuint _binding, Buffer &_buf, GLint _size, bool _normalized) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              _size,
                              utils::typeToGLenum<E>(),
                              _normalized ? GL_TRUE : GL_FALSE,
                              sizeof(T),
                              reinterpret_cast<void const *>(core::offset_of<P>()));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size;

        bindings.insert(_binding);

        return *this;
    }

    template<typename T, auto P, GLint size, typename E>
        requires std::is_arithmetic_v<E>
    auto Model::buffer(GLuint _binding, Buffer &_buf, bool _normalized) -> Model & {
        _buf.bind();
        glVertexAttribPointer(_binding,
                              size,
                              utils::typeToGLenum<E>(),
                              _normalized ? GL_TRUE : GL_FALSE,
                              sizeof(T),
                              reinterpret_cast<void const *>(core::offset_of<P>()));
        glEnableVertexAttribArray(_binding);
        _buf.unbind();

        verticesCount = _buf.size;

        bindings.insert(_binding);

        return *this;
    }

    template<std::unsigned_integral U>
    fn Model::index(Buffer &_buf) -> Model & {
        ebo          = const_cast<Buffer *>(&_buf);
        eboValueType = utils::typeToGLenum<U>();

        return *this;
    }

    fn Model::render(Primitive _mode) -> Model & {
        if (ebo != nullptr) {
            ebo->bind();
            glDrawElements(static_cast<GLenum>(_mode), static_cast<GLsizei>(ebo->size), eboValueType, nullptr);
            ebo->unbind();
        }
        else
            glDrawArrays(static_cast<GLenum>(_mode), 0, static_cast<GLsizei>(verticesCount));

        return *this;
    }

    auto Model::polygon(Face _face, Polygon _mode) -> void {
        glPolygonMode(static_cast<GLenum>(_face), static_cast<GLenum>(_mode));
    }
}

#endif //MICRO_GRAPHICS_MODEL_H
