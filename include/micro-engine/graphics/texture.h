//
// Created by kbratko on 7/24/2023.
//

#ifndef MICRO_GRAPHICS_TEXTURE_H
#define MICRO_GRAPHICS_TEXTURE_H

#include <sstream>

#include <glad/glad.h>
#include "../core/types.h"
#include "../utils/image.h"

namespace micro::graphics {
    class Texture {
    public:
        enum class Target : GLenum {
            Line      = GL_TEXTURE_1D,
            Plane     = GL_TEXTURE_2D,
            Space     = GL_TEXTURE_3D,
            CubeMap   = GL_TEXTURE_CUBE_MAP,
            Rectangle = GL_TEXTURE_RECTANGLE
        };

        enum class Parameter : GLenum {
            WrapS       = GL_TEXTURE_WRAP_S,
            WrapT       = GL_TEXTURE_WRAP_T,
            WrapR       = GL_TEXTURE_WRAP_R,
            SwizzleR    = GL_TEXTURE_SWIZZLE_R,
            SwizzleG    = GL_TEXTURE_SWIZZLE_G,
            SwizzleB    = GL_TEXTURE_SWIZZLE_B,
            SwizzleA    = GL_TEXTURE_SWIZZLE_A,
            SwizzleRGBA = GL_TEXTURE_SWIZZLE_RGBA,
            MinFilter   = GL_TEXTURE_MIN_FILTER,
            MagFilter   = GL_TEXTURE_MAG_FILTER
        };

        enum class Value : GLint {
            // Wrap
            ClampToEdge         = GL_CLAMP_TO_EDGE,
            ClampToBorder       = GL_CLAMP_TO_BORDER,
            MirroredRepeat      = GL_MIRRORED_REPEAT,
            Repeat              = GL_REPEAT,
            MirroredClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
            // Swizzle
            Red   = GL_RED,
            Green = GL_GREEN,
            Blue  = GL_BLUE,
            Alpha = GL_ALPHA,
            Zero  = GL_ZERO,
            One   = GL_ONE,
            // MagFilter
            Nearest              = GL_NEAREST,
            Linear               = GL_LINEAR,
            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR
        };

        enum class InternalFormat : GLint {
            DepthComponent = GL_DEPTH_COMPONENT,
            DepthStencil   = GL_DEPTH_STENCIL,
            R              = GL_RED,
            RG             = GL_RG,
            RGB            = GL_RGB,
            RGBA           = GL_RGBA,
        };

        enum class PixelFormat : GLenum {
            R              = GL_RED,
            RG             = GL_RG,
            RGB            = GL_RGB,
            BGR            = GL_BGR,
            RGBA           = GL_RGBA,
            BGRA           = GL_BGRA,
            RInt           = GL_RED_INTEGER,
            RGInt          = GL_RG_INTEGER,
            RGBInt         = GL_RGB_INTEGER,
            BGRInt         = GL_BGR_INTEGER,
            RGBAInt        = GL_RGBA_INTEGER,
            BGRAInt        = GL_BGRA_INTEGER,
            StencilIndex   = GL_STENCIL_INDEX,
            DepthComponent = GL_DEPTH_COMPONENT,
            DepthStencil   = GL_DEPTH_STENCIL
        };

        GLuint id = 0;
        Target target;

        Texture();

        explicit Texture(Target target);

        Texture(Texture const &tex) = delete;

        fn operator=(Texture const &tex) -> Texture & = delete;

        Texture(Texture &&tex) noexcept;

        fn operator=(Texture &&tex) noexcept -> Texture &;

        ~Texture();

        fn dispose() -> void;

        fn bind() -> Texture &;

        fn bindOnUnit(GLuint index) -> Texture &;

        fn unbind() -> Texture &;

        fn parameter(Parameter parameter, GLfloat value) -> Texture &;

        fn parameter(Parameter parameter, GLint value) -> Texture &;

        fn parameter(Parameter parameter, Value value) -> Texture &;

        fn image(asset::Image const &image) -> Texture &;

        fn image(asset::Image const &image, InternalFormat iFormat, PixelFormat pFormat) -> Texture &;

        fn (error)(core::Consumer<std::string const &> const &handler) -> Texture &;

        static fn activateUnit(GLuint index) -> void;

    private:
        core::Consumer<std::string const &> onError;
    };

    fn texture() -> Texture { return Texture{}; }

    fn texture(Texture::Target target) -> Texture { return Texture{target}; }

    fn texture(Texture::Target                                                   target,
               asset::Image const &                                              image,
               Texture::InternalFormat                                           iFormat,
               Texture::PixelFormat                                              pFormat,
               std::vector<std::pair<Texture::Parameter, Texture::Value>> const &parameters,
               core::Consumer<std::string const &> const &                       handler = [](auto const &) {}) -> Texture {
        auto tex = texture(target);

        tex
            .bind()
            .error(handler);

        std::for_each(parameters.begin(), parameters.end(),
                      [&](auto const &source) { tex.parameter(source.first, source.second); });

        tex
            .image(image, iFormat, pFormat)
            .unbind();

        return tex;
    }

    fn texture(Texture::Target                                                   target,
               asset::Image const &                                              image,
               std::vector<std::pair<Texture::Parameter, Texture::Value>> const &parameters,
               core::Consumer<std::string const &> const &                       handler = [](auto const &) {}) -> Texture {
        auto tex = texture(target);

        tex
            .bind()
            .error(handler);

        std::for_each(parameters.begin(), parameters.end(),
                      [&](auto const &source) { tex.parameter(source.first, source.second); });

        tex
            .image(image)
            .unbind();

        return tex;
    }

    Texture::Texture() : Texture(Target::Plane) { }

    Texture::Texture(Texture::Target _target) : target{_target} {
        glGenTextures(1, &id);
    }

    Texture::~Texture() { dispose(); }

    Texture::Texture(Texture &&tex) noexcept: id{std::exchange(tex.id, 0)},
                                              target{tex.target},
                                              onError{std::exchange(tex.onError, {})} {}

    auto Texture::operator=(Texture &&tex) noexcept -> Texture & {
        if (this == &tex)
            return *this;

        id      = std::exchange(tex.id, 0);
        onError = std::exchange(tex.onError, {});
        target  = tex.target;

        return *this;
    }

    fn Texture::dispose() -> void {
        if (id) {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }

    fn Texture::bind() -> Texture & {
        glBindTexture(static_cast<GLenum>(target), id);

        return *this;
    }

    fn Texture::bindOnUnit(GLuint _index) -> Texture & {
        Texture::activateUnit(_index);

        return bind();
    }

    fn Texture::unbind() -> Texture & {
        glBindTexture(static_cast<GLenum>(target), id);

        return *this;
    }

    fn Texture::parameter(Texture::Parameter _parameter, GLfloat _value) -> Texture & {
        glTexParameterf(static_cast<GLenum>(target), static_cast<GLenum>(_parameter), _value);

        return *this;
    }

    fn Texture::parameter(Texture::Parameter _parameter, GLint _value) -> Texture & {
        glTexParameteri(static_cast<GLenum>(target), static_cast<GLenum>(_parameter), _value);

        return *this;
    }

    fn Texture::parameter(Texture::Parameter _parameter, Texture::Value _value) -> Texture & { return Texture::parameter(_parameter, static_cast<GLint>(_value)); }

    fn Texture::image(asset::Image const &_image) -> Texture & {
        GLint format;
        switch (_image.depth) {
            case 1:
                format = GL_RED;
                break;
            case 2:
                format = GL_RG;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                std::ostringstream out{};
                out << "error while trying to determine texture data format, provided depth: " << _image.depth;

                if (onError)
                    onError(out.str());
                else
                    throw std::runtime_error{out.str().c_str()};
                break;
        }

        glTexImage2D(static_cast<GLenum>(target),
                     0,
                     format,
                     _image.width,
                     _image.height,
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     _image.pixels.get());
        glGenerateMipmap(static_cast<GLenum>(target));

        return *this;
    }

    fn Texture::image(asset::Image const & _image, Texture::InternalFormat _iFormat,
                      Texture::PixelFormat _pFormat) -> Texture & {
        glTexImage2D(static_cast<GLenum>(target),
                     0,
                     static_cast<GLint>(_iFormat),
                     _image.width,
                     _image.height,
                     0,
                     static_cast<GLenum>(_pFormat),
                     GL_UNSIGNED_BYTE,
                     _image.pixels.get());
        glGenerateMipmap(static_cast<GLenum>(target));

        return *this;
    }

    fn Texture::error(core::Consumer<std::string const &> const &_onError) -> Texture & {
        onError = _onError;

        return *this;
    }

    fn Texture::activateUnit(GLuint _index) -> void {
        glActiveTexture(GL_TEXTURE0 + _index);
    }
}

#endif //MICRO_GRAPHICS_TEXTURE_H
