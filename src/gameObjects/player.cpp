#include "player.hpp"

namespace gameObjects
{

    Player::Player(std::string name, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
        : Entity(name, pos, rotation, scale, this->playerModelPath)
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
        if (isActionActive("move_forward"))
        {
            glm::vec3 newPos = GetPosition();
            newPos.z += deltaTime; // Move forward in the z direction
            SetPosition(newPos);
        }
        if (isActionActive("move_backward"))
        {
            glm::vec3 newPos = GetPosition();
            newPos.z -= deltaTime; // Move backward in the z direction
            SetPosition(newPos);
        }
    }
} // namespace gameObjects
