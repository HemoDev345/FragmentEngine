#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/ElementLayout.hpp"
#include "FM/Core/Log.hpp"
#include <glad/glad.h>

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

    ElementLayout::ElementLayout(std::initializer_list<Element> elements)
        : m_elements(elements)
    {
        InitElements();
    }

    ElementLayout::ElementLayout()
    {
    }

    void ElementLayout::SetData(std::initializer_list<Element> elements)
    {
        m_elements = elements;
        InitElements();
    }

    void ElementLayout::InitElements()
    {
        uint32_t offset = 0u;
        m_stride = 0u;
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

