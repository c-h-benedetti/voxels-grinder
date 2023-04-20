#ifndef ACTIONS_MANAGER_HPP_INCLUDED
#define ACTIONS_MANAGER_HPP_INCLUDED

#include <functional>
#include <initializer_list>
#include "ActionPathCategory.hpp"
#include "classifier.hpp"
#include "Action.hpp"


class ActionPath : public classifier::Path<action_key> {

    ActionPathCategory category = ActionPathCategory::ANONYMOUS;

private:

    inline bool hasObject() const override { return this->getObject().size() > 0; }

public:

    inline bool hasAction() const { return this->category == ActionPathCategory::NAMED; }
    bool isValid() const;

    ActionPath() = default;
    ActionPath(std::vector<action_key>&& init);
    ActionPath(std::vector<action_key>&& init, const action_key& obj);

    friend ActionPath actionPathBuilder(const char* stream);
    friend ActionPath actionPathBuilder(std::vector<action_key>&& init, const action_key& obj);
};

// These functions make sure that the generated objects are named.
ActionPath actionPathBuilder(const char* stream);
ActionPath actionPathBuilder(std::vector<action_key>&& init, const action_key& obj);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


using actions_node = classifier::Node<action_key, std::function<action_ptr()>>;
using actions_tree = classifier::Tree<action_key, std::function<action_ptr()>>;

class ActionsManager {

    static std::unique_ptr<actions_tree> tree;

public:

    static actions_tree& getActionsTree() { return *(ActionsManager::tree.get()); }

    static action_ptr instanciateAction(const ActionPath& p) {
        std::function<action_ptr()>* fx = ActionsManager::getActionsTree().getObject(p);
        return (fx == nullptr) ? (nullptr) : ((*fx)());
    }
};


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


class DuplicateActionIDException: public std::exception {};
class IncorrectPathException: public std::exception {};



template <typename T>
class ActionRegister : public ActionsManager {

public:

    ActionRegister(const ActionPath& s) : ActionsManager() {
        ActionsManager::getActionsTree().addObject(s, [](){ return std::unique_ptr<T>(new T{}); });
    }
};


#define DECLARE_ACTION(TYPE) static ActionRegister<TYPE> reg
#define REGISTER_ACTION(TYPE) ActionRegister<TYPE> TYPE::reg(TYPE::action_id())


#endif // ACTIONS_MANAGER_HPP_INCLUDED

/*

Pour le moment on part avec ce système d'arbre réel, mais il n'est pas exclu qu'on switch
à un arbre dont tout le contenu est aligné en mémoire.

*/