#include <utils/config/config.hpp>
#include <string>
#include <engine/renderer/renderer.hpp>

namespace engine{
class Engine {
    private:
        GLFWwindow* window;
        Config* config;
        Renderer* renderer;
    public:
        Engine();
        ~Engine();
        void Run();
        std::string GetConfigValue(const std::string section, const std::string key);

};
}
