#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

const int MAX_PORTS = 40;
const int MAX_ROUTES = 2000;
const int MAX_WAYPOINTS = 10;
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

// Waypoint structure for routing around continents
struct Waypoint {
    double lat;
    double lon;
    sf::Vector2f pixel;
};

// ----------------- Globals -----------------
Port ports[MAX_PORTS];
int portCount = 0;
Routes routes[MAX_ROUTES];
int routeCount = 0;

// Critical ocean waypoints to route ships around continents
const int WAYPOINT_COUNT = 15;
Waypoint oceanWaypoints[WAYPOINT_COUNT];

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

void loadOceanWaypoints() {
    // Strategic waypoints for ocean routing
    // These points help routes go around major landmasses

    // Suez Canal area
    oceanWaypoints[0].lat = 30.0; oceanWaypoints[0].lon = 32.0;
    // Cape of Good Hope
    oceanWaypoints[1].lat = -35.0; oceanWaypoints[1].lon = 20.0;
    // Gibraltar Strait
    oceanWaypoints[2].lat = 36.0; oceanWaypoints[2].lon = -5.0;
    // Panama Canal
    oceanWaypoints[3].lat = 9.0; oceanWaypoints[3].lon = -80.0;
    // Malacca Strait
    oceanWaypoints[4].lat = 2.0; oceanWaypoints[4].lon = 100.0;
    // Horn of Africa
    oceanWaypoints[5].lat = 12.0; oceanWaypoints[5].lon = 50.0;
    // Mid-Atlantic
    oceanWaypoints[6].lat = 0.0; oceanWaypoints[6].lon = -30.0;
    // Mid-Pacific
    oceanWaypoints[7].lat = 0.0; oceanWaypoints[7].lon = -150.0;
    // Mid-Indian Ocean
    oceanWaypoints[8].lat = -10.0; oceanWaypoints[8].lon = 80.0;
    // North Atlantic
    oceanWaypoints[9].lat = 50.0; oceanWaypoints[9].lon = -30.0;
    // North Pacific
    oceanWaypoints[10].lat = 40.0; oceanWaypoints[10].lon = -160.0;
    // South Pacific
    oceanWaypoints[11].lat = -20.0; oceanWaypoints[11].lon = 160.0;
    // Bering Strait area
    oceanWaypoints[12].lat = 60.0; oceanWaypoints[12].lon = -170.0;
    // Indonesian waters
    oceanWaypoints[13].lat = -5.0; oceanWaypoints[13].lon = 115.0;
    // Arabian Sea
    oceanWaypoints[14].lat = 15.0; oceanWaypoints[14].lon = 65.0;
}

void loadPorts() {
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

    cout << "Loaded " << portCount << " ports" << endl;
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
    cout << "Loaded " << routeCount << " routes" << endl;
    return true;
}

string intToString(int val) {
    if (val == 0) return "0";
    string result = "";
    bool negative = val < 0;
    if (negative) val = -val;
    while (val > 0) {
        result = char('0' + (val % 10)) + result;
        val /= 10;
    }
    if (negative) result = "-" + result;
    return result;
}

sf::VideoMode getDesktopMode() {
    return sf::VideoMode::getDesktopMode();
}

// Determine which major ocean body separates two ports
int determineOceanRoute(const Port& origin, const Port& dest) {
    // Returns region code for routing logic
    double oLat = origin.lat, oLon = origin.lon;
    double dLat = dest.lat, dLon = dest.lon;

    // Europe to Asia (via Suez or around Africa)
    if ((oLon < 30 && oLat > 30) && (dLon > 50 && dLat > 10)) return 1; // Via Suez

    // Europe to East Asia (via Suez)
    if ((oLon < 30 && oLat > 30) && (dLon > 100)) return 2;

    // Americas to Asia (via Pacific)
    if ((oLon < -60 && oLon > -130) && (dLon > 100)) return 3;

    // Africa to Asia (Indian Ocean)
    if ((oLon > 0 && oLon < 40 && oLat < 0) && (dLon > 60 && dLat < 30)) return 4;

    // Cross-Atlantic
    if ((oLon < -30 && oLon > -80) && (dLon > -20 && dLon < 20)) return 5;

    // Around Africa (Cape route)
    if ((oLon < 20 && oLat > 30) && (dLon > 30 && dLat < 0)) return 6;

    return 0; // Default/nearby
}

// Generate intelligent waypoints to avoid land
void generateOceanPath(sf::Vector2f start, sf::Vector2f end, int originIdx, int destIdx,
    sf::Vector2f* pathPoints, int& pathLength) {
    pathPoints[0] = start;
    pathLength = 1;

    // Get route type based on geography
    int routeType = determineOceanRoute(ports[originIdx], ports[destIdx]);

    // Add intermediate waypoints based on route type
    if (routeType == 1) { // Europe to Middle East via Suez
        pathPoints[pathLength++] = oceanWaypoints[2].pixel; // Gibraltar
        pathPoints[pathLength++] = oceanWaypoints[0].pixel; // Suez
    }
    else if (routeType == 2) { // Europe to East Asia via Suez
        pathPoints[pathLength++] = oceanWaypoints[2].pixel; // Gibraltar
        pathPoints[pathLength++] = oceanWaypoints[0].pixel; // Suez
        pathPoints[pathLength++] = oceanWaypoints[4].pixel; // Malacca
    }
    else if (routeType == 3) { // Americas to Asia via Pacific
        pathPoints[pathLength++] = oceanWaypoints[3].pixel; // Panama
        pathPoints[pathLength++] = oceanWaypoints[7].pixel; // Mid-Pacific
    }
    else if (routeType == 4) { // Africa to Asia via Indian Ocean
        pathPoints[pathLength++] = oceanWaypoints[1].pixel; // Cape
        pathPoints[pathLength++] = oceanWaypoints[8].pixel; // Mid-Indian
    }
    else if (routeType == 5) { // Cross-Atlantic
        pathPoints[pathLength++] = oceanWaypoints[6].pixel; // Mid-Atlantic
    }
    else if (routeType == 6) { // Around Africa
        pathPoints[pathLength++] = oceanWaypoints[1].pixel; // Cape of Good Hope
    }
    else {
        // For nearby routes, add a gentle arc
        sf::Vector2f mid = (start + end) / 2.f;
        sf::Vector2f diff = end - start;
        sf::Vector2f perp(-diff.y, diff.x);
        float len = sqrt(perp.x * perp.x + perp.y * perp.y);
        if (len > 0) {
            perp.x /= len;
            perp.y /= len;
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
            mid += perp * (dist * 0.1f);
            pathPoints[pathLength++] = mid;
        }
    }

    pathPoints[pathLength++] = end;
}

// Draw smooth curved path through waypoints
void drawOceanRoute(sf::RenderWindow& window, sf::Vector2f* points, int pointCount, sf::Color color) {
    if (pointCount < 2) return;

    // Draw curved segments between waypoints
    for (int i = 0; i < pointCount - 1; ++i) {
        sf::Vector2f start = points[i];
        sf::Vector2f end = points[i + 1];

        // Bezier curve control point
        sf::Vector2f mid = (start + end) / 2.f;
        sf::Vector2f diff = end - start;
        sf::Vector2f perp(-diff.y, diff.x);
        float len = sqrt(perp.x * perp.x + perp.y * perp.y);
        if (len > 0) perp = perp / len;

        float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
        sf::Vector2f control = mid + perp * (dist * 0.08f);

        // Draw curve
        int segments = 15;
        sf::Vertex line[2];

        for (int j = 0; j < segments; ++j) {
            float t1 = (float)j / segments;
            float t2 = (float)(j + 1) / segments;

            float u1 = 1 - t1;
            sf::Vector2f p1 = u1 * u1 * start + 2.f * u1 * t1 * control + t1 * t1 * end;

            float u2 = 1 - t2;
            sf::Vector2f p2 = u2 * u2 * start + 2.f * u2 * t2 * control + t2 * t2 * end;

            line[0].position = p1;
            line[1].position = p2;
            line[0].color = color;
            line[1].color = color;

            window.draw(line, 2, sf::Lines);
        }
    }
}

void drawPortDropdown(sf::RenderWindow& window, sf::Font& font, int portIndex, sf::Vector2i mousePos) {
    int outgoingCount = 0;
    for (int i = 0; i < routeCount; ++i) {
        if (routes[i].valid && routes[i].originIndex == portIndex) {
            outgoingCount++;
        }
    }

    float dropdownWidth = 300.f;
    float lineHeight = 20.f;
    float headerHeight = 25.f;
    int maxVisibleRoutes = 10;
    int visibleRoutes = (outgoingCount < maxVisibleRoutes) ? outgoingCount : maxVisibleRoutes;
    float dropdownHeight = headerHeight + (visibleRoutes + 1) * lineHeight + 10.f;

    sf::RectangleShape dropdown(sf::Vector2f(dropdownWidth, dropdownHeight));
    dropdown.setPosition(mousePos.x + 15, mousePos.y + 15);
    dropdown.setFillColor(sf::Color(40, 40, 40, 240));
    dropdown.setOutlineThickness(2.f);
    dropdown.setOutlineColor(sf::Color(255, 200, 0));
    window.draw(dropdown);

    sf::Text header("", font, 16);
    header.setString(ports[portIndex].name);
    header.setFillColor(sf::Color(255, 200, 0));
    header.setStyle(sf::Text::Bold);
    header.setPosition(mousePos.x + 25, mousePos.y + 20);
    window.draw(header);

    sf::Text coords("", font, 12);
    coords.setString("Lat: " + intToString((int)ports[portIndex].lat) +
        ", Lon: " + intToString((int)ports[portIndex].lon));
    coords.setFillColor(sf::Color(200, 200, 200));
    coords.setPosition(mousePos.x + 25, mousePos.y + 42);
    window.draw(coords);

    sf::Text routesHeader("", font, 13);
    routesHeader.setString("Outgoing Routes (" + intToString(outgoingCount) + "):");
    routesHeader.setFillColor(sf::Color(150, 220, 255));
    routesHeader.setStyle(sf::Text::Bold);
    routesHeader.setPosition(mousePos.x + 25, mousePos.y + 62);
    window.draw(routesHeader);

    int displayCount = 0;
    float yOffset = 82.f;

    for (int i = 0; i < routeCount && displayCount < maxVisibleRoutes; ++i) {
        if (routes[i].valid && routes[i].originIndex == portIndex) {
            sf::Text routeInfo("", font, 11);
            string routeText = "-> " + ports[routes[i].destinationIndex].name +
                " | " + routes[i].date + " | $" + intToString(routes[i].vCost);
            routeInfo.setString(routeText);
            routeInfo.setFillColor(sf::Color(220, 220, 220));
            routeInfo.setPosition(mousePos.x + 30, mousePos.y + yOffset);
            window.draw(routeInfo);

            yOffset += lineHeight;
            displayCount++;
        }
    }

    if (outgoingCount > maxVisibleRoutes) {
        sf::Text more("", font, 11);
        more.setString("... and " + intToString(outgoingCount - maxVisibleRoutes) + " more");
        more.setFillColor(sf::Color(150, 150, 150));
        more.setStyle(sf::Text::Italic);
        more.setPosition(mousePos.x + 30, mousePos.y + yOffset);
        window.draw(more);
    }
}