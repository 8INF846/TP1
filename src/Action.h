#ifndef ACTION_H
#define ACTION_H

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
    Iddle    // sur base : timer = t, energy = 2 * t
};

struct Action {
    ActionType type = Iddle;
    double timer = 0;
};

#endif
