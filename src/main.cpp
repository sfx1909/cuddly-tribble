#include <iostream>
#include <fstream>
#include <string>
#include "config.h"

int main()
{
    Config::instance().load();
    auto assetRoot = Config::instance().get()["assets"]["root"];
    std::cout << "Asset Root: " << assetRoot << std::endl;
    return 0;
}
