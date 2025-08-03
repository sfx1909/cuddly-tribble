#include "engine.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
namespace engine
{
    Engine::Engine()
    {
        Config::instance().load();
        auto *config = &Config::instance();
        this->config = config;
        if (!glfwInit())
        {

            throw std::runtime_error("Failed to initialize GLFW");
        }
        int width = std::stoi(this->config->get()["window"]["width"]);
        int height = std::stoi(this->config->get()["window"]["height"]);
        const char *title = this->config->get()["window"]["title"].c_str();
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window)
        {
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

        glfwMakeContextCurrent(window);
        glClearColor(0, 0, 0, 1);
    };

    Engine::~Engine()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        delete this->config;
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

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            // Animate ball
            ballX += ballVX;
            ballY += ballVY;
            if (ballX < ballRadius || ballX > width - ballRadius)
                ballVX = -ballVX;
            if (ballY < ballRadius || ballY > height - ballRadius)
                ballVY = -ballVY;
            // Draw ball
            glColor3f(1, 1, 1);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(ballX / (width / 2) - 1, ballY / (height / 2) - 1);
            for (int i = 0; i <= 100; ++i)
            {
                float angle = i * 2.0f * M_PI / 100;
                float x = ballX + cos(angle) * ballRadius;
                float y = ballY + sin(angle) * ballRadius;
                glVertex2f(x / (width / 2) - 1, y / (height / 2) - 1);
            }
            glEnd();
            glfwSwapBuffers(window);
            glfwPollEvents();
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