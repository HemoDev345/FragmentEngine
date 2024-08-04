#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/Shader.hpp"
#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"
#include "FM/Core/Log.hpp"
#include "glad/glad.h"

namespace fm
{
    Shader::Shader(const char* vert_path, const char* frag_path)
    {
        Init(vert_path, frag_path);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
    }

    void Shader::Init(const char* vert_path, const char* frag_path)
    {
        std::string frag_src = LoadFile(frag_path);
        const char* frag_str = frag_src.c_str();

        uint32_t frag;
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &frag_str, NULL);
        glCompileShader(frag);

        int frag_success;
        glGetShaderiv(frag, GL_COMPILE_STATUS, &frag_success);
        if(!frag_success)
        {
            char infoLog[512];
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            FM_LOG_ERROR("fragment shader compilation failed {}", infoLog);
        }

        std::string vertex_src = LoadFile(vert_path);
        const char* vertex_str = vertex_src.c_str();

        uint32_t vert;
        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vertex_str, NULL);
        glCompileShader(vert);

        int vertex_success;
        glGetShaderiv(vert, GL_COMPILE_STATUS, &vertex_success);
        if(!vertex_success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vert, 512, NULL, infoLog);
            FM_LOG_ERROR("vertex shader compilation failed {}", infoLog);
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, vert);
        glAttachShader(m_program, frag);
        glLinkProgram(m_program);

        int program_success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &program_success);
        if(!program_success) {
            char infoLog[512];
            glGetProgramInfoLog(m_program, 512, NULL, infoLog);
            FM_LOG_ERROR("program linking failed {}", infoLog);
            
        }

        glDetachShader(m_program, vert);
        glDetachShader(m_program, frag);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_program);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::Destroy() const
    {
        glDeleteProgram(m_program);
    }

    std::string Shader::LoadFile(const char* path)
    {
        std::ifstream file(path);
        std::string line;
        std::string out;

        if (file.is_open())
        {
            while (file) 
            { // equivalent to myfile.good()
                std::getline(file, line);
                out += line + '\n';
            }
        }
        else 
        {
            FM_LOG_ERROR("couldn't open {}", path);
        }
        return out;

    }
}

#endif
