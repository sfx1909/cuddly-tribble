#pragma once
#include "engine/input/input.hpp"
#include "engine/entity/entity.hpp"

namespace gameObjects
{
    class player : public engine::Entity
    {
    private:
        const std::string playerModelPath = "./assets/models/vehicle-speedster.obj";
        engine::Input<std::string> *input; // Input handler for player actions

    public:
        ~player();

        player(std::string name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
        ~player();
        void Update(float deltaTime) override; // override base Update
        void start() override;                 // override base start
    };
}