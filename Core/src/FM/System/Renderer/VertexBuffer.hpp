#pragma once

#include "FM/Core/GLCall.hpp"
#include "glad/glad.h"


namespace fm
{
    class VertexBuffer
    {
    public:
        enum STREAM_TYPE
        {
            STREAM_DRAW,
            STREAM_READ,
            STREAM_COPY,
            STATIC_DRAW,
            STATIC_READ,
            STATIC_COPY,
            DYNAMIC_DRAW,
            DYNAMIC_READ,
            DYNAMIC_COPY
        };

        VertexBuffer(const float* vertices, unsigned size, STREAM_TYPE usage)
        {
            
            FM_GL_CALL(glGenBuffers(1, &m_id));
            FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
            FM_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, convert_GLusage(usage)));
        }

        ~VertexBuffer()
        {
            destroy();
        }

        void bind() const
        {
            FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
        }

        void unbind() const
        {
            FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        void set_data(const float* vertices, unsigned size, STREAM_TYPE usage)
        {
            FM_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
            FM_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) , vertices, convert_GLusage(usage)));
        }

        void destroy()
        {
            FM_GL_CALL(glDeleteBuffers(1, &m_id));
        }

    private:
        unsigned m_id;

        int convert_GLusage(STREAM_TYPE usage)
        {
            switch (usage)
            {
            case STREAM_TYPE::STREAM_DRAW:
                return GL_STREAM_DRAW;
            
            case STREAM_TYPE::STREAM_COPY:
                return GL_STREAM_COPY;
            
            case STREAM_TYPE::STREAM_READ:
                return GL_STREAM_READ;
            
            case STREAM_TYPE::STATIC_DRAW:
                return GL_STATIC_DRAW;
            
            case STREAM_TYPE::STATIC_COPY:
                return GL_STATIC_COPY;
            
            case STREAM_TYPE::STATIC_READ:
                return GL_STATIC_READ;
            
            case STREAM_TYPE::DYNAMIC_DRAW:
                return GL_DYNAMIC_DRAW;
            
            case STREAM_TYPE::DYNAMIC_COPY:
                return GL_DYNAMIC_DRAW;
            
            case STREAM_TYPE::DYNAMIC_READ:
                return GL_DYNAMIC_DRAW;
            }

            FM_LOG_ERROR("unkown usage {}", (int)usage);
            return 0;
        }
    };

    
}
