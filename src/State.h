#ifndef STATE_HPP
#define STATE_HPP

#include "Action.h"

class StateStrategy;

class State {
public:
    virtual Action execute(StateStrategy* strategy) = 0;
};

#endif
