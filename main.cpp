#include <SFML/Graphics.hpp>
#include <iostream>
#include "MenuButton.h"
#include "SFMLrun.h"
#include "Graph.h"
using namespace std;

enum Page { MENU, MAP, BOOKING, DIJKSTRACOST, DIJKSTRATIME };

int main() {

    Graph g;
    g.loadFile("Routes.txt");
    Page currentPage = MENU;

    const string routeFile = "routes.txt";
    const string mapFile   = "world_map.png";
    const string menuBG    = "menuBG.png";

    sf::VideoMode desktop = getDesktopMode();
    unsigned int windowWidth  = desktop.width * 0.9;
    unsigned int windowHeight = desktop.height * 0.9;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "OceanRoute Nav");
    window.setFramerateLimit(60);

    sf::Texture menuTexture;
    menuTexture.loadFromFile(menuBG);

    sf::Sprite menuSprite(menuTexture);
    menuSprite.setScale(
        float(windowWidth) / menuTexture.getSize().x,
        float(windowHeight) / menuTexture.getSize().y
    );

    sf::Font font;
    font.loadFromFile("Arial.ttf");

    float buttonWidth  = 300;
    float buttonHeight = 80;

    float centerX = (windowWidth - buttonWidth) / 2.f;
    float centerY = windowHeight * 0.45f;

    Button mapBtn(centerX, centerY, buttonWidth, buttonHeight, "MAP", font);
    Button bookBtn(centerX, centerY + 120, buttonWidth, buttonHeight, "SHIP BOOKING", font);
    Button dijkstraCostBtn(centerX, centerY + 240, buttonWidth, buttonHeight, "DIJKSTRA MIN COST", font);
    Button dijkstraTimeBtn(centerX, centerY + 360, buttonWidth, buttonHeight, "DIJKSTRA MIN TIME", font);


    // ===================== MAIN LOOP ============================
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ===================== MENU PAGE =========================
        if (currentPage == MENU) 
        {
            window.clear();
            window.draw(menuSprite);
            mapBtn.draw(window);
            bookBtn.draw(window);
            dijkstraCostBtn.draw(window);
            dijkstraTimeBtn.draw(window);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (mapBtn.isMouseOver(mousePos)) currentPage = MAP;
                else if (bookBtn.isMouseOver(mousePos)) currentPage = BOOKING;
                else if (dijkstraCostBtn.isMouseOver(mousePos)) currentPage = DIJKSTRACOST;
                else if (dijkstraTimeBtn.isMouseOver(mousePos)) currentPage = DIJKSTRATIME;
            }

            window.display();
            continue;
        }

        // ===================== MAP PAGE ==========================
        if (currentPage == MAP) 
        {
            runMap(window, g);
            currentPage = MENU;
            continue;
        }

        // ===================== BOOKING PAGE ======================
        if (currentPage == BOOKING) 
        {
            window.clear();
            shipBooking(window, g);
            currentPage = MENU;
            continue;
             
            /*
            g.printGraph();

            string source, destination, date;
            cout << "Enter the source port (first letter should be capital): ";
            getline(cin >> ws, source);
            cout << "Enter the destination port (first letter should be capital): ";
            getline(cin >> ws, destination);
            cout << "Enter the departure date (DD/MM/YYYY): ";
            getline(cin >> ws, date);
            g.searchRoutes(source, destination, date);

            currentPage = MENU;
            continue;

            */
        }

        // ===================== DIJKSTRA PAGE =====================
        if (currentPage == DIJKSTRACOST)
        {
            window.close();
            g.printGraph();

            string source, destination;
            cout << "Enter the source port (first letter should be capital): ";
            getline(cin >> ws, source);
            cout << "Enter the destination port (first letter should be capital): ";
            getline(cin >> ws, destination);
            g.dijkstraMinCost(source, destination);

            currentPage = MENU;
            continue;
        }

        if (currentPage == DIJKSTRATIME)
        {
            window.close();
            g.printGraph();

            string source, destination;
            cout << "Enter the source port (first letter should be capital): ";
            getline(cin >> ws, source);
            cout << "Enter the destination port (first letter should be capital): ";
            getline(cin >> ws, destination);
            g.dijkstraMinTime(source, destination);

            currentPage = MENU;
            continue;
        }
    }

    return 0;
}



