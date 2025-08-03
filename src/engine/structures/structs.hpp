#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
namespace engine
{
    struct MeshData
    {
        GLuint vao;
        size_t vertexCount;
    };

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
}