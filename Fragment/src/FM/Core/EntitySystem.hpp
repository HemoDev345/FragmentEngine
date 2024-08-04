#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include "FM/Core/Game.hpp"

namespace fm 
{
    class Entity;

    class EntitySystem
    {
    public:
        EntitySystem()
        {
        }

        ~EntitySystem()
        {
        }

        std::unordered_map<int, void*>::iterator begin()
        {
            return m_entitys_dir.begin();
        }

        std::unordered_map<int, void*>::iterator end()
        {
            return m_entitys_dir.end();
        }

        template<typename T>
        std::vector<T>& get()
        {
            return *((std::vector<T>*)m_entitys_dir.at(typeid(T).hash_code()));
        }

        template<typename T>
        void append()
        {
            m_entitys_dir[typeid(T).hash_code()] = new std::vector<T>();
        }

        template<typename T>
        void push_back(T&& entity)
        {
            ((std::vector<T>*)m_entitys_dir.at(typeid(T).hash_code()))->push_back(entity);
        }

        template<typename T>
        void pop_back()
        {
            ((std::vector<T>*)m_entitys_dir.at(typeid(T).hash_code()))->pop_back();
        }

        template<typename T>
        void erase(std::vector<Entity*>::iterator _where)
        {
            ((std::vector<T>*)m_entitys_dir.at(typeid(T).hash_code()))->erase(_where);
        }

        void clear()
        {
            m_entitys_dir.clear();
        }

        template<typename T>
        void clear()
        {
            delete ((std::vector<T>*)m_entitys_dir.at(typeid(T).hash_code()));
            m_entitys_dir.erase(typeid(T).hash_code());
        }

    private:
        std::unordered_map<int, void*> m_entitys_dir;
        

    };
}
