#include "FM/FM.hpp"
#include "tracy/Tracy.hpp"
#include <math.h>


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

            unsigned int indices[] = {  // note that we start from 0!
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };  

            m_vertex_buffer = new fm::VertexBuffer(vertices, sizeof(vertices), fm::VertexBuffer::STATIC_DRAW);

            m_index_buffer = new fm::IndexBuffer(indices, sizeof(indices), fm::IndexBuffer::STATIC_DRAW);
            
            int s = sizeof(float) * 7;

            m_element_buffer = new fm::ElementBuffer{
                {fm::Element::Type::VEC3_FLOAT, "gPos", false},
                {fm::Element::Type::VEC4_FLOAT, "gColor", false},
            };

            m_shader = new fm::Shader("Fragmentbox/src/VertexShader.vert", "Fragmentbox/src/FragmentShader.frag");
    }

    void render(SDL_Window* window) override
    {
        FM_PROFILE_SCOPE("render");
        FM_GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        FM_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        m_vertex_buffer->bind();

        m_element_buffer->bind();

        m_index_buffer->bind();

        m_shader->bind();

        FM_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        m_element_buffer->unbind();

        m_shader->unbind();

        SDL_GL_SwapWindow(window);
    }

    void destory() override
    {
        
    }

private:
    fm::VertexBuffer* m_vertex_buffer;
    fm::IndexBuffer* m_index_buffer;
    fm::ElementBuffer* m_element_buffer;
    fm::Shader* m_shader;

};


fm::Game* getGame()
{
    fm::Game* game = new fm::Game("Test", 640, 420);
    game->push(new FragmentBox());
    return game;
}

// struct ExportGame {
//     fm::Game*(*get)(void);
// };

// struct ExportScene export_scene {
//     getScene
// };

