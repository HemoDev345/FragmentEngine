#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"
#include "FM/Platfrom/Renderer/IndexBuffer.hpp"
#include "glad/glad.h"


namespace fm
{
    IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t size, USAGE_TYPE usage)
    {
        FM_GL_CALL(glGenBuffers(1, &m_id));
        FM_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
        FM_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * size , indices, ConvertUsage(usage)));
    }

    IndexBuffer::~IndexBuffer()
    {
        Destroy();
    }

    void IndexBuffer::Bind() const
    {
        FM_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

    void IndexBuffer::Unbind() const
    {
        FM_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::SetData(const uint32_t* indices, uint32_t size, USAGE_TYPE usage) const
    {
        FM_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
        FM_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size , indices, ConvertUsage(usage)));
    }

    void IndexBuffer::Destroy() const
    {
        FM_GL_CALL(glDeleteBuffers(1, &m_id));
    }

}

#endif