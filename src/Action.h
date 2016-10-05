#ifndef ACTION_H
#define ACTION_H

#include <iostream>
class Vacuum;

/**
 * All possible types for Action.type
 */
enum ActionType {
    GoNorth, // timer = 1, energy = 1
    GoSouth, // idem
    GoEast,  // idem
    GoWest,  // idem
    Suck,    // timer = t, energy = t, remove = 15 * t
    Gather,  // timer = 3, energy = 1.4, remove = 1
    Iddle,    // sur base : timer = t, energy = 2 * t
    None
};

struct Action {
    Action() {}
    Action(ActionType type, double timer=0): type(type), timer(timer) {}
    ActionType type = None;
    double timer = 0;
    friend std::ostream& operator<<(std::ostream& output, const Action& action);
};

#endif
