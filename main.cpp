#include <SFML/Graphics.hpp>
#include <iostream>
#include "MenuButton.h"
#include "SFMLrun.h"
#include "Graph.h"
using namespace std;

enum Page { MENU, MAP, BOOKING, DIJKSTRA };

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
    Button dijkstraBtn(centerX, centerY + 240, buttonWidth, buttonHeight, "DIJKSTRA", font);

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
            dijkstraBtn.draw(window);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (mapBtn.isMouseOver(mousePos)) currentPage = MAP;
                else if (bookBtn.isMouseOver(mousePos)) currentPage = BOOKING;
                else if (dijkstraBtn.isMouseOver(mousePos)) currentPage = DIJKSTRA;
            }

            window.display();
            continue;
        }

        // ===================== MAP PAGE ==========================
        if (currentPage == MAP) 
        {
            loadPorts();
            loadOceanWaypoints();
            if (!loadRoutes(routeFile)) return 1;

            sf::Texture mapTexture;
            if (!mapTexture.loadFromFile(mapFile)) {
                cerr << "Failed to load map: " << mapFile << endl;
                return 1;
            }

            sf::Sprite mapSprite(mapTexture);
            sf::Vector2u mapSize = mapTexture.getSize();

            for (int i = 0; i < portCount; ++i)
                ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);

            for (int i = 0; i < WAYPOINT_COUNT; ++i)
                oceanWaypoints[i].pixel = latlonToPixel(oceanWaypoints[i].lat, oceanWaypoints[i].lon, mapSize);

            float scaleX = float(windowWidth)  / mapSize.x;
            float scaleY = float(windowHeight) / mapSize.y;
            float scale  = (scaleX < scaleY) ? scaleX : scaleY;

            mapSprite.setScale(scale, scale);

            for (int i = 0; i < portCount; ++i) {
                ports[i].pixel.x *= scale;
                ports[i].pixel.y *= scale;
            }

            for (int i = 0; i < WAYPOINT_COUNT; ++i) {
                oceanWaypoints[i].pixel.x *= scale;
                oceanWaypoints[i].pixel.y *= scale;
            }

            sf::View view = window.getDefaultView();
            float zoom = 1.f;

            sf::Font font;
            font.loadFromFile("Arial.ttf");

            sf::CircleShape portShape(PORT_RADIUS);
            portShape.setOrigin(PORT_RADIUS, PORT_RADIUS);
            portShape.setFillColor(sf::Color::White);
            portShape.setOutlineThickness(1);
            portShape.setOutlineColor(sf::Color(120,120,120));

            sf::CircleShape portHighlight(HIGHLIGHT_RADIUS);
            portHighlight.setOrigin(HIGHLIGHT_RADIUS, HIGHLIGHT_RADIUS);
            portHighlight.setOutlineThickness(2);
            portHighlight.setOutlineColor(sf::Color::Yellow);
            portHighlight.setFillColor(sf::Color::Transparent);

            bool dragging = false;
            sf::Vector2i lastMouse;
            int hoveredPort = -1, hoveredRoute = -1, selectedRoute = -1;

            // ----------- MAP INNER LOOP -------------
            while (window.isOpen() && currentPage == MAP)
            {
                sf::Event e;
                while (window.pollEvent(e)) 
                {
                    if (e.type == sf::Event::Closed) window.close();

                    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                        currentPage = MENU;

                    if (e.type == sf::Event::MouseWheelScrolled) {
                        zoom *= (e.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                        zoom = std::max(0.1f, std::min(zoom, 10.f));
                        view.setSize(window.getDefaultView().getSize().x * zoom,
                                     window.getDefaultView().getSize().y * zoom);
                    }

                    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                        dragging = true;
                        lastMouse = sf::Mouse::getPosition(window);
                    }

                    if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
                        dragging = false;
                        if (hoveredRoute != -1) {
                            if (selectedRoute == hoveredRoute) selectedRoute = -1;
                            else selectedRoute = hoveredRoute;
                        }
                    }

                    if (e.type == sf::Event::MouseMoved && dragging) {
                        sf::Vector2i now = sf::Mouse::getPosition(window);
                        sf::Vector2f delta = 
                            window.mapPixelToCoords(lastMouse) -
                            window.mapPixelToCoords(now);
                        view.move(delta);
                        lastMouse = now;
                    }
                }

                sf::Vector2i mousePix = sf::Mouse::getPosition(window);
                sf::Vector2f world = window.mapPixelToCoords(mousePix, view);

                hoveredPort = -1;
                hoveredRoute = -1;

                // -----------------  HOVERING -----------------
                for (int i = 0; i < portCount; ++i) {
                    float dx = ports[i].pixel.x - world.x;
                    float dy = ports[i].pixel.y - world.y;
                    if (sqrt(dx*dx + dy*dy) <= 10.f) {
                        hoveredPort = i;
                        break;
                    }
                }

                for (int i = 0; i < routeCount; ++i) {
                    if (!routes[i].valid) continue;
                    sf::Vector2f a = ports[routes[i].originIndex].pixel;
                    sf::Vector2f b = ports[routes[i].destinationIndex].pixel;
                    sf::Vector2f ap = world - a;
                    sf::Vector2f ab = b - a;
                    float ab2 = ab.x*ab.x + ab.y*ab.y;
                    if (ab2 == 0) continue;
                    float t = (ap.x*ab.x + ap.y*ab.y) / ab2;
                    t = std::max(0.f, std::min(t, 1.f));
                    sf::Vector2f proj = a + ab*t;
                    float dist = sqrt((proj.x-world.x)*(proj.x-world.x) +
                                      (proj.y-world.y)*(proj.y-world.y));
                    if (dist <= 8.f) {
                        hoveredRoute = i;
                        break;
                    }
                }

                // ------------------ DRAW ---------------------------------
                window.clear(sf::Color(20,40,70));
                window.setView(view);
                window.draw(mapSprite);

                // Routes
                for (int i = 0; i < routeCount; ++i) {
                    if (!routes[i].valid) continue;

                    sf::Vector2f a = ports[routes[i].originIndex].pixel;
                    sf::Vector2f b = ports[routes[i].destinationIndex].pixel;

                    bool isSel = (i == selectedRoute);
                    bool isHover = (i == hoveredRoute);
                    bool fromHover = (hoveredPort == routes[i].originIndex);

                    sf::Color lineColor =
                        isSel ? sf::Color::Cyan :
                        isHover ? sf::Color::Yellow :
                        fromHover ? sf::Color(255,140,0) :
                        sf::Color(150,180,200,100);

                    sf::Vector2f pathPoints[MAX_WAYPOINTS];
                    int pathLength = 0;
                    generateOceanPath(a, b, routes[i].originIndex, routes[i].destinationIndex,
                                      pathPoints, pathLength);

                    drawOceanRoute(window, pathPoints, pathLength, lineColor);
                }

                // Ports
                for (int i = 0; i < portCount; ++i) {
                    portShape.setPosition(ports[i].pixel);
                    window.draw(portShape);
                    if (i == hoveredPort) {
                        portHighlight.setPosition(ports[i].pixel);
                        window.draw(portHighlight);
                    }
                }

                window.setView(window.getDefaultView());

                if (hoveredPort != -1) 
                    drawPortDropdown(window, font, hoveredPort, mousePix);
                else if (hoveredRoute != -1) {
                    Routes &r = routes[hoveredRoute];

                    sf::RectangleShape tooltip(sf::Vector2f(400, 60));
                    tooltip.setPosition(mousePix.x+15, mousePix.y+15);
                    tooltip.setFillColor(sf::Color(40,40,40,240));
                    tooltip.setOutlineThickness(2);
                    tooltip.setOutlineColor(sf::Color::Yellow);
                    window.draw(tooltip);

                    sf::Text info1(ports[r.originIndex].name + " -> " + ports[r.destinationIndex].name,
                                   font, 13);
                    info1.setFillColor(sf::Color::White);
                    info1.setStyle(sf::Text::Bold);
                    info1.setPosition(mousePix.x + 25, mousePix.y + 20);
                    window.draw(info1);

                    sf::Text info2(r.date + " | " + r.depTime + " - " + r.arrTime,
                                   font, 11);
                    info2.setFillColor(sf::Color(200,200,200));
                    info2.setPosition(mousePix.x + 25, mousePix.y + 40);
                    window.draw(info2);

                    sf::Text info3("Cost: $" + intToString(r.vCost) + " | " + r.company,
                                   font, 11);
                    info3.setFillColor(sf::Color(150,220,255));
                    info3.setPosition(mousePix.x + 25, mousePix.y + 55);
                    window.draw(info3);
                }

                sf::Text instructions("Controls: Mouse Wheel = Zoom | Drag = Pan | R = Reset | ESC = Exit | Hover ports for details",
                                      font, 12);
                instructions.setFillColor(sf::Color(200,200,200));
                instructions.setPosition(10, windowHeight - 25);
                window.draw(instructions);

                window.display();
            }

            continue;
        }

        // ===================== BOOKING PAGE ======================
        if (currentPage == BOOKING) 
        {
            window.close();
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
        }

        // ===================== DIJKSTRA PAGE =====================
        if (currentPage == DIJKSTRA)
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
    }

    return 0;
}



