#ifndef ACTION_KEY_HPP_INCLUDED
#define ACTION_KEY_HPP_INCLUDED

#include "StackSegment.hpp"

#define SIZE_OF_ACTION_KEY 32

class action_key: public StackSegment<SIZE_OF_ACTION_KEY> {
public:
    action_key(): StackSegment<SIZE_OF_ACTION_KEY>(0){}
	action_key(const char* c): StackSegment<SIZE_OF_ACTION_KEY>(0){
        this->override(c, strlen(c));
    }
};

inline bool operator<(const action_key& k1, const action_key& k2) {
    return strcmp(k1.c_str(), k2.c_str()) < 0;
}

#endif // ACTION_KEY_HPP_INCLUDED