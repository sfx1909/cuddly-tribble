#include "config.h"
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

bool Config::load()
{
    mINI::INIFile file("../config.ini");
    bool success = file.read(ini);

    if (!success)
    {
        std::cerr << "Failed to open config file: " << "../config.ini" << std::endl;
        return false;
    }
    return true;
}