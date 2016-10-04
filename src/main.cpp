#include <iostream>
#include <thread>
#include <exception>

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "Vacuum.h"
#include "Pos.h"
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

    //Load the map
    Map map;
    try {
        map = Loader::loadMap(std::string(argv[1]));
    }
    catch(const std::string& e) {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
    std::thread threadMap = map.start();

    //Load the vacuum
    Vacuum vacuum;
    try {
        vacuum = Loader::loadVacuum(std::string(argv[2]), map);
    }
    catch(const std::string& e) {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
    std::thread threadVacuum = vacuum.start();

    // Start rendering
    sf::RenderWindow window(
            sf::VideoMode(100 * map.width(), 100 * map.height()),
            "TP1 IA");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    // Floor
    sf::RectangleShape floor;
    floor.setSize(sf::Vector2f(100, 100));
    floor.setFillColor(sf::Color::White);
    // Vacuum
    sf::Texture vacuumTexture;
    vacuumTexture.loadFromFile("./vacuum.png");
    vacuumTexture.setSmooth(true);
    sf::Sprite vacuumSprite;
    vacuumSprite.setTexture(vacuumTexture);
    auto vBounds = vacuumSprite.getLocalBounds();
    vacuumSprite.setScale(float(100 / vBounds.width), float(100 / vBounds.height));
    // Vacuum's Base
    sf::Texture baseTexture;
    baseTexture.loadFromFile("./base.png");
    baseTexture.setSmooth(true);
    sf::Sprite baseSprite;
    baseSprite.setTexture(baseTexture);
    auto bBounds = baseSprite.getLocalBounds();
    baseSprite.setScale(float(100 / bBounds.width), float(100 / bBounds.height));
    // Jewelry
    sf::Texture jewelryTexture;
    jewelryTexture.loadFromFile("./jewelry.png");
    jewelryTexture.setSmooth(true);
    sf::Sprite jewelrySprite;
    jewelrySprite.setTexture(jewelryTexture);
    auto jBounds = jewelrySprite.getLocalBounds();
    jewelrySprite.setScale(float(100 / jBounds.width), float(100 / jBounds.height));

    while (window.isOpen()) {
        // Gestion des évènements
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear(sf::Color(0,0,0,255));

        // Affichage de la carte
        for(int i = 0; i < map.width(); i++) {
            for(int j = 0; j < map.height(); j++) {
                Pos pos(i, j);
                if(map.isFloor(pos)) {
                    float dirtLevel = map.dirtLevel(pos);
                    floor.setPosition(i * 100, j * 100);
                    floor.setFillColor(sf::Color(
                                255 - 10 * dirtLevel,
                                255 - 20 * dirtLevel,
                                255 - 200 * dirtLevel));
                    window.draw(floor);
                    if(map.jewelry(pos) > 0) {
                        jewelrySprite.setPosition(pos.x * 100, pos.y * 100);
                        window.draw(jewelrySprite);
                    }
                }
            }
        }

        // Affichage de la base de rechargement du robot aspirateur
        Pos pos = vacuum.basePosition();
        baseSprite.setPosition(pos.x * 100, pos.y * 100);
        window.draw(baseSprite);

        // Affichage du robot-aspirateur
        pos = vacuum.position();
        vacuumSprite.setPosition(pos.x * 100, pos.y * 100);
        window.draw(vacuumSprite);

        window.display();
    }

    map.stop();
    vacuum.stop();
    threadMap.join();
    threadVacuum.join();
    return EXIT_SUCCESS;
}
