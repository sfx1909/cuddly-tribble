#pragma once
#include "engine/input/input.hpp"

namespace engine
{
    class WithInputMixin
    {
    protected:
        engine::Input<std::string> *inputHandler = nullptr;

    public:
        void setInputHandler(engine::Input<std::string> *handler)
        {
            inputHandler = handler;
        }

        bool isActionActive(const std::string &action) const
        {
            return inputHandler ? inputHandler->IsActionActive(action) : false;
        }
    };
}
