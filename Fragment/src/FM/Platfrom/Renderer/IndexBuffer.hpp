#pragma once

#include <cstdint>
#include "FM/Platfrom/Renderer/UsageType.hpp"

namespace fm
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const uint32_t* indices, uint32_t size, USAGE_TYPE usage);

        ~IndexBuffer();

        void Bind() const;

        void Unbind() const;

        void SetData(const uint32_t* indices, uint32_t size, USAGE_TYPE usage) const;

        void Destroy() const;

    private:
        unsigned m_id;

    };

    
}
