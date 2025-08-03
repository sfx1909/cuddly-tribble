#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utils/config/config.hpp>
#include <string>
#include <functional>
#include "engine/input/input.hpp"
#include "engine/renderer/renderer.hpp"
#include <chrono>
#include "engine/entity/entity.hpp"

namespace engine
{
    class Engine
    {
    private:
        GLFWwindow *window;
        Config *config;
        Input<std::string> *input;
        Renderer *renderer;

    public:
        Engine();
        ~Engine();
        void Run();
        float UpdateDeltaTime();
        std::string GetConfigValue(const std::string section, const std::string key);
    };
}
