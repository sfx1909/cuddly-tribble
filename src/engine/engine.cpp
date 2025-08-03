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
        this->config = config;  // Store pointer to config for later use

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
        const char *title = this->config->get()["window"]["title"].c_str();
    
    // Create the GLFW window with specified dimensions and title
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
    if (err != GLEW_OK) {
        // If GLEW initialization fails, output the error and throw exception
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        throw std::runtime_error("GLEW initialization failed");
    }

    // 🌟 SET FRAMEBUFFER SIZE FOR OPENGL VIEWPORT
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight); // Important!

    // Optional: auto-adjust viewport on resize
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // Enable depth test
    glEnable(GL_DEPTH_TEST);


    // Set the default clear color (background color) to black with full opacity
    glClearColor(0, 0, 0, 1);
    
    // Create an instance of Renderer, which handles drawing in the OpenGL context
    this->renderer = new Renderer();
}


    Engine::~Engine()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        };

    void Engine::OnKeybindPress(std::function<void(const std::string &)> callback)
    {
        input->OnPressAction([callback](const std::string &action)
                             {
            if (!action.empty())
            {
                callback(action);
            } });
    }

    void Engine::OnKeybindHold(std::function<void(const std::string &)> callback)
    {
        input->OnHoldAction([callback](const std::string &action)
                            {
            if (!action.empty())
            {
                callback(action);
            } });
    }

    void Engine::OnKeybindRelease(std::function<void(const std::string &)> callback)
    {
        input->OnReleaseAction([callback](const std::string &action)
                               {
            if (!action.empty())
            {
                callback(action);
            } });
    }

    void Engine::Run()
    {
        float ballX = 400, ballY = 300, ballVX = 3, ballVY = 2, ballRadius = 40;
        int width = 800, height = 600;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->renderer->Render("vehicle-speedster"); // Render the scene

        glfwSwapBuffers(window);
        glfwPollEvents();
        // glClear(GL_COLOR_BUFFER_BIT);
        // // Animate ball
        // ballX += ballVX;
        // ballY += ballVY;
        // if (ballX < ballRadius || ballX > width - ballRadius) ballVX = -ballVX;
        // if (ballY < ballRadius || ballY > height - ballRadius) ballVY = -ballVY;
        // // Draw ball
        // glColor3f(1, 1, 1);
        // glBegin(GL_TRIANGLE_FAN);
        // glVertex2f(ballX / (width / 2) - 1, ballY / (height / 2) - 1);
        // for (int i = 0; i <= 100; ++i) {
        //     float angle = i * 2.0f * M_PI / 100;
        //     float x = ballX + cos(angle) * ballRadius;
        //     float y = ballY + sin(angle) * ballRadius;
        //     glVertex2f(x / (width / 2) - 1, y / (height / 2) - 1);
        // }
        // glEnd();
        // glfwSwapBuffers(window);
        // glfwPollEvents();
    }

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
}