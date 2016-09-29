#include <iostream>
#include <thread>
#include <exception>

#include <SFML/Graphics.hpp>

#include "Loader.h"
#include "Startable.h"

void help() {
    std::cout << "Utilisation : vacuum [map_file_name] [vacuum_file_name]" << std::endl;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        help();
        return EXIT_FAILURE;
    }

    // Charger la map et la lancer

    MapReal map;
    try {
        map = Loader::loadMap(std::string(argv[1]));
    } catch(const std::string& e) {
        std::cerr << e << std::endl;
    }
    std::cout << map;
    std::thread threadMap = map.start();
    // Charger l'aspirateur, l'ajouter dans la map et le lancer
    Vacuum vacuum;
    try {
        vacuum = Loader::loadVacuum(std::string(argv[2]), map);
    } catch(const std::string& e) {
        std::cerr << e << std::endl;
    }
    std::thread threadVacuum = vacuum.start();

    //TODO SFML
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow app(
    sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel),
    "TP1 IA",
    sf::Style::Close | sf::Style::Titlebar );

    //TODO move to a thread
    while (app.isOpen())
    {
        sf::Event event;
        while (app.waitEvent(event))
        {
            if (event.type == sf::Event::Closed)
            app.close();
        }
        app.clear();
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(100, 50));
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setFillColor(sf::Color::Green);
        rectangle.setOutlineThickness(5);
        rectangle.setPosition(10, 20);
        app.draw(rectangle);
        app.display();
    }
    // Attendre la fin de l'exécution de la map et de l'aspirateur
    threadMap.join();
    threadVacuum.join();
    return EXIT_SUCCESS;
}
