#include <glad/glad.h>
#include "FM/FM.hpp"
#include "tracy/Tracy.hpp"
#include "FM/Platfrom/Renderer/OpenGL/GLCall.hpp"

class FragmentBox : public fm::Scene
{
public:
    FragmentBox()
    {

    }

    ~FragmentBox()
    {

    }
    
    void startup() override
    {
        const float vertices[] = {
                0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };  

        m_vertex_array = new fm::VertexArray();
        
        m_vertex_buffer = new fm::VertexBuffer(vertices, sizeof(vertices), fm::STATIC_DRAW);

        m_index_buffer = new fm::IndexBuffer(indices, sizeof(indices), fm::STATIC_DRAW);
        
        m_vertex_buffer->SetElementLayout(fm::ElementLayout{
            {fm::Element::Type::VEC3_FLOAT, "gPos", false},
            {fm::Element::Type::VEC4_FLOAT, "gColor", false},
        });
        
        m_vertex_array->addVertexBuffer(m_vertex_buffer);
        m_vertex_array->setIndexBuffer(m_index_buffer);
        
        m_shader = new fm::Shader("Fragmentbox/src/VertexShader.vert", "Fragmentbox/src/FragmentShader.frag");
    }

    void render(fm::Window& window) override
    {
        FM_PROFILE_SCOPE("render");
        FM_GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        FM_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        
        m_vertex_array->Bind();
        m_shader->Bind();

        FM_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        window.Display();
    }

    void destory() override
    {
        
    }

private:
    fm::VertexBuffer* m_vertex_buffer;
    fm::IndexBuffer* m_index_buffer;
    fm::Shader* m_shader;
    fm::VertexArray* m_vertex_array;
};


fm::Game* getGame()
{
    fm::Game* game = new fm::Game("Test", 640, 420);
    game->push(new FragmentBox());
    return game;
}

