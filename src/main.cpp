#include <iostream>
#include <GL/glew.h>
#include "engine/engine.hpp"

int main()
{
    //TODO: Needs to be fixed but not crucial
    //fix name not loading in
    engine::Engine engine = engine::Engine();
    engine.OnKeybindPress([](const std::string& action) {
        std::cout << "Keybind press triggered: " << action << std::endl;
    });
    engine.OnKeybindHold([](const std::string& action) {
        std::cout << "Keybind hold triggered: " << action << std::endl;
    });
    engine.OnKeybindRelease([](const std::string& action) {
        std::cout << "Keybind release triggered: " << action << std::endl;
    });
    engine.Run();
    return 0;
}
