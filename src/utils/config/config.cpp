#include "config.hpp"
#include <fstream>
#include <iostream>

Config::Config() {}

Config& Config::instance()
{
    static Config instance;
    return instance;
}

mINI::INIStructure Config::get()
{
    return ini;
}

static const auto configFilePath = "./config.ini";

bool Config::load()
{
    
    mINI::INIFile file(configFilePath);
    bool success = file.read(ini);

    if (!success)
    {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
        return false;
    }
    return true;
}