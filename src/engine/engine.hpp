#include <GLFW/glfw3.h>
#include <utils/config/config.hpp>
#include <string>

namespace engine{
class Engine {
    private:
        GLFWwindow* window;
        Config* config;
    public:
        Engine();
        ~Engine();
        void Run();
        std::string GetConfigValue(const std::string section, const std::string key);

};
}
