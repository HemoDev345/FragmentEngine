#pragma once

#include <fstream>

#include "FM/Core/GLCall.hpp"
#include "glad/glad.h"

namespace fm
{
    class Shader
    {
    public:

        Shader(const char* vert_path, const char* frag_path)
        {
            init(vert_path, frag_path);
        }

        ~Shader()
        {
            glDeleteProgram(m_program);
        }


        void init(const char* vert_path, const char* frag_path)
        {
            std::string frag_src = load_file(frag_path);
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

            std::string vertex_src = load_file(vert_path);
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

        void bind()
        {
            glUseProgram(m_program);
        }

        void unbind()
        {
            glUseProgram(0);
        }

    private:
        uint32_t m_program;

        std::string load_file(const char* path)
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
    };
}
