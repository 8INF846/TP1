#ifndef ACTION_H
#define ACTION_H

/**
 * All possible types for Action.type
 */
enum ActionType {
    GoNorth,
    GoSouth,
    GoEast,
    GoWest,
    Suck,
    Gather,
    Iddle
};

struct Action {
    ActionType type = Iddle;
    double timer = 0;
};

#endif
