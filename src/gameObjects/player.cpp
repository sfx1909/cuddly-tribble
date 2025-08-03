#include "player.hpp"

namespace gameObjects
{
    player::player(std::string name, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
        : Entity(name, pos, rotation, scale, playerModelPath)
    {
        // Constructor initializes the player entity with a model
    }

    player::~player()
    {
        // Destructor cleans up resources if needed
    }

    void player::Update(float deltaTime)
    {
        // Update player state based on input and delta time
        Entity::Update(deltaTime); // Call base class update
        // Additional player-specific update logic can go here
    }

    void player::start()
    {
        // Initialize player-specific properties or state
        Entity::start(); // Call base class start method
    }
} // namespace gameObjects
