//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_GRAPHICS_SHADER_H
#define MICRO_GRAPHICS_SHADER_H

#include <glad/glad.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "texture.h"
#include "../core/types.h"
#include "../mathematics/linear.h"

namespace micro::graphics {
    class Shader {
    public:
        enum class Type : GLenum {
            Compute        = GL_COMPUTE_SHADER,
            Vertex         = GL_VERTEX_SHADER,
            Fragment       = GL_FRAGMENT_SHADER,
            Geometry       = GL_GEOMETRY_SHADER,
            TessControl    = GL_TESS_CONTROL_SHADER,
            TessEvaluation = GL_TESS_EVALUATION_SHADER
        };

        GLuint                       id;
        std::map<std::string, GLint> attributes;
        std::map<std::string, GLint> uniforms;

        Shader();

        Shader(Shader const &shader) = delete;

        fn operator=(Shader const &shader) -> Shader & = delete;

        Shader(Shader &&shader) noexcept;

        fn operator=(Shader &&shader) noexcept -> Shader &;

        ~Shader();

        fn link() -> Shader &;

        fn bind() -> Shader &;

        fn unbind() -> Shader &;

        fn dispose() -> void;

        fn file(Type type, std::string const &fileName) -> Shader &;

        fn source(Type type, std::string const &source) -> Shader &;

        fn attribute(GLuint location, std::string const &attribute) -> Shader &;

        fn attribute(std::string const &attribute) -> GLint;

        template<typename T>
        fn uniform(std::string const &name, T &u) -> Shader &;

        template<typename T, std::size_t N>
        fn uniform(std::string const &name, T const (&u)[N]) -> Shader &;

        fn uniform(std::string const &uniform) -> GLint;

        fn error(core::Consumer<std::string const &> const &onError) -> Shader &;

    private:
        std::vector<GLuint>                 shaders;
        core::Consumer<std::string const &> onError;
    };

    fn shader(std::vector<std::pair<Shader::Type, core::cstring>> const &_sources,
              core::Consumer<std::string const &> const &                _onError = [](auto const &) {}) -> Shader {
        Shader shader{};

        shader
            .bind()
            .error(_onError);

        std::for_each(_sources.begin(), _sources.end(),
                      [&](auto const &source) { shader.source(source.first, source.second); });

        shader
            .link()
            .unbind();
        return std::move(shader);
    }

    Shader::Shader() : id{glCreateProgram()}, attributes{}, uniforms{}, shaders{}, onError{} {}

    Shader::Shader(Shader &&shader) noexcept: id{std::exchange(shader.id, 0)},
                                              onError{std::exchange(shader.onError, {})} {
        uniforms = shader.uniforms;
        shader.uniforms.clear();
        attributes = shader.attributes;
        shader.attributes.clear();
        shaders = shader.shaders;
        shader.shaders.clear();
    }

    auto Shader::operator=(Shader &&shader) noexcept -> Shader & {
        if (this == &shader)
            return *this;

        id       = std::exchange(shader.id, 0);
        onError  = std::exchange(shader.onError, {});
        uniforms = shader.uniforms;
        shader.uniforms.clear();
        attributes = shader.attributes;
        shader.attributes.clear();
        shaders = shader.shaders;
        shader.shaders.clear();

        return *this;
    }

    Shader::~Shader() { dispose(); }

    fn Shader::file(Type _type, std::string const &_fileName) -> Shader & {
        std::ifstream file{_fileName};
        if (file.is_open()) {
            std::string buffer{std::istreambuf_iterator<char>{file}, {}};
            source(_type, buffer);

            file.close();
        }
        else {
            std::ostringstream out{};
            out << "could not access file" << _fileName;

            if (onError)
                onError(out.str());
            else
                throw std::runtime_error{out.str().c_str()};
        }

        return *this;
    }

    fn Shader::source(Type _type, std::string const &_source) -> Shader & {
        GLuint      shader  = glCreateShader(static_cast<GLenum>(_type));
        char const *csource = _source.c_str();
        glShaderSource(shader, 1, &csource, nullptr);

        glCompileShader(shader);
        auto compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            GLint infoLogSize;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);

            auto *infoLog = new GLchar[infoLogSize];
            glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);

            std::ostringstream out{};
            out << "error compiling the " << static_cast<GLenum>(_type) << " shader\n" << infoLog;

            delete[] infoLog;

            if (onError)
                onError(out.str());
            else
                throw std::runtime_error{out.str().c_str()};
        }

        shaders.push_back(shader);

        return *this;
    }

    fn Shader::attribute(GLuint _location, std::string const &_attribute) -> Shader & {
        glBindAttribLocation(id, _location, _attribute.c_str());

        if (!attributes.contains(_attribute)) attributes.emplace(_attribute, _location);

        return *this;
    }

    fn Shader::attribute(std::string const &_attribute) -> GLint {
        auto it = attributes.find(_attribute);
        if (it != std::end(attributes)) return it->second;

        GLint location = glGetAttribLocation(id, _attribute.c_str());
        if (location == -1) {
            std::ostringstream out{};
            out << "could not find attribute " << _attribute;

            if (onError)
                onError(out.str());
            else
                throw std::runtime_error{out.str().c_str()};
        }

        attributes.emplace(_attribute, location);
        return location;
    }

    fn Shader::uniform(std::string const &_uniform) -> GLint {
        auto it = uniforms.find(_uniform);
        if (it != std::end(uniforms)) return it->second;

        GLint location = glGetUniformLocation(id, _uniform.c_str());
        if (location == -1) {
            std::ostringstream out{};
            out << "could not find uniform " << _uniform;

            if (onError)
                onError(out.str());
            else
                throw std::runtime_error{out.str().c_str()};
        }

        uniforms.emplace(_uniform, location);
        return location;
    }

    fn Shader::link() -> Shader & {
        std::for_each(std::begin(shaders), std::end(shaders), [&](GLuint shdrId) {
            if (shdrId != 0)
                glAttachShader(id, shdrId);
        });

        {
            auto linkStatus = 0;
            glLinkProgram(id);
            glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == GL_FALSE) {
                GLint infoLogSize;
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);

                auto *infoLog = new GLchar[infoLogSize];
                glGetProgramInfoLog(id, infoLogSize, nullptr, infoLog);

                std::ostringstream out{};
                out << "error linking " << id << " id\n" << infoLog;

                delete[] infoLog;

                if (onError)
                    onError(out.str());
                else
                    throw std::runtime_error{out.str().c_str()};
            }
        }

        {
            auto validationStatus = 0;
            glValidateProgram(id);
            glGetProgramiv(id, GL_VALIDATE_STATUS, &validationStatus);
            if (validationStatus == GL_FALSE) {
                GLint infoLogSize;
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);

                auto *infoLog = new GLchar[infoLogSize];
                glGetProgramInfoLog(id, infoLogSize, nullptr, infoLog);

                std::ostringstream out{};
                out << "error validating " << id << " id\n" << infoLog;

                delete[] infoLog;

                if (onError)
                    onError(out.str());
                else
                    throw std::runtime_error{out.str().c_str()};
            }
        }

        std::for_each(
            std::begin(shaders), std::end(shaders),
            [&](GLuint shdrId) {
                if (shdrId != 0) {
                    glDetachShader(id, shdrId);
                    glDeleteShader(shdrId);
                }
            }
        );

        return *this;
    }

    fn Shader::bind() -> Shader & {
        glUseProgram(id);
        return *this;
    }

    fn Shader::unbind() -> Shader & {
        glUseProgram(0);
        return *this;
    }

    fn Shader::error(core::Consumer<std::string const &> const &_onError) -> Shader & {
        onError = _onError;

        return *this;
    }

    fn Shader::dispose() -> void {
        if (id) {
            glDeleteProgram(id);
            id = 0;

            shaders.clear();
            attributes.clear();
            uniforms.clear();
        }
    }

    template<typename T>
    fn Shader::uniform(std::string const &name, T &u) -> Shader & {
        (void)u;

        std::ostringstream out{};
        out << "data type not recognized for uniform " << name;

        if (onError)
            onError(out.str());
        else
            throw std::runtime_error{out.str().c_str()};
    }

    template<typename T, size_t N>
    fn Shader::uniform(std::string const &name, T const (&u)[N]) -> Shader & {
        (void)u;

        std::ostringstream out{};
        out << "data type not recognized for uniform " << name;

        if (onError)
            onError(out.str());
        else
            throw std::runtime_error{out.str().c_str()};
    }

    template<>
    fn Shader::uniform(std::string const &name, bool const &u) -> Shader & {
        glUniform1i(uniform(name), u);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, int const &u) -> Shader & {
        glUniform1i(uniform(name), u);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, core::u32 &u) -> Shader & {
        glUniform1ui(uniform(name), u);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, Texture &u) -> Shader & {
        glUniform1ui(uniform(name), u.id);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, Texture const &u) -> Shader & {
        glUniform1ui(uniform(name), u.id);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, float const &u) -> Shader & {
        glUniform1f(uniform(name), u);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, double const &u) -> Shader & {
        glUniform1f(uniform(name), (float)u);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, float const (&u)[3]) -> Shader & {
        glUniform3fv(uniform(name), 1, &u[0]);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, float const (&u)[4]) -> Shader & {
        glUniform4fv(uniform(name), 1, &u[0]);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector2<core::f32> const &u) -> Shader & {
        glUniform2fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector2<core::i32> const &u) -> Shader & {
        glUniform2iv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector3<core::f32> const &u) -> Shader & {
        glUniform3fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector4<core::f32> const &u) -> Shader & {
        glUniform4fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::matrix3x3<core::f32> const &u) -> Shader & {
        glUniformMatrix3fv(uniform(name), 1, GL_FALSE, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::matrix4x4<core::f32> const &u) -> Shader & {
        glUniformMatrix4fv(uniform(name), 1, GL_FALSE, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, std::vector<math::matrix4x4<core::f32>> const &u) -> Shader & {
        glUniformMatrix4fv(uniform(name), static_cast<GLsizei>(u.size()), GL_FALSE, &u[0][0][0]);

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector2<core::f32> &u) -> Shader & {
        glUniform2fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector2<core::i32> &u) -> Shader & {
        glUniform2iv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector3<core::f32> &u) -> Shader & {
        glUniform3fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::vector4<core::f32> &u) -> Shader & {
        glUniform4fv(uniform(name), 1, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::matrix3x3<core::f32> &u) -> Shader & {
        glUniformMatrix3fv(uniform(name), 1, GL_FALSE, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, math::matrix4x4<core::f32> &u) -> Shader & {
        glUniformMatrix4fv(uniform(name), 1, GL_FALSE, math::value_ptr(u));

        return *this;
    }

    template<>
    fn Shader::uniform(std::string const &name, std::vector<math::matrix4x4<core::f32>> &u) -> Shader & {
        glUniformMatrix4fv(uniform(name), static_cast<GLsizei>(u.size()), GL_FALSE, &u[0][0][0]);

        return *this;
    }
}

#endif //MICRO_GRAPHICS_SHADER_H
