#pragma once

#include <cstdint>
#include "FM/Platfrom/Renderer/UsageType.hpp"
#include "FM/Platfrom/Renderer/ElementLayout.hpp"

namespace fm
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const float* vertices, uint32_t size, USAGE_TYPE usage);

        ~VertexBuffer();

        void Bind() const;

        void Unbind() const;
        void SetData(const float* vertices, uint32_t size, USAGE_TYPE usage) const;
        
        void SetElementLayout(const ElementLayout& element_layout) { m_element_layout = element_layout; }
        ElementLayout& GetElementLayout() { return m_element_layout; }

        void Destroy() const;

    private:
        uint32_t m_id;
        ElementLayout m_element_layout;

    };

    
}
