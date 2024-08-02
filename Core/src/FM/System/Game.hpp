#pragma once    

#include <stack>
#include <SDL2/SDL.h>

#include "FM/Core/Assret.hpp"
#include "FM/Core/Profile.hpp"

#include "FM/System/Scene.hpp"
#include "FM/System/Time/Time.hpp"

#include "FM/Platfrom/Renderer/API.hpp"

#include "FM/Platfrom/Window/Window.hpp"
#include <GL/glx.h>

namespace fm
{
    class Game
    {
    public:
        Game(const char* name, unsigned int w, unsigned h)
        {
            Time::init();

            m_window.Init({360, 360}, {620, 460}, "Hello");


            Renderer::Init(m_window);
        }

        ~Game()
        {

        }

        void push(fm::Scene* scene) 
        {
            if (m_running == true)
            {
                if (m_scene_stack.size() > 0)
                    m_scene_stack.top()->destory();
                scene->startup();
            }

         	m_scene_stack.push(scene);    
        }

        void pop()
        {
            m_scene_stack.top()->destory();
            m_scene_stack.pop();
		
         	m_scene_stack.top()->startup();    
        }

        void run()
        {
			m_running = true;
            FM_ASSERT(m_scene_stack.size() != 0, "no scene was push");
			m_scene_stack.top()->startup();
            
            while (m_quit == false)
            {
                fm::Time::restart();
                FM_PROFILE_FRAME;
                update();

                render();

                pollEvents();
            }

            quit();
        }
    private:
         void update()
        {
            FM_PROFILE_FUNC;
            m_scene_stack.top()->update(); 
        }

        void render()
        {
            m_scene_stack.top()->render(NULL);


            m_window.Display();
        } 


        void pollEvents()
        {
            FM_PROFILE_FUNC;
            
            m_quit = !m_window.PollEvent();            
        }

        void quit()
        {
            FM_PROFILE_FUNC;
            // m_renderer.stop();
            m_scene_stack.top()->destory();
            while (m_scene_stack.size() != 0)
            {
                m_scene_stack.pop();
            }

            m_quit = true;
            m_window.Shutdown();
        }

    private:
        Window m_window;

        // fm::Renderer m_renderer;
        // std::mutex m_render_mutex;

        std::stack<fm::Scene*> m_scene_stack;
        // TTF_Font* m_default_font;
        // uint16_t m_next_sec;
        // uint16_t m_last_frame;
        // SDL_Texture* m_fps_texture;
        // uint16_t m_fps;

        bool m_quit;
        bool m_running;
    };
};

