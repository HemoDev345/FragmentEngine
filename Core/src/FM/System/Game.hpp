#pragma once    

#include <stack>
#include <SDL2/SDL.h>

#include "FM/Core/Assret.hpp"
#include "FM/Core/Profile.hpp"

#include "FM/System/Scene.hpp"
#include "FM/System/Time/Time.hpp"
#include "FM/System/Input/Keyboard.hpp"
#include "FM/System/Input/Mouse.hpp"

#include "FM/Platfrom/Renderer/API.hpp"

#include "FM/Platfrom/Window/Window.hpp"

namespace fm
{
    class Game
    {
    public:
        Game(const char* name, unsigned int w, unsigned h)
            : m_window(NULL), m_quit(false), m_running(false)
        {
            Time::init();

            window.Init({100.f, 100.f}, {360u, 360u}, "Hello");

            m_window = SDL_CreateWindow(
                name,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                w,
                h,
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            );

            if(m_window == NULL)
            {
                FM_SDL_LOG_ERROR;
                exit(EXIT_FAILURE);
            }

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
        }
    private:
         void update()
        {
            FM_PROFILE_FUNC;
            m_scene_stack.top()->update(); 
        }

        void render()
        {
            m_scene_stack.top()->render(m_window);
        } 


        void pollEvents()
        {
            FM_PROFILE_FUNC;

            window.PollEvent();

            fm::Keyboard::reset();
            fm::Mouse::getSelf().reset();

            SDL_Event e; 
            while( SDL_PollEvent( &e ) )
            { 
                fm::Keyboard::handleEvent(e);
                fm::Mouse::getSelf().handleEvent(e);

                if( e.type == SDL_QUIT ) 
                    quit(); 


            } 
            
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
            window.Shutdown();
            SDL_DestroyWindow( m_window );
            SDL_Quit();
        }

    private:
        SDL_Window* m_window;
        Window window;

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

