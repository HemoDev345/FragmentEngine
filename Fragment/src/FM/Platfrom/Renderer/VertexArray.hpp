#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <vector>

namespace fm
{
    class VertexArray
    {
    public:
        VertexArray();

        ~VertexArray();

        void Bind();

        void Unbind();

        void addVertexBuffer(VertexBuffer* vertex_buffer);
        void setIndexBuffer(IndexBuffer* index_buffer);

    private:
        std::vector<VertexBuffer*> m_vertex_buffers;
        IndexBuffer* m_index_buffer;
        uint32_t m_id;

    };
}

