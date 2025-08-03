#pragma once
#include "engine/input/input.hpp"
#include "engine/entity/entity.hpp"
#include "engine/entity/input_mixin.hpp"

namespace gameObjects
{
    class Player : public engine::Entity, public engine::WithInputMixin
    {
    private:
        const std::string playerModelPath = "./assets/models/vehicle-speedster.obj";
        void Movement(float deltaTime); // Function to handle player movement
    public:
        ~Player();
        Player(std::string name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
        void Update(float deltaTime) override; // override base Update
        void Start() override;                 // override base start
    };
}