#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "keybinds.hpp"
#include "keyboard.hpp"
#include <string>

namespace engine
{
    template <typename ActionType>
    class Input
    {
    public:
        Input(GLFWwindow *window)
        {
            keyboard = std::make_unique<input::Keyboard<ActionType>>(window);
        }

        void Update() {
            keyboard->checkHeldKeys();
        }

        void Bind(const std::string &keybind, const ActionType &action)
        {
            keyboard->bindKey(keybind, action);
        }

        void OnPressAction(typename input::Keyboard<ActionType>::KeybindCallback cb)
        {
            keyboard->onKeybindPress(cb);
        }
        void OnReleaseAction(typename input::Keyboard<ActionType>::KeybindCallback cb)
        {
            keyboard->onKeybindRelease(cb);
        }
        void OnHoldAction(typename input::Keyboard<ActionType>::KeybindCallback cb)
        {
            keyboard->onKeybindHold(cb);
        }

        bool LoadKeybindsFromFile(const std::string &filePath)
        {
            mINI::INIStructure keybinds;
            mINI::INIFile file(filePath);
            
            bool success = file.read(keybinds);

            if (!success)
            {
                return false;
            }

            auto &keyboardSection = keybinds["keyboard"];

            for (const auto &pair : keyboardSection)
            {
                const std::string &keybind = pair.second;
                const ActionType &action = pair.first;
                Bind(keybind, action);
            }

            return true;
        }

    private:
        std::unique_ptr<input::Keyboard<ActionType>> keyboard;
    };
}
