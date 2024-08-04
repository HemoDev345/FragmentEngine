#pragma once

#include <vector>
#include <string>

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

        Element(Type _type, const char* _name, bool _normalized);

        int32_t TypeCount();

        uint32_t ConvertType();

    private:
        uint32_t TypeSize();
    };


    class ElementBuffer
    {
    public:
        ElementBuffer(std::initializer_list<Element> elements);

        void Bind() const;

        void Unbind() const;

        void setData(std::initializer_list<Element> elements);

        void Destroy() const;

    private:
        void InitElements();

private:
        std::vector<Element> m_elements;
        uint32_t m_id;
        uint32_t m_stride;

    };
}
