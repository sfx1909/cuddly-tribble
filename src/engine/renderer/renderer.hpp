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

namespace engine{
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct MeshData {
    GLuint vao;
    size_t vertexCount;
};


class Renderer {
    public: 
        Renderer();
        ~Renderer();
        std::vector<Vertex> LoadModel(const std::string& path);
        MeshData setupBuffers(const std::vector<Vertex>& vertices);
        void Render(const std::string& modelKey);
        Renderer(const Renderer&) = delete;
    private:
        std::unordered_map<std::string, MeshData> meshVAOs;
        GLuint shaderProgram;
        GLuint CompileShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        void Initialize();

        

};
}

