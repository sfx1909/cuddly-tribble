#include "renderer.hpp"
#include <stdexcept>
#include <glm/ext/matrix_transform.hpp>  // For glm::lookAt, glm::translate, etc.
#include <glm/ext/matrix_clip_space.hpp> // For glm::perspective

namespace engine
{

    // Constructor: calls the Initialize method to set up rendering
    Renderer::Renderer()
    {
        Initialize(); // Set up shaders, load model, create buffers
    };

    // Initialize: sets up shaders, loads the model, creates buffers
    void Renderer::Initialize()
    {
        try
        {
            // 🔹 Vertex shader source — transforms 3D position using MVP matrix
            std::string vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec3 aNormal;
                layout (location = 2) in vec2 aTexCoords;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                }
            )";

            // 🔹 Fragment shader source — outputs solid white color
            std::string fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                void main() {
                    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White
                }
            )";

            // 🔹 Compile and link the shaders into a program
            shaderProgram = CompileShader(vertexShaderSrc, fragmentShaderSrc);

            // 🔹 Load model vertices using Assimp
            auto carVerts = LoadModel("./assets/models/vehicle-speedster.obj");

            // 🔹 Upload model data to GPU
            auto carMeshData = setupBuffers(carVerts);

            // 🔹 Store the mesh with a key for later use in Render()
            meshVAOs["vehicle-speedster"] = carMeshData;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Exception caught during Renderer initialization: " << ex.what() << std::endl;
        }
    }

    // Destructor: currently empty, could be used to delete GPU buffers
    Renderer::~Renderer() {}

    // Loads a model file using Assimp and converts it into a list of Vertex structs
    std::vector<Vertex> Renderer::LoadModel(const std::string &path)
    {
        Assimp::Importer importer;

        // Read file with Assimp, converting to triangles and flipping winding
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder);

        if (!scene || !scene->mRootNode)
        {
            std::cerr << "Error: " << importer.GetErrorString() << std::endl;
            return {};
        }

        std::vector<Vertex> vertices;

        // Loop over each mesh in the model
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[i];

            // Extract each vertex
            for (unsigned int j = 0; j < mesh->mNumVertices; j++)
            {
                Vertex vertex;
                vertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
                vertex.normal = glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
                if (mesh->mTextureCoords[0])
                {
                    vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
                }
                else
                {
                    vertex.texCoords = glm::vec2(0.0f, 0.0f); // Default to 0 if no UVs
                }
                vertices.push_back(vertex);
            }
        }

        return vertices;
    };

    // Compiles vertex and fragment shaders and links them into a shader program
    GLuint Renderer::CompileShader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        // Helper lambda for compiling a shader of a given type
        auto compile = [](GLenum type, const std::string &src) -> GLuint
        {
            GLuint shader = glCreateShader(type);
            const char *csrc = src.c_str();
            glShaderSource(shader, 1, &csrc, nullptr);
            glCompileShader(shader);

            // Check compilation result
            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
            }
            return shader;
        };

        // Compile both shaders
        GLuint vertexShader = compile(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSrc);

        // Link shaders into a program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Check linking result
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            throw std::runtime_error("Shader linking failed: " + std::string(infoLog));
        }

        // We can now delete the individual shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    };

    // Creates a VAO and VBO, and uploads the vertex data to GPU memory
    MeshData Renderer::setupBuffers(const std::vector<Vertex> &vertices)
    {
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Enable and describe position attribute (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);

        // Enable and describe normal attribute (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Enable and describe texture coordinate attribute (location 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        return {VAO, vertices.size()}; // Return mesh info (VAO and vertex count)
    };

    // Renders the model identified by modelKey using the shader and transform matrices
    void Renderer::Render(const std::string &modelKey)
    {
        auto it = meshVAOs.find(modelKey);
        if (it == meshVAOs.end())
        {
            std::cerr << "Render error: VAO for model '" << modelKey << "' not found." << std::endl;
            return;
        }

        // Use the shader program
        glUseProgram(shaderProgram);

        // Create transformation matrices
        glm::mat4 model = glm::mat4(1.0f); // Identity model matrix (no transform)
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f), // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f), // Target
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up direction
        );
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Send matrices to shader via uniforms
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // Bind VAO and draw the model
        const MeshData &mesh = it->second;
        glBindVertexArray(mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertexCount));
        glBindVertexArray(0); // Unbind VAO
    }

}
