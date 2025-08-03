#pragma once
#include "engine/input/input.hpp"

namespace engine {
    template <typename Derived, typename ActionType>
    class WithInputMixin {
    protected:
        engine::Input<ActionType>* inputHandler = nullptr;
    public:
        void setInputHandler(engine::Input<ActionType>* handler) {
            inputHandler = handler;
        }
        engine::Input<ActionType>* input() const {
            return inputHandler;
        }
        bool isActionActive(const ActionType& action) const {
            return inputHandler ? inputHandler->IsActionActive(action) : false;
        }
    };
}
