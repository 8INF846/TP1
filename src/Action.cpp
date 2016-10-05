#include "Action.h"

std::ostream& operator<<(std::ostream& output, const Action& action) {
    output << "Action(";
    switch(action.type) {
    case GoNorth:
        output << "GoNorth";
        break;
    case GoSouth:
        output << "GoSouth";
        break;
    case GoEast:
        output << "GoEast";
        break;
    case GoWest:
        output << "GoWest";
        break;
    case Suck:
        output << "Suck, " << action.timer;
        break;
    case Gather:
        output << "Gather";
        break;
    case Iddle:
        output << "Iddle, " << action.timer;
        break;
    case None:
        output << "None";
        break;
    }
    output << ")";
    return output;
}

