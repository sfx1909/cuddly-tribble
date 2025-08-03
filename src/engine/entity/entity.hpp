#pragma once
#include <string>
#include <vector>
#include "engine/structures/structs.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/renderer/renderer.hpp"

namespace engine
{
    class Entity
    {
    private:
        std::string name;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        engine::MeshData meshData;

    public:
        Entity(std::string name, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), const std::string &modelPath = "./assets/models/vehicle-speedster.obj");
        virtual ~Entity() noexcept;

        // move this into cpp for better encapsulation
        glm::vec3 GetPosition() const
        {
            return position;
        }

        void SetPosition(const glm::vec3 &pos)
        {
            position = pos;
        }
        const std::string &GetName() const
        {
            return name;
        }

        void SetName(const std::string &name)
        {
            this->name = name;
        }

        void SetRotation(const glm::vec3 &rot)
        {
            rotation = rot;
        }

        void SetScale(const glm::vec3 &scale)
        {
            this->scale = scale;
        }

        virtual void Update(float deltaTime);
        virtual void Start();

        void setMeshData(const engine::MeshData &data);
        const engine::MeshData *GetMeshData() const;
    };
}
