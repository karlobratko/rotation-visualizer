//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_GRAPHICS_BUFFER_H
#define MICRO_GRAPHICS_BUFFER_H

#include <cstddef>
#include <vector>

#include <glad/glad.h>
#include "../core/types.h"

namespace micro::graphics {
    class Buffer {
    public:
        enum class Target : GLenum {
            None   = 0,
            Vertex = GL_ARRAY_BUFFER,
            Index  = GL_ELEMENT_ARRAY_BUFFER,
            Read   = GL_COPY_READ_BUFFER,
            Write  = GL_COPY_WRITE_BUFFER
        };

        enum class Usage : GLenum {
            Stream  = GL_STREAM_DRAW,
            Static  = GL_STATIC_DRAW,
            Dynamic = GL_DYNAMIC_DRAW
        };

        GLuint      id     = 0;
        Target      target = Target::None;
        core::usize size   = 0;

        explicit Buffer(Target target);

        Buffer(Buffer const &buf);

        fn operator=(Buffer const &buf) -> Buffer &;

        Buffer(Buffer &&buf) noexcept;

        fn operator=(Buffer &&buf) noexcept -> Buffer &;

        ~Buffer();

        fn dispose() -> void;

        fn bind() -> Buffer &;

        fn unbind() -> Buffer &;

        template<typename T>
        fn fill(core::usize size, T const *data, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fill(std::vector<T> const &buf, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fill(T const &val, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fill(core::usize size, T const *data, Target target, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fill(std::vector<T> const &buf, Target target, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fill(T const &val, Target target, Usage usage = Usage::Static) -> Buffer &;

        template<typename T>
        fn fetch(size_t size, T *data, GLintptr offset = 0) -> void;

        template<typename T>
        fn fetch(std::vector<T> &buf) -> void;

    private:
        core::usize typeSize = 0;
    };

    Buffer::Buffer(Buffer::Target _target) : target{_target}, size{0}, typeSize{0} {
        glGenBuffers(1, &id);
    }

    Buffer::Buffer(Buffer const &buf) : target{buf.target}, size{buf.size}, typeSize{buf.typeSize} {
        glGenBuffers(1, &id);

        glBindBuffer(static_cast<GLenum>(Target::Read), buf.id);
        glBindBuffer(static_cast<GLenum>(Target::Write), id);
        glCopyBufferSubData(static_cast<GLenum>(Target::Read), static_cast<GLenum>(Target::Write), 0, 0,
                            static_cast<GLsizeiptr>(typeSize * size));
    }

    fn Buffer::operator=(Buffer const &buf) -> Buffer & {
        if (this == &buf)
            return *this;

        target   = buf.target;
        size     = buf.size;
        typeSize = buf.typeSize;

        glGenBuffers(1, &id);

        glBindBuffer(static_cast<GLenum>(Target::Read), buf.id);
        glBindBuffer(static_cast<GLenum>(Target::Write), id);
        glCopyBufferSubData(static_cast<GLenum>(Target::Read), static_cast<GLenum>(Target::Write), 0, 0,
                            static_cast<GLsizeiptr>(typeSize * size));

        return *this;
    }

    Buffer::Buffer(Buffer &&buf) noexcept
        : id{std::exchange(buf.id, 0)},
          target{std::exchange(buf.target, Target::None)},
          size{std::exchange(buf.size, 0)},
          typeSize{std::exchange(buf.typeSize, 0)} { }

    fn Buffer::operator=(Buffer &&buf) noexcept -> Buffer & {
        if (this == &buf)
            return *this;

        id       = std::exchange(buf.id, 0);
        target   = std::exchange(buf.target, Target::None);
        size     = std::exchange(buf.size, 0);
        typeSize = std::exchange(buf.typeSize, 0);

        return *this;
    }

    Buffer::~Buffer() { dispose(); }

    template<typename T>
    fn Buffer::fill(core::usize _size, T const *_data, Usage _usage) -> Buffer & { return fill<T>(_size, _data, target, _usage); }

    template<typename T>
    fn Buffer::fill(std::vector<T> const &_buf, Usage _usage) -> Buffer & { return fill<T>(_buf.size(), _buf.data(), target, _usage); }

    template<typename T>
    fn Buffer::fill(T const &_val, Usage _usage) -> Buffer & { return fill<T>(1, &_val, target, _usage); }

    template<typename T>
    fn Buffer::fill(core::usize _size, T const *_data, Target _target, Usage _usage) -> Buffer & {
        size     = _size;
        typeSize = sizeof(T);
        target   = _target;

        glBufferData(static_cast<GLenum>(target), size * typeSize, _data, static_cast<GLenum>(_usage));

        return *this;
    }

    template<typename T>
    fn Buffer::fill(std::vector<T> const &_buf, Target _target, Usage _usage) -> Buffer & { return fill<T>(_buf.size(), _buf.data(), _target, _usage); }

    template<typename T>
    fn Buffer::fill(T const &_val, Target _target, Usage _usage) -> Buffer & { return fill<T>(1, &_val, _target, _usage); }

    template<typename T>
    fn Buffer::fetch(size_t _size, T *_data, GLintptr _offset) -> void {
        glGetBufferSubData(target, _offset, _size * sizeof(T), _data);
    }

    template<typename T>
    fn Buffer::fetch(std::vector<T> &_buf) -> void { fetch<T>(_buf.size(), _buf.data()); }

    fn Buffer::bind() -> Buffer & {
        glBindBuffer(static_cast<GLenum>(target), id);

        return *this;
    }

    fn Buffer::unbind() -> Buffer & {
        glBindBuffer(static_cast<GLenum>(target), 0);

        return *this;
    }

    fn Buffer::dispose() -> void {
        if (id != 0) {
            glDeleteBuffers(1, &id);
            id       = 0;
            target   = Target::None;
            size     = 0;
            typeSize = 0;
        }
    }
}

#endif //MICRO_GRAPHICS_BUFFER_H
