#include "ActionPathAutomata.hpp"

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #                DEFINING FROM EACH STATE TRANSITIONS                         #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

// t_state = Transitions starting from "state" and going somewhere.

State t_start(const char& c, ActionPathAutomata& ref) {
    if (isAlphaNumeric(c)) { 
        return ref.addChar(c) ? State::A : State::INVALID;
    }
    if (c == AUTOMATA_ACTION_SEPARATOR) {
        return State::C; 
    }
    if (c == '\0') {
        return State::ANONYMOUS;
    }
    return State::INVALID;
};



State t_invalid(const char& c, ActionPathAutomata& ref) {
    return State::INVALID;
};



State t_a(const char& c, ActionPathAutomata& ref) {
    if (isAlphaNumeric(c)) { 
        return ref.addChar(c) ? State::A : State::INVALID; 
    }
    if (c == AUTOMATA_PATH_SEPARATOR) { 
        ref.transferBuffer();
        return State::B; 
    }
    if (c == AUTOMATA_ACTION_SEPARATOR) {
        ref.transferBuffer();
        return State::C; 
    }
    if (c == '\0') { 
        ref.transferBuffer();
        return State::ANONYMOUS; 
    }
    return State::INVALID;
};



State t_b(const char& c, ActionPathAutomata& ref) {
    if (isAlphaNumeric(c)) { 
        return ref.addChar(c) ? State::A : State::INVALID;
    }
    return State::INVALID;
};



State t_c(const char& c, ActionPathAutomata& ref) {
    if (isAlphaNumeric(c)) { 
        return ref.addChar(c) ? State::D : State::INVALID;
    }
    if (c == '\0') {
        return State::CAPED;
    }
    return State::INVALID;
};



State t_d(const char& c, ActionPathAutomata& ref) {
    if (isAlphaNumeric(c)) { 
        return ref.addChar(c) ? State::D : State::INVALID;
    }
    if (c == '\0') { 
        ref.getTarget() = ref.getBuffer();
        ref.resetBuffer();
        return State::NAMED; 
    }
    return State::INVALID;
};



State t_caped(const char& c, ActionPathAutomata& ref) {
    if (c == '\0') { return State::CAPED; }
    return State::INVALID;
};



State t_anonymous(const char& c, ActionPathAutomata& ref) {
    if (c == '\0') { return State::ANONYMOUS; }
    return State::INVALID;
};



State t_named(const char& c, ActionPathAutomata& ref) {
    if (c == '\0') { return State::NAMED; }
    return State::INVALID;
};



Transitions ActionPathAutomata::transitions = {
    t_start,
    t_invalid,
    t_a,
    t_b,
    t_c,
    t_d,
    t_caped,
    t_anonymous,
    t_named
};


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


ActionPathAutomata::ActionPathAutomata(const char* data) { this->parseStream(data); }

void ActionPathAutomata::parseStream(const char* data) {
    if (data == nullptr) { 
        this->state = State::INVALID;
        return;
    }

    this->path.clear();
    this->path.reserve(this->numberOfSeparators(data) + 1);

    size_t i = 0;
    
    do {
        this->state = ActionPathAutomata::transitions[static_cast<int>(this->state)](data[i], *this);
    } while ((this->state != State::INVALID) && (data[i++] != '\0'));
}


size_t ActionPathAutomata::numberOfSeparators(const char* data) {
    size_t i = 0, count = 0;
    char c;

    do {
        c = data[i++];
        if ((c == AUTOMATA_PATH_SEPARATOR) || (c == AUTOMATA_ACTION_SEPARATOR)) { count++; }
    } while (c);

    return count;
}



