#pragma once
#include <vector>
#include <string>

namespace engine
{
    // forward declaration of Entity class
    class Entity;

    class EntityManager
    {
    private:
        std::vector<Entity *> entities; // List of all entities managed by this manager
    public:
        static EntityManager &Instance()
        {
            static EntityManager instance;
            return instance;
        }
        void AddEntity(Entity *entity);
        bool DeleteEntity(Entity *entity);
        std::vector<Entity *> GetEntities();
    };
};