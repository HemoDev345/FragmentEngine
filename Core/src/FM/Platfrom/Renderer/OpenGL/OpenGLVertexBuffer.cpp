#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/VertexBuffer.hpp"
#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"
#include "glad/glad.h"

namespace fm
{
    VertexBuffer::VertexBuffer(const float* vertices, uint32_t size, USAGE_TYPE usage)
    {
        FM_GL_CALL(glGenBuffers(1, &m_id));
        FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
        FM_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, ConvertUsage(usage)));
    }

    VertexBuffer::~VertexBuffer()
    {
        Destroy();
    }

    void VertexBuffer::Bind() const
    {
        FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    }

    void VertexBuffer::Unbind() const
    {
        FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::SetData(const float* vertices, uint32_t size, USAGE_TYPE usage) const
    {
        FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
        FM_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) , vertices, ConvertUsage(usage)));
    }

    void VertexBuffer::Destroy() const
    {
        FM_GL_CALL(glDeleteBuffers(1, &m_id));
    }
}


#endif
