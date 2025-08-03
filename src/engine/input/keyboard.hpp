#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_set>
#include <functional>
#include "keybinds.hpp"

namespace input
{
    template <typename ActionType>
    class Keyboard
    {
    public:
        using KeybindCallback = std::function<void(const ActionType &)>;
        Keyboard(GLFWwindow *window);
        void bindKey(const std::string &keybind, const ActionType &action);
        ActionType getAction(const std::string &key) const;
        void onKeybindPress(KeybindCallback cb);
        void onKeybindRelease(KeybindCallback cb);
        void onKeybindHold(KeybindCallback cb);
        void checkHeldKeys();
        bool isActionActive(const ActionType &action);
        static void keyCallbackDispatch(GLFWwindow *window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow *window;
        Keybind<ActionType> keybinds;
        KeybindCallback onPressCallback = nullptr;
        KeybindCallback onReleaseCallback = nullptr;
        KeybindCallback onHoldCallback = nullptr;
        static Keyboard<ActionType> *instance;
        std::unordered_set<KeyInfo> heldKeys;
    };
    template <typename ActionType>
    Keyboard<ActionType> *Keyboard<ActionType>::instance = nullptr;

    // Template implementation
    template <typename ActionType>
    Keyboard<ActionType>::Keyboard(GLFWwindow *window)
        : window(window)
    {
        glfwSetKeyCallback(window, Keyboard<ActionType>::keyCallbackDispatch);
        instance = this;
    }
    template <typename ActionType>
    void Keyboard<ActionType>::bindKey(const std::string &keybind, const ActionType &action)
    {
        keybinds.BindKey(keybind, action);
    }
    template <typename ActionType>
    ActionType Keyboard<ActionType>::getAction(const std::string &key) const
    {
        KeyInfo keyInfo = parseKeyString(key);
        return keybinds.GetAction(keyInfo);
    }
    template <typename ActionType>
    void Keyboard<ActionType>::onKeybindPress(KeybindCallback cb)
    {
        onPressCallback = cb;
    }
    template <typename ActionType>
    void Keyboard<ActionType>::onKeybindRelease(KeybindCallback cb)
    {
        onReleaseCallback = cb;
    }
    template <typename ActionType>
    void Keyboard<ActionType>::onKeybindHold(KeybindCallback cb)
    {
        onHoldCallback = cb;
    }
    template <typename ActionType>
    void Keyboard<ActionType>::keyCallbackDispatch(GLFWwindow *window, int key, int scancode, int action, int mods)
    {

        if (!instance)
            return;

        KeyInfo keyInfo{key, mods};
        ActionType act = instance->keybinds.GetAction(keyInfo);

        if (act == ActionType{})
            return;

        if (action == GLFW_PRESS && instance->onPressCallback)
        {
            instance->heldKeys.insert(keyInfo);
            instance->onPressCallback(act);
        }
        if (action == GLFW_RELEASE && instance->onReleaseCallback)
        {
            instance->heldKeys.erase(keyInfo);
            instance->onReleaseCallback(act);
        }
    }
    template <typename ActionType>
    void Keyboard<ActionType>::checkHeldKeys()
    {
        if (!instance)
            return;

        for (const auto &keyInfo : instance->heldKeys)
        {
            ActionType act = instance->keybinds.GetAction(keyInfo);
            if (act != ActionType{} && instance->onHoldCallback)
            {
                instance->onHoldCallback(act);
            }
        }
    }

    template <typename ActionType>
    bool Keyboard<ActionType>::isActionActive(const ActionType &action)
    {
        if (!instance)
            return false;

        if (instance->heldKeys.empty())
            return false;

        for (const auto &keyInfo : instance->heldKeys)
        {
            if (instance->keybinds.GetAction(keyInfo) == action)
                return true;
        }
        return false;
    }
}
