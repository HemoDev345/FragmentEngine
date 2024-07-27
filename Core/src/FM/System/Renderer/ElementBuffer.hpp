#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>


#include "FM/Core/GLCall.hpp"
#include "FM/Core/Log.hpp"

namespace fm
{
    struct Element
    {
        enum Type
        {
            INT, VEC2_INT, VEC3_INT, VEC4_INT, 
            FLOAT, VEC2_FLOAT, VEC3_FLOAT, VEC4_FLOAT, 
            MAT3, MAT4, 
            BOOL 
        };

        Type type;
        std::string name;
        int32_t offset;
        uint32_t size;
        bool normalized;

        Element(Type _type, const char* _name, bool _normalized)
            : name(_name), type(_type), size(type_size()), normalized(_normalized)
        {
            
        }

        int32_t type_count()
        {
            switch (type)
            {
                case INT:           return 1;
                case VEC2_INT:      return 2;
                case VEC3_INT:      return 3;
                case VEC4_INT:      return 4;
                case FLOAT:         return 1;
                case VEC2_FLOAT:    return 2;
                case VEC3_FLOAT:    return 3;
                case VEC4_FLOAT:    return 4;
                case MAT3:          return 3 * 3;
                case MAT4:          return 4 * 4;
                case BOOL:          return 1;
            };

            FM_LOG_ERROR("unkown type");
            return 0;
        }

        GLenum type_to_GLenum()
        {
            switch (type)
            {
                case INT:           return GL_INT;
                case VEC2_INT:      return GL_INT;
                case VEC3_INT:      return GL_INT;
                case VEC4_INT:      return GL_INT;
                case FLOAT:         return GL_FLOAT;
                case VEC2_FLOAT:    return GL_FLOAT;
                case VEC3_FLOAT:    return GL_FLOAT;
                case VEC4_FLOAT:    return GL_FLOAT;
                case MAT3:          return 0;
                case MAT4:          return 0;
                case BOOL:          return GL_BOOL;
            };

            FM_LOG_ERROR("unkown type");
            return 0;
        }

    private:
        uint32_t type_size()
        {
            switch (type)
            {
                case INT:           return 4;
                case VEC2_INT:      return 4 * 2;
                case VEC3_INT:      return 4 * 3;
                case VEC4_INT:      return 4 * 4;
                case FLOAT:         return 4;
                case VEC2_FLOAT:    return 4 * 2;
                case VEC3_FLOAT:    return 4 * 3;
                case VEC4_FLOAT:    return 4 * 4;
                case MAT3:          return 4 * 3 * 3;
                case MAT4:          return 4 * 4 * 4;
                case BOOL:          return 1;
            };

            FM_LOG_ERROR("unkown type");
            return 0;   
        }
    };


    class ElementBuffer
    {
    public:
        ElementBuffer(std::initializer_list<Element> elements)
            : m_elements(elements)
        {
            init_elements();

            FM_GL_CALL(glGenVertexArrays(1, &m_id));
            FM_GL_CALL(glBindVertexArray(m_id));
            // GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

            for (uint32_t i = 0; i < m_elements.size(); i++)
                FM_GL_CALL(glVertexAttribPointer(
                    i, 
                    m_elements[i].type_count(),
                    m_elements[i].type_to_GLenum(),
                    m_elements[i].normalized, 
                    m_stride,
                    (void*)m_elements[i].offset
                ));

        }

        void bind()
        {
            for (int i = 0; i < m_elements.size(); i++)
                FM_GL_CALL(glEnableVertexAttribArray(i));
            FM_GL_CALL(glBindVertexArray(m_id));
        }

        void unbind()
        {
            FM_GL_CALL(glBindVertexArray(0));
            for (int i = 0; i < m_elements.size(); i++)
                FM_GL_CALL(glDisableVertexAttribArray(i));
        }


    private:
        void init_elements()
        {
            uint32_t offset = 0u;
            m_stride = 0u;
            for (auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

private:
        std::vector<Element> m_elements;
        uint32_t m_id;
        uint32_t m_stride;

    };
}
