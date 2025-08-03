#include <unordered_map>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <sstream>
#include <algorithm>

namespace input
{
    struct KeyInfo
    {
        int key;  // GLFW_KEY_*
        int mods; // Modifier bits
        bool operator==(const KeyInfo &other) const
        {
            return key == other.key && mods == other.mods;
        }
    };
}

namespace std
{
    template <>
    struct hash<input::KeyInfo>
    {
        std::size_t operator()(const input::KeyInfo &k) const
        {
            return std::hash<int>()(k.key) ^ std::hash<int>()(k.mods);
        }
    };
}

namespace input
{
    // Helper to map string to GLFW key code
    int stringToGLFWKey(const std::string &name)
    {
        if (name.size() == 1)
        {
            char lower = std::tolower(name[0]);

            if (lower >= 'a' && lower <= 'z')
                return GLFW_KEY_A + (lower - 'a');
            if (lower >= '0' && lower <= '9')
                return GLFW_KEY_0 + (lower - '0');
        }
        if (name == "space")
            return GLFW_KEY_SPACE;
        if (name == "arrow_up" || name == "up_arrow")
            return GLFW_KEY_UP;
        if (name == "arrow_down" || name == "down_arrow")
            return GLFW_KEY_DOWN;
        if (name == "arrow_left" || name == "left_arrow")
            return GLFW_KEY_LEFT;
        if (name == "arrow_right" || name == "right_arrow")
            return GLFW_KEY_RIGHT;
        // TODO: Add more key mappings as needed
        return -1;
    }

    KeyInfo parseKeyString(const std::string &keyStr)
    {
        int mods = 0;
        std::string keyName;
        std::vector<std::string> parts;
        std::stringstream ss(keyStr);
        std::string item;
        while (std::getline(ss, item, '+'))
        {
            // trim spaces
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);
            parts.push_back(item);
        }
        for (const auto &part : parts)
        {
            if (part == "shift")
                mods |= GLFW_MOD_SHIFT;
            else if (part == "ctrl")
                mods |= GLFW_MOD_CONTROL;
            else if (part == "alt")
                mods |= GLFW_MOD_ALT;
            else
                keyName = part;
        }

        int key = stringToGLFWKey(keyName);
        return {key, mods};
    }

    std::vector<std::string> splitKeys(const std::string &keybind)
    {
        std::vector<std::string> keys;
        std::stringstream ss(keybind);
        std::string item;
        while (std::getline(ss, item, '|'))
        {
            // Remove whitespace
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            // Strip leading and trailing single or double quotes
            if (!item.empty() && (item.front() == '\'' || item.front() == '"'))
                item.erase(0, 1);
            if (!item.empty() && (item.back() == '\'' || item.back() == '"'))
                item.pop_back();
            keys.push_back(item);
        }
        return keys;
    }
} // namespace input
