#pragma once

#include "FM/System/Game.hpp"
#include "FM/System/EntitySystem.hpp"
#include "FM/Platfrom/Window/Window.hpp"


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

        virtual void render(Window& window)
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

