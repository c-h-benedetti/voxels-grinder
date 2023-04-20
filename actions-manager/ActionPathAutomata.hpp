#ifndef ACTION_PATH_AUTOMATA_HPP_INCLUDED
#define ACTION_PATH_AUTOMATA_HPP_INCLUDED

#include <cstddef>
#include <functional>
#include <array>
#include <vector>
#include "action_key.hpp"
#include "ActionPathCategory.hpp"


#define ACTION_AUTOMATA_STATES 9
#define AUTOMATA_PATH_SEPARATOR '.'
#define AUTOMATA_ACTION_SEPARATOR ':'

class ActionPathAutomata;


using State = ActionPathCategory;
using Transition = std::function<State(const char& c, ActionPathAutomata&)>;
using Transitions = std::array<Transition, ACTION_AUTOMATA_STATES>;


class ActionPathAutomata {

    State state = State::START;
    static Transitions transitions;

    std::vector<action_key> path;
    action_key target;
    action_key buffer;
    size_t index = 0;

private:

    void parseStream(const char* data);
    size_t numberOfSeparators(const char* data);

public:

    inline std::vector<action_key>& getPathVector() { return this->path; }
    inline action_key& getTarget() { return this->target; }
    inline size_t& getCurrentIndex() { return this->index; }
    inline action_key& getBuffer() { return this->buffer; }

    inline void resetBuffer() { this->buffer.nullify(); this->index = 0;}
    inline void transferBuffer() { this->path.push_back(this->buffer); this->resetBuffer(); }
    inline bool addChar(const char& c) { 
        if (this->index >= SIZE_OF_ACTION_KEY) { return false; }
        this->buffer[this->index++] = c; 
        return true;
    }

    inline const std::vector<action_key>& getPath() const { return this->path; }
    inline const action_key& getActionId() const { return this->target; }
    inline State getState() const { return this->state; }

    ActionPathAutomata() = default;
    ActionPathAutomata(const char* data);

    friend State t_start(const char& c, ActionPathAutomata& ref);
    friend State t_invalid(const char& c, ActionPathAutomata& ref);
    friend State t_a(const char& c, ActionPathAutomata& ref);
    friend State t_b(const char& c, ActionPathAutomata& ref);
    friend State t_c(const char& c, ActionPathAutomata& ref);
    friend State t_d(const char& c, ActionPathAutomata& ref);
    friend State t_caped(const char& c, ActionPathAutomata& ref);
    friend State t_anonymous(const char& c, ActionPathAutomata& ref);
    friend State t_named(const char& c, ActionPathAutomata& ref);
};


inline bool isAlphaNumeric(const char& c) {
    if ((c >= 'a') && (c <= 'z')) { return true; }
    if ((c >= 'A') && (c <= 'Z')) { return true; }
    if ((c >= '0') && (c <= '9')) { return true; }
    if ((c == '_') || (c == '-')) { return true; }
    return false;
}


inline bool isValidKey(const char* data) {
    if (data == nullptr) { return false; }
    size_t i = 0;
    while(data[i] != '\0') {
        if (!isAlphaNumeric(data[i])) { return false; }
        i++;
    }
    return (i < SIZE_OF_ACTION_KEY) && (i > 0);
}


#endif // ACTION_PATH_AUTOMATA_HPP_INCLUDED