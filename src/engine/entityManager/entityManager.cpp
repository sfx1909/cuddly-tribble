#include "entityManager.hpp"

namespace engine
{

    // AddEntity: adds a new entity to the manager's list
    void EntityManager::AddEntity(Entity *entity)
    {
        if (entity) // Check if the entity pointer is valid
        {
            entities.push_back(entity); // Add the entity to the list
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