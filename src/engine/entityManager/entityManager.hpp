#pragma once
#include <vector>
#include <string>
#include "engine/input/input.hpp"
#include "engine/entity/entity.hpp"
#include "engine/entity/input_mixin.hpp"

namespace engine
{
    // forward declaration of Entity class
    class Entity;

    class EntityManager
    {
    private:
        std::vector<Entity *> entities; // List of all entities managed by this manager
        engine::Input<std::string> *inputHandler;

    public:
        static EntityManager &Instance()
        {
            static EntityManager instance;
            return instance;
        }
        void AddEntity(Entity *entity);
        bool DeleteEntity(Entity *entity);
        std::vector<Entity *> GetEntities();
        void SetUpInputHandler(engine::Input<std::string> *input);
    };
};