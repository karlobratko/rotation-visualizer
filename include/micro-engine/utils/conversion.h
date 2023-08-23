//
// Created by kbratko on 7/26/2023.
//

#ifndef MICRO_UTILS_CONVERSION_H
#define MICRO_UTILS_CONVERSION_H

#include <typeindex>
#include <unordered_map>

#include <glad/glad.h>
#include "../core/types.h"

namespace micro::utils {
    template<typename T>
        requires std::is_arithmetic_v<T>
    fn typeToGLenum() -> GLenum {
        static std::unordered_map<std::type_index, GLenum> const typeMap{
            {std::type_index{typeid(GLfloat)}, GL_FLOAT},
            {std::type_index{typeid(GLdouble)}, GL_DOUBLE},
            {std::type_index{typeid(GLbyte)}, GL_BYTE},
            {std::type_index{typeid(GLubyte)}, GL_UNSIGNED_BYTE},
            {std::type_index{typeid(GLshort)}, GL_SHORT},
            {std::type_index{typeid(GLushort)}, GL_UNSIGNED_SHORT},
            {std::type_index{typeid(GLint)}, GL_INT},
            {std::type_index{typeid(GLuint)}, GL_UNSIGNED_INT}
        };

        return typeMap.at(std::type_index{typeid(T)});
    }
}

#endif //MICRO_UTILS_CONVERSION_H
