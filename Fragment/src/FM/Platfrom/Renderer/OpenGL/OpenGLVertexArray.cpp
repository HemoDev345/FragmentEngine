#if defined(FM_RENDER_API_OPENGL)
#include "FM/Platfrom/Renderer/ElementLayout.hpp"
#include "FM/Platfrom/Renderer/VertexArray.hpp"

#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"
#include <glad/glad.h>

namespace fm
{
    VertexArray::VertexArray()
    {
        FM_GL_CALL(glGenVertexArrays(1, &m_id));
    }

    VertexArray::~VertexArray()
    {
        FM_GL_CALL(glDeleteVertexArrays(1, &m_id));
    }

    void VertexArray::Bind()
    {
        FM_GL_CALL(glBindVertexArray(m_id));
    }

    void VertexArray::Unbind()
    {
        FM_GL_CALL(glBindVertexArray(0));
    }

    void VertexArray::addVertexBuffer(VertexBuffer* vertex_buffer)
    {
        FM_GL_CALL(glBindVertexArray(m_id));
        
        ElementLayout& elements = vertex_buffer->GetElementLayout();
        for (int i = 0; i < elements.Size(); i++)
        {
            FM_GL_CALL(glEnableVertexAttribArray(i));
            FM_GL_CALL(glVertexAttribPointer(
                i,
                elements[i].TypeCount(), elements[i].ConvertType(),
                elements[i].normalized ? GL_TRUE : GL_FALSE,
                elements.GetStride(),
                (const void*)elements[i].offset
            ));
        }
        m_vertex_buffers.push_back(vertex_buffer);
    }

    void VertexArray::setIndexBuffer(IndexBuffer* index_buffer)
    {
        FM_GL_CALL(glBindVertexArray(m_id));
        index_buffer->Bind();
        m_index_buffer = index_buffer;
    }
}

#endif
