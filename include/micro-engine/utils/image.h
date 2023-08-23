//
// Created by kbratko on 7/22/2023.
//

#ifndef MICRO_UTILS_IMAGE_H
#define MICRO_UTILS_IMAGE_H

#define STB_IMAGE_IMPLEMENTATION

#include <memory>
#include <sstream>

#include "stb-image.h"
#include "../core/types.h"

namespace micro::asset {
    class Image {
    public:
        core::i32                 width  = 0;
        core::i32                 height = 0;
        core::i32                 depth  = 0;
        std::shared_ptr<core::u8> pixels;

        fn error(core::Consumer<std::string const &> const &onError) -> Image &;

        fn load(core::cstring path, bool flip_vertically = true) -> Image &;

    private:
        core::Consumer<std::string const &> onError;
    };

    static fn image(core::cstring                              path,
                    bool                                       flip_vertically = true,
                    core::Consumer<std::string const &> const &on_error        = [](auto const &) {}) -> Image {
        Image image{};
        image
            .error(on_error)
            .load(path, flip_vertically);

        return image;
    }

    fn Image::error(core::Consumer<std::string const &> const &_onError) -> Image & {
        onError = _onError;

        return *this;
    }

    auto Image::load(core::cstring _path, bool _flip_vertically) -> Image & {
        stbi_set_flip_vertically_on_load(_flip_vertically);
        unsigned char *data = stbi_load(_path, &width, &height, &depth, 0);

        if (!data) {
            std::ostringstream out{};
            out << "could not load image from file " << _path;

            if (onError)
                onError(out.str());
            else
                throw std::runtime_error{out.str().c_str()};
        }

        pixels.reset(data);

        return *this;
    }
}

#endif //MICRO_UTILS_IMAGE_H
