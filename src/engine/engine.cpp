#include <GL/glew.h>
#include "engine.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
namespace engine
{
    Engine::Engine()
    {
        // Load configuration settings from a singleton Config instance
        Config::instance().load();
        auto *config = &Config::instance();
        this->config = config; // Store pointer to config for later use

        // Initialize GLFW library — necessary for creating windows and OpenGL contexts
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Set GLFW window hints to specify the OpenGL context version and profile:
        // Request OpenGL version 3.3 (major=3, minor=3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // Request a core profile (modern OpenGL without deprecated features)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        // On macOS, set forward compatibility to true to ensure compatibility with modern OpenGL
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Retrieve window width, height, and title from the config file
        int width = std::stoi(this->config->get()["window"]["width"]);
        int height = std::stoi(this->config->get()["window"]["height"]);

        auto title = GetConfigValue("window", "title");
        if (!title.empty() && title.front() == '"' && title.back() == '"')
        {
            title = title.substr(1, title.size() - 2);
        }

        // Create the GLFW window with specified dimensions and title
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            // If window creation fails, clean up GLFW and throw an error
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        input = new Input<std::string>(window);

        // This can be called again to reload keybinds if needed
        std::string keybindsFile = "./preferences/keybinds.ini";
        auto loadedKeybindsFile = input->LoadKeybindsFromFile(keybindsFile);
        if (!loadedKeybindsFile)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to load Keybinds file: " + keybindsFile);
        }

        // Make the OpenGL context of the window current on the calling thread
        glfwMakeContextCurrent(window);

        // Enable V-sync by setting swap interval to 1 (limits FPS to monitor refresh rate)
        glfwSwapInterval(1);

        // Initialize GLEW to load OpenGL extensions.
        // This must be done after making the OpenGL context current.
        glewExperimental = GL_TRUE; // Enable experimental features for modern OpenGL functionality
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            // If GLEW initialization fails, output the error and throw exception
            std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
            throw std::runtime_error("GLEW initialization failed");
        }

        // 🌟 SET FRAMEBUFFER SIZE FOR OPENGL VIEWPORT
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight); // Important!

        // Optional: auto-adjust viewport on resize
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height)
                                       { glViewport(0, 0, width, height); });

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Set the default clear color (background color) to black with full opacity
        glClearColor(0, 0, 0, 1);

        // Create an instance of Renderer, which handles drawing in the OpenGL context
        this->renderer = new Renderer();
    }

    void Engine::Run()
    {
        auto car = new engine::Entity("Car", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "./assets/models/vehicle-speedster.obj");
        auto entityManager = &engine::EntityManager::Instance();
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto deltaTime = UpdateDeltaTime(); // Get delta time for frame rate independent updates
            for (auto &entity : entityManager->GetEntities())
            {
                entity->Update(deltaTime);      // Update each entity with delta time
                this->renderer->Render(entity); // Render each entity
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    Engine::~Engine()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    };

    std::string Engine::GetConfigValue(const std::string section, const std::string key)
    {
        try
        {
            return this->config->get()[section][key];
        }
        catch (...)
        {
            throw std::runtime_error("Failed to get config value for section: " + section + ", key: " + key);
        }
    };

    float Engine::UpdateDeltaTime()
    {
        // Calculate delta time for frame rate independent updates
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        return deltaTime.count(); // Return the delta time for this frame
    };
}