#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

namespace input {
    struct KeyInfo {
        int key;
        int mods;
        bool operator==(const KeyInfo& other) const {
            return key == other.key && mods == other.mods;
        }
    };

    int stringToGLFWKey(const std::string& name);
    KeyInfo parseKeyString(const std::string& keyStr);
    std::vector<std::string> splitKeys(const std::string &keybind);

    template<typename KeybindAction>
    class Keybind {
    public:
        void BindKey(const std::string &keybind, const KeybindAction &action);
        KeybindAction GetAction(const KeyInfo &key) const;
    private:
        std::unordered_map<KeyInfo, KeybindAction> keyToAction;
    };
}

namespace std {
    template<>
    struct hash<input::KeyInfo> {
        std::size_t operator()(const input::KeyInfo& k) const {
            return std::hash<int>()(k.key) ^ std::hash<int>()(k.mods);
        }
    };
}

// Template implementation
namespace input {
    template<typename KeybindAction>
    void Keybind<KeybindAction>::BindKey(const std::string &keybind, const KeybindAction &action) {
        auto keys = splitKeys(keybind);
        for (const auto &keyStr : keys) {
            KeyInfo keyInfo = parseKeyString(keyStr);
            if (keyInfo.key != -1)
                keyToAction[keyInfo] = action;
        }
    }
    template<typename KeybindAction>
    KeybindAction Keybind<KeybindAction>::GetAction(const KeyInfo &key) const {
        auto it = keyToAction.find(key);
        if (it != keyToAction.end())
            return it->second;
        return KeybindAction {};
    }
}
