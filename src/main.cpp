#include <iostream>
#include "Environnement.h"

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cerr << "Incorrect number of parameters!" << std::endl;
        return EXIT_FAILURE;
    }
    Environnement(std::string(argv[1]));
    return EXIT_SUCCESS;
}
