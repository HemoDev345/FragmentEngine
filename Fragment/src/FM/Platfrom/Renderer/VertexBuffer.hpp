#pragma once

#include <cstdint>
#include "FM/Platfrom/Renderer/UsageType.hpp"

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

        void Destroy() const;

    private:
        uint32_t m_id;

    };

    
}
