#include "ActionPathAutomata.hpp"
#include "ActionPathCategory.hpp"
#include "ActionsManager.hpp"
#include "action_key.hpp"


std::unique_ptr<actions_tree> ActionsManager::tree{ new actions_tree{} };


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


ActionPath::ActionPath(std::vector<action_key>&& init) : classifier::Path<action_key>(std::move(init)){
    this->category = ActionPathCategory::ANONYMOUS;
}

ActionPath::ActionPath(std::vector<action_key>&& init, const action_key& obj) : classifier::Path<action_key>(std::move(init), obj) {
    this->category = ActionPathCategory::NAMED;
}


bool ActionPath::isValid() const {
    for (const action_key& a : *this) {
        if (!isValidKey(a.c_str())) {
            return false;
        }
    }

    if ((this->category == State::NAMED) && !isValidKey(this->getObject().c_str())) {
        return false;
    }

    return true;
}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


ActionPath actionPathBuilder(const char* stream) {
    ActionPathAutomata a1(stream);

    if (a1.getState() != State::NAMED) {
        throw "InvalidActionPathException";
    }
    
    return ActionPath(std::move(a1.getPathVector()), a1.getTarget());
}


ActionPath actionPathBuilder(std::vector<action_key>&& init, const action_key& obj) {
    if (obj.length() == 0) {
        throw "InvalidActionPathException";
    }

    for (const action_key& a : init) {
        if (!isValidKey(a.c_str())) {
            throw "InvalidActionPathException";
        }
    }

    return ActionPath(std::move(init), obj);
}