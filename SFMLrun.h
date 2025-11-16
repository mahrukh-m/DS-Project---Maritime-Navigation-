#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include <cmath>
#include <cstdlib>

const int MAX_PORTS = 40;
const int MAX_ROUTES = 2000;
const float PORT_RADIUS = 5.f;
const float HIGHLIGHT_RADIUS = 7.f;

struct Port {
    string name;
    double lat;
    double lon;
    sf::Vector2f pixel;
};

struct Routes {
    int originIndex;
    int destinationIndex;
    string date;
    string depTime;
    string arrTime;
    int vCost;
    string company;
    bool valid;
};

// ----------------- Globals -----------------
Port ports[MAX_PORTS];
int portCount = 0;
Routes routes[MAX_ROUTES];
int routeCount = 0;

// ----------------- Helpers -----------------
int findPortIndexByName(const string& name) {
    for (int i = 0; i < portCount; ++i)
        if (ports[i].name == name) return i;
    return -1;
}

sf::Vector2f latlonToPixel(double lat, double lon, const sf::Vector2u& imgSize) {
    double x = (lon + 180.0) / 360.0 * imgSize.x;
    double y = (90.0 - lat) / 180.0 * imgSize.y;
    return sf::Vector2f((float)x, (float)y);
}

void loadPorts() {
    // Hardcoded port list with coordinates
    string names[MAX_PORTS] = {
        "AbuDhabi","Alexandria","Antwerp","Athens","Busan","CapeTown","Chittagong","Colombo",
        "Copenhagen","Doha","Dubai","Dublin","Durban","Genoa","Hamburg","Helsinki","HongKong",
        "Istanbul","Jakarta","Jeddah","Karachi","Lisbon","London","LosAngeles","Manila",
        "Marseille","Melbourne","Montreal","Mumbai","NewYork","Osaka","Oslo","PortLouis",
        "Rotterdam","Shanghai","Singapore","Stockholm","Sydney","Tokyo","Vancouver"
    };

    double lats[MAX_PORTS] = {
        24.4539,31.2001,51.2194,37.9838,35.1796,-33.9249,22.3569,6.9271,
        55.6761,25.2854,25.2048,53.3498,-29.8587,44.4056,53.5511,60.1699,22.3193,
        41.0082,-6.2088,21.4858,24.8607,38.7223,51.5074,34.0522,14.5995,
        43.2965,-37.8136,45.5017,19.0760,40.7128,34.6937,59.9139,-20.1609,
        51.9244,31.2304,1.3521,59.3293,-33.8688,35.6762,49.2827
    };

    double lons[MAX_PORTS] = {
        54.3773,29.9187,4.4025,23.7275,129.0756,18.4241,91.7832,79.8612,
        12.5683,51.5310,55.2708,-6.2603,31.0218,8.9463,9.9937,24.9384,114.1694,
        28.9784,106.8456,39.1925,67.0011,-9.1393,-0.1278,-118.2437,120.9842,
        5.3698,144.9631,-73.5673,72.8777,-74.0060,135.5023,10.7522,57.5012,
        4.4777,121.4737,103.8198,18.0686,151.2093,139.6503,-123.1207
    };

    for (int i = 0; i < MAX_PORTS; ++i) {
        ports[i].name = names[i];
        ports[i].lat = lats[i];
        ports[i].lon = lons[i];
        portCount++;
    }

    cout << "Loaded " << portCount << " ports (hardcoded)" << endl;
}

bool loadRoutes(const string& fname) {
    ifstream fin(fname.c_str());
    if (!fin.is_open()) {
        cerr << "Cannot open routes file: " << fname << endl;
        return false;
    }

    string origin, destination, date, depTime, arrTime, company;
    int vCost;
    while (fin >> origin >> destination >> date >> depTime >> arrTime >> vCost >> company) {
        if (routeCount >= MAX_ROUTES) break;
        int oi = findPortIndexByName(origin);
        int di = findPortIndexByName(destination);
        if (oi == -1 || di == -1) {
            cerr << "Warning: Port not found for route: " << origin << " -> " << destination << endl;
            continue;
        }
        routes[routeCount].originIndex = oi;
        routes[routeCount].destinationIndex = di;
        routes[routeCount].date = date;
        routes[routeCount].depTime = depTime;
        routes[routeCount].arrTime = arrTime;
        routes[routeCount].vCost = vCost;
        routes[routeCount].company = company;
        routes[routeCount].valid = true;
        routeCount++;
    }
    fin.close();
    cout << "Loaded " << routeCount << " routes from " << fname << endl;
    return true;
}




class SFMLrun {
private:
    sf::RenderWindow window;
    Graph graph; 

public:
    SFMLrun(): window(sf::VideoMode(800, 600), "Maritime Navigation") 
    {
        graph.loadFile("Routes.txt");
    }
    void run()
    {
         loadPorts();

    const string routeFile = "routes.txt";
    const string mapFile = "world_map.png";

    if (!loadRoutes(routeFile)) return ;

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile(mapFile)) {
        cerr << "Failed to load map: " << mapFile << endl;
        return ;
    }

    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u mapSize = mapTexture.getSize();

    for (int i = 0; i < portCount; ++i)
        ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);

    sf::RenderWindow window(sf::VideoMode(1200, 700), "Shipping Routes Map");
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();
    float zoom = 1.f;

    sf::Font font;
    font.loadFromFile("Arial.ttf");

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
                view.setSize(window.getDefaultView().getSize().x * zoom, window.getDefaultView().getSize().y * zoom);
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
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R) {
                view = window.getDefaultView();
                zoom = 1.f;
            }
        }

        // Hover detection
        sf::Vector2i mousePix = sf::Mouse::getPosition(window);
        sf::Vector2f world = window.mapPixelToCoords(mousePix, view);
        hoveredPort = -1; hoveredRoute = -1;

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
            if (t < 0) t = 0; if (t > 1) t = 1;
            sf::Vector2f proj = a + ab * t;
            float dist = sqrt((proj.x - world.x) * (proj.x - world.x) + (proj.y - world.y) * (proj.y - world.y));
            if (dist <= 6.f) { hoveredRoute = i; break; }
        }

        window.clear(sf::Color::Black);
        window.setView(view);
        window.draw(mapSprite);

        // Draw routes
        for (int i = 0; i < routeCount; ++i) {
            if (!routes[i].valid) continue;
            sf::Vector2f a = ports[routes[i].originIndex].pixel;
            sf::Vector2f b = ports[routes[i].destinationIndex].pixel;
            sf::Vertex line[2];
            line[0].position = a;
            line[1].position = b;

            bool isSel = (i == selectedRoute);
            bool isHover = (i == hoveredRoute);
            bool fromHover = (hoveredPort == routes[i].originIndex);

            if (isSel)
                line[0].color = line[1].color = sf::Color::Cyan;
            else if (isHover)
                line[0].color = line[1].color = sf::Color::Yellow;
            else if (fromHover)
                line[0].color = line[1].color = sf::Color(255, 140, 0);
            else
                line[0].color = line[1].color = sf::Color(200, 200, 200, 90);

            window.draw(line, 2, sf::Lines);
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
        sf::Text info("", font, 14);
        info.setFillColor(sf::Color::White);

        if (hoveredRoute != -1) {
            Routes& r = routes[hoveredRoute];
            string txt = ports[r.originIndex].name + " -> " + ports[r.destinationIndex].name +
                " | " + r.date + " " + r.depTime + "-" + r.arrTime +
                " | $" + to_string(r.vCost) + " | " + r.company;
            info.setString(txt);
            info.setPosition(mousePix.x + 10, mousePix.y + 10);
            window.draw(info);
        }
        else if (hoveredPort != -1) {
            string txt = ports[hoveredPort].name;
            info.setString(txt);
            info.setPosition(mousePix.x + 10, mousePix.y + 10);
            window.draw(info);
        }

        window.display();
    }

    }
};
