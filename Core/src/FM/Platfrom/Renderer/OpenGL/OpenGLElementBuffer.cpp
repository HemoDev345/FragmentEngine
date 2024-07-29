#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/ElementBuffer.hpp"

#include <glad/glad.h>
#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"
#include "FM/Core/Log.hpp"

namespace fm
{
    Element::Element(Element::Type _type, const char* _name, bool _normalized)
        : name(_name), type(_type), size(TypeSize()), normalized(_normalized)
    {
        
    }

    int32_t Element::TypeCount()
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

    uint32_t Element::ConvertType()
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

    uint32_t Element::TypeSize()
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

    ElementBuffer::ElementBuffer(std::initializer_list<Element> elements)
        : m_elements(elements)
    {
        InitElements();

        FM_GL_CALL(glGenVertexArrays(1, &m_id));
        FM_GL_CALL(glBindVertexArray(m_id));
        // GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

        for (uint32_t i = 0; i < m_elements.size(); i++)
            FM_GL_CALL(glVertexAttribPointer(
                i, 
                m_elements[i].TypeCount(),
                m_elements[i].ConvertType(),
                m_elements[i].normalized, 
                m_stride,
                (void*)m_elements[i].offset
            ));

    }

    void ElementBuffer::Bind() const
    {
        for (int i = 0; i < m_elements.size(); i++)
            FM_GL_CALL(glEnableVertexAttribArray(i));
        FM_GL_CALL(glBindVertexArray(m_id));
    }

    void ElementBuffer::Unbind() const
    {
        FM_GL_CALL(glBindVertexArray(0));
        for (int i = 0; i < m_elements.size(); i++)
            FM_GL_CALL(glDisableVertexAttribArray(i));
    }


    void ElementBuffer::setData(std::initializer_list<Element> elements)
    {
        m_elements = elements; 
        InitElements();

        FM_GL_CALL(glGenVertexArrays(1, &m_id));
        FM_GL_CALL(glBindVertexArray(m_id));

        for (uint32_t i = 0; i < m_elements.size(); i++)
            FM_GL_CALL(glVertexAttribPointer(
                i, 
                m_elements[i].TypeCount(),
                m_elements[i].ConvertType(),
                m_elements[i].normalized, 
                m_stride,
                (void*)m_elements[i].offset
            ));
    }

    void ElementBuffer::Destroy() const
    {
        FM_GL_CALL(glDeleteVertexArrays(1, &m_id));
    }

    void ElementBuffer::InitElements()
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


}

#endif

