#pragma once
#include "utils/parser/mini/ini.h"

class Config
{
public:
    static Config& instance();
    mINI::INIStructure get();
    bool load();

private:
    Config();
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    mINI::INIStructure ini;
};