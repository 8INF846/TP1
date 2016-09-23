#ifndef ACTION_H
#define ACTION_H

enum ActionType {
    GoNorth,
    GoSouth,
    GoEst,
    GoWest,
    Suck,
    Gather,
    Iddle
};

struct Action {
    ActionType type = iddle;
    double timer = 0;
}

#endif
