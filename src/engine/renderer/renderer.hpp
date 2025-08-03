
#pragma once
// Standard C++ Libraries
#include <iostream>
#include <vector>
#include <unordered_map>

// GLEW must be included before any OpenGL or GLFW headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM (Mathematical types for OpenGL)
#include <glm/glm.hpp>

// Assimp (Model loading)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <engine/structures/structs.hpp>

namespace engine
{

    class Entity;

    class Renderer
    {
    public:
        static Renderer &Instance()
        {
            static Renderer instance;
            return instance;
        }
        Renderer();
        ~Renderer();
        std::vector<Vertex> LoadModel(const std::string &path);
        engine::MeshData setupBuffers(const std::vector<Vertex> &vertices);
        void Render(engine::Entity *entity);
        Renderer(const Renderer &) = delete;

    private:
        GLuint shaderProgram;
        GLuint CompileShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        void Initialize();
    };
}
