#pragma once

#include "FM/System/Game.hpp"
#include "FM/System/EntitySystem.hpp"

namespace fm
{
    class Entity;
    class Game;
    class Scene
    {
    public:
        Scene()
        {

        }

        virtual ~Scene()
        {

        }

        virtual void startup()
        {
        }

        virtual void update()
        {

        }

        virtual void render(SDL_Window* window)
        {
        }

        virtual void destory()
        {
        }

        EntitySystem& getSystem()
        {
            return m_system;
        }


    private:
        EntitySystem m_system;

    };
}

