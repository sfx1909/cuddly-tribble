#include "entity.hpp"

namespace engine
{
    Entity::Entity(std::string name, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, const std::string &modelPath)
    {
        // Constructor initializes default values
        this->SetName(name);
        this->SetPosition(pos);
        this->SetRotation(rotation);
        this->SetScale(scale);
        auto &renderer = Renderer::Instance();
        std::vector<Vertex> vertices = renderer.LoadModel(modelPath);
        this->setMeshData(renderer.setupBuffers(vertices)); // Set mesh data using renderer
        engine::EntityManager::Instance().AddEntity(this);  // Add this entity to the manager
        this->Start();                                      // Call start method to initialize any additional properties
    }

    Entity::~Entity()
    {
        // Destructor cleans up resources
        engine::EntityManager::Instance().DeleteEntity(this); // Remove this entity from the manager
    }

    void Entity::Start()
    {
    }

    void Entity::Update(float deltaTime)
    {
        // Update entity position, rotation, scale, etc. based on deltaTime
        // This is a placeholder for actual update logic
        position.x += deltaTime; // Example: move along x-axis
        // position.y += deltaTime; // Example: move along y-axis
        // position.z += deltaTime; // Example: move along z-axis
    }

    void Entity::setMeshData(const engine::MeshData &data)
    {
        meshData = data; // Set the mesh data for this entity
    }

    const engine::MeshData *Entity::GetMeshData() const
    {
        return &meshData;
    }
}