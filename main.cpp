#include <SFML/Graphics.hpp>
#include "SFMLrun.h"
#include <iostream>
#include "Graph.h"
using namespace std;

int main()
{
    
    loadPorts();
    loadOceanWaypoints();

    const string routeFile = "routes.txt";
    const string mapFile = "world_map.png";

    if (!loadRoutes(routeFile)) return 1;

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile(mapFile)) {
        cerr << "Failed to load map: " << mapFile << endl;
        return 1;
    }

    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u mapSize = mapTexture.getSize();

    // Initialize port pixel positions
    for (int i = 0; i < portCount; ++i)
        ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);

    // Initialize waypoint pixel positions
    for (int i = 0; i < WAYPOINT_COUNT; ++i)
        oceanWaypoints[i].pixel = latlonToPixel(oceanWaypoints[i].lat, oceanWaypoints[i].lon, mapSize);

    sf::VideoMode desktop = getDesktopMode();
    unsigned int windowWidth = desktop.width * 0.9;
    unsigned int windowHeight = desktop.height * 0.9;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Shipping Routes Map");
    window.setFramerateLimit(60);

    float scaleX = (float)windowWidth / mapSize.x;
    float scaleY = (float)windowHeight / mapSize.y;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    mapSprite.setScale(scale, scale);

    // Scale all positions
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
    if (!font.loadFromFile("Arial.ttf")) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            cerr << "Warning: Could not load font" << endl;
        }
    }

    sf::CircleShape portShape(PORT_RADIUS);
    portShape.setOrigin(PORT_RADIUS, PORT_RADIUS);
    portShape.setFillColor(sf::Color::White);
    portShape.setOutlineThickness(1.f);
    portShape.setOutlineColor(sf::Color(120, 120, 120));

    sf::CircleShape portHighlight(HIGHLIGHT_RADIUS);
    portHighlight.setOrigin(HIGHLIGHT_RADIUS, HIGHLIGHT_RADIUS);
    portHighlight.setOutlineThickness(2.f);
    portHighlight.setOutlineColor(sf::Color::Yellow);
    portHighlight.setFillColor(sf::Color::Transparent);

    bool dragging = false;
    sf::Vector2i lastMouse;
    int hoveredPort = -1, hoveredRoute = -1, selectedRoute = -1;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::MouseWheelScrolled) {
                zoom *= (e.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                if (zoom < 0.1f) zoom = 0.1f;
                if (zoom > 10.f) zoom = 10.f;
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
                sf::Vector2f delta = window.mapPixelToCoords(lastMouse) - window.mapPixelToCoords(now);
                view.move(delta);
                lastMouse = now;
            }

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::R) {
                    view = window.getDefaultView();
                    zoom = 1.f;
                }
                if (e.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        sf::Vector2i mousePix = sf::Mouse::getPosition(window);
        sf::Vector2f world = window.mapPixelToCoords(mousePix, view);
        hoveredPort = -1;
        hoveredRoute = -1;

        for (int i = 0; i < portCount; ++i) {
            float dx = ports[i].pixel.x - world.x;
            float dy = ports[i].pixel.y - world.y;
            if (sqrt(dx * dx + dy * dy) <= 10.f) {
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
            float ab2 = ab.x * ab.x + ab.y * ab.y;
            if (ab2 == 0) continue;
            float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            sf::Vector2f proj = a + ab * t;
            float dist = sqrt((proj.x - world.x) * (proj.x - world.x) +
                (proj.y - world.y) * (proj.y - world.y));
            if (dist <= 8.f) {
                hoveredRoute = i;
                break;
            }
        }

        window.clear(sf::Color(20, 40, 70));
        window.setView(view);
        window.draw(mapSprite);

        // Draw routes with intelligent ocean paths
        for (int i = 0; i < routeCount; ++i) {
            if (!routes[i].valid) continue;

            sf::Vector2f a = ports[routes[i].originIndex].pixel;
            sf::Vector2f b = ports[routes[i].destinationIndex].pixel;

            bool isSel = (i == selectedRoute);
            bool isHover = (i == hoveredRoute);
            bool fromHover = (hoveredPort == routes[i].originIndex);

            sf::Color lineColor;
            if (isSel)
                lineColor = sf::Color::Cyan;
            else if (isHover)
                lineColor = sf::Color::Yellow;
            else if (fromHover)
                lineColor = sf::Color(255, 140, 0);
            else
                lineColor = sf::Color(150, 180, 200, 100);

            // Generate intelligent ocean path
            sf::Vector2f pathPoints[MAX_WAYPOINTS];
            int pathLength = 0;
            generateOceanPath(a, b, routes[i].originIndex, routes[i].destinationIndex,
                pathPoints, pathLength);

            // Draw the route following ocean paths
            drawOceanRoute(window, pathPoints, pathLength, lineColor);
        }

        // Draw ports
        for (int i = 0; i < portCount; ++i) {
            portShape.setPosition(ports[i].pixel);
            window.draw(portShape);
            if (i == hoveredPort) {
                portHighlight.setPosition(ports[i].pixel);
                window.draw(portHighlight);
            }
        }

        window.setView(window.getDefaultView());

        if (hoveredPort != -1) {
            drawPortDropdown(window, font, hoveredPort, mousePix);
        }
        else if (hoveredRoute != -1) {
            Routes& r = routes[hoveredRoute];

            sf::RectangleShape tooltip(sf::Vector2f(400.f, 60.f));
            tooltip.setPosition(mousePix.x + 15, mousePix.y + 15);
            tooltip.setFillColor(sf::Color(40, 40, 40, 240));
            tooltip.setOutlineThickness(2.f);
            tooltip.setOutlineColor(sf::Color::Yellow);
            window.draw(tooltip);

            sf::Text info1("", font, 13);
            info1.setString(ports[r.originIndex].name + " -> " + ports[r.destinationIndex].name);
            info1.setFillColor(sf::Color::White);
            info1.setStyle(sf::Text::Bold);
            info1.setPosition(mousePix.x + 25, mousePix.y + 20);
            window.draw(info1);

            sf::Text info2("", font, 11);
            info2.setString(r.date + " | " + r.depTime + " - " + r.arrTime);
            info2.setFillColor(sf::Color(200, 200, 200));
            info2.setPosition(mousePix.x + 25, mousePix.y + 40);
            window.draw(info2);

            sf::Text info3("", font, 11);
            info3.setString("Cost: $" + intToString(r.vCost) + " | " + r.company);
            info3.setFillColor(sf::Color(150, 220, 255));
            info3.setPosition(mousePix.x + 25, mousePix.y + 55);
            window.draw(info3);
        }

        sf::Text instructions("", font, 12);
        instructions.setString("Controls: Mouse Wheel = Zoom | Drag = Pan | R = Reset | ESC = Exit | Hover ports for details");
        instructions.setFillColor(sf::Color(200, 200, 200));
        instructions.setPosition(10, windowHeight - 25);
        window.draw(instructions);

        window.display();
    }

    
    Graph g;
    g.loadFile("Routes.txt");
    g.printGraph();

    string source, destination, date;
    cout << "Enter the source port (first letter should be capital): ";
    getline(cin >> ws, source);
    cout << "Enter the destination port (first letter should be capital): ";
    getline(cin >> ws, destination);
    cout << "Enter the departure date (DD/MM/YYYY): ";
    getline(cin >> ws, date);
    g.searchRoutes(source, destination, date);
}


