#include "entityManager.hpp"
#include "entity.hpp" // Add this include to ensure Entity is fully defined
#include <input.hpp>
#include <type_traits>
#include <input_mixin.hpp>

namespace engine
{
    // AddEntity: adds a new entity to the manager's list
    void EntityManager::AddEntity(Entity *entity)
    {
        if (entity) // Check if the entity pointer is valid
        {
            if (auto inputEntity = dynamic_cast<engine::WithInputMixin *>(entity))
            {
                inputEntity->setInputHandler(inputHandler);
            }
            entities.push_back(entity);
        }
    }

    bool EntityManager::DeleteEntity(Entity *entity)
    {
        if (!entity)
            return false; // Handle null pointer safely

        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            if (*it == entity)
            {
                delete *it;         // free the memory
                entities.erase(it); // remove pointer from vector
                return true;        // found and deleted
            }
        }
        return false; // not found
    }

    std::vector<Entity *> EntityManager::GetEntities()
    {
        return entities;
    }
}