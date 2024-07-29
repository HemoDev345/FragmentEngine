#pragma once

#include <cstdint>
#include <fstream>

namespace fm
{
    class Shader
    {
    public:
        Shader(const char* vert_path, const char* frag_path);

        ~Shader();

        void Init(const char* vert_path, const char* frag_path);

        void Bind() const;

        void Unbind() const;

        void Destroy() const;

    private:
        uint32_t m_program;

        std::string LoadFile(const char* path);
    };
}
