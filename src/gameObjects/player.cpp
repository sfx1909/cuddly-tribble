#include "player.hpp"

namespace gameObjects
{

    Player::Player(std::string name, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
        : Entity(name, pos, rotation, scale, playerModelPath)
    {
    }

    Player::~Player()
    {
        // Destructor cleans up resources if needed
    }

    void Player::Update(float deltaTime)
    {
        // Update Player state based on input and delta time
        Movement(deltaTime);       // Handle movement based on input
        Entity::Update(deltaTime); // Call base class update
        // Additional Player-specific update logic can go here
    }

    void Player::Start()
    {
        // Initialize Player-specific properties or state
        Entity::Start(); // Call base class start method
    }

    void Player::Movement(float deltaTime)
    {
        // Example movement logic
        glm::vec3 newPos = GetPosition();
        if (isActionActive("move_forward"))
        {
            newPos.z += deltaTime; // Move forward in the z direction
        }
        if (isActionActive("move_backward"))
        {
            newPos.z -= deltaTime; // Move backward in the z direction
        }

        if (isActionActive("move_left"))
        {
            newPos.x += deltaTime; // Move left in the x direction
        }
        if (isActionActive("move_right"))
        {
            newPos.x -= deltaTime; // Move right in the x direction
        }
        SetPosition(newPos);
    }

}
