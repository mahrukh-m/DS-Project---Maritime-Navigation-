#pragma once
#include "Graph.h"
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


// Visual representation of a route leg
struct VisualLeg {
    int routeIndex;        // index into routes[]
    bool layoverFeasible;  // layover AFTER this leg
};


// text box for user input
class TextBox {
public:
    sf::RectangleShape box;
    sf::Text text;
    bool active;

    TextBox() : active(false) {}

    TextBox(float x, float y, float w, float h,
        sf::Font& font,
        const std::string& placeholder = "")
    {
        active = false;
        box.setPosition(x, y);
        box.setSize({ w, h });
        box.setFillColor(sf::Color(15, 25, 40, 220));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(180, 180, 180));

        text.setFont(font);
        text.setString(placeholder);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color(200, 200, 200));
        text.setPosition(x + 8.f, y + 6.f);
    }

    bool isMouseOver(sf::Vector2f mouse) const {
        return box.getGlobalBounds().contains(mouse);
    }

    void setActive(bool a) {
        active = a;
        box.setOutlineColor(a ? sf::Color(0, 200, 255) : sf::Color(180, 180, 180));
    }

    std::string getString() const {
        return text.getString();
    }

    void clear() {
        text.setString("");
    }

    void handleEvent(const sf::Event& e) {
        if (!active) return;
        if (e.type == sf::Event::TextEntered) {
            if (e.text.unicode == 8) { // backspace
                std::string s = text.getString();
                if (!s.empty()) s.pop_back();
                text.setString(s);
            }
            else if (e.text.unicode >= 32 && e.text.unicode <= 126) {
                std::string s = text.getString();
                s.push_back(static_cast<char>(e.text.unicode));
                text.setString(s);
            }
        }
    }

    void draw(sf::RenderWindow& win) const {
        win.draw(box);
        win.draw(text);
    }
};


// ----------------- Globals -----------------
Port ports[MAX_PORTS];
int portCount = 0;
Routes routes[MAX_ROUTES];
int routeCount = 0;


const int MAX_VISUAL_LEGS = 10;
VisualLeg g_selectedItinerary[MAX_VISUAL_LEGS];
int g_visualLegCount = 0;

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
void drawOceanRoute(sf::RenderWindow& window, sf::Vector2f* pts, int cnt,
    sf::Color color, float thickness) {
    if (cnt < 2) return;

    int layers = (int)std::max(1.f, thickness);
    float step = 0.5f;
    for (int layer = 0; layer < layers; ++layer) {
        float off = (layer - layers / 2.f) * step;
        for (int i = 0; i < cnt - 1; ++i) {
            sf::Vector2f s = pts[i];
            sf::Vector2f e = pts[i + 1];

            sf::Vector2f mid = (s + e) / 2.f;
            sf::Vector2f diff = e - s;
            sf::Vector2f perp(-diff.y, diff.x);
            float l = std::sqrt(perp.x * perp.x + perp.y * perp.y);
            if (l > 0) perp /= l;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            sf::Vector2f c = mid + perp * (dist * 0.08f);

            int segs = 16;
            sf::Vertex line[2];
            for (int j = 0; j < segs; ++j) {
                float t1 = (float)j / segs;
                float t2 = (float)(j + 1) / segs;
                float u1 = 1 - t1;
                sf::Vector2f p1 = u1 * u1 * s + 2.f * u1 * t1 * c + t1 * t1 * e;
                float u2 = 1 - t2;
                sf::Vector2f p2 = u2 * u2 * s + 2.f * u2 * t2 * c + t2 * t2 * e;

                sf::Vector2f seg = p2 - p1;
                sf::Vector2f segPerp(-seg.y, seg.x);
                float sl = std::sqrt(segPerp.x * segPerp.x + segPerp.y * segPerp.y);
                if (sl > 0) {
                    segPerp /= sl;
                    p1 += segPerp * off;
                    p2 += segPerp * off;
                }

                line[0].position = p1;
                line[1].position = p2;
                line[0].color = color;
                line[1].color = color;
                window.draw(line, 2, sf::Lines);
            }
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


// time helpers
int timeToMinutesLocal(const string& time) {
    int hrs = 0, mins = 0;
    int colon = (int)time.find(':');
    if (colon == (int)string::npos) return -1;
    for (int i = 0; i < colon; ++i) hrs = hrs * 10 + (time[i] - '0');
    for (size_t i = colon + 1; i < time.size(); ++i) mins = mins * 10 + (time[i] - '0');
    return hrs * 60 + mins;
}

int dateToIntLocal(const string& date) {
    int d = 0, m = 0, y = 0, p = 0;
    while (p < (int)date.size() && date[p] != '/') d = d * 10 + (date[p++] - '0');
    p++;
    while (p < (int)date.size() && date[p] != '/') m = m * 10 + (date[p++] - '0');
    p++;
    while (p < (int)date.size()) y = y * 10 + (date[p++] - '0');
    return y * 10000 + m * 100 + d;
}

string getNextDateLocal(string currentDate) {
    int day = 0, month = 0, year = 0, pos = 0;
    int dim[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    while (pos < (int)currentDate.size() && currentDate[pos] != '/')
        day = day * 10 + (currentDate[pos++] - '0');
    pos++;
    while (pos < (int)currentDate.size() && currentDate[pos] != '/')
        month = month * 10 + (currentDate[pos++] - '0');
    pos++;
    while (pos < (int)currentDate.size())
        year = year * 10 + (currentDate[pos++] - '0');

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        dim[2] = 29;

    day++;
    if (day > dim[month]) {
        day = 1; month++;
        if (month > 12) { month = 1; year++; }
    }

    string r;
    if (day < 10) r += "0";
    r += to_string(day) + "/";
    if (month < 10) r += "0";
    r += to_string(month) + "/";
    r += to_string(year);
    return r;
}

bool isLayoverFeasibleLocal(const Routes& prevRoute, const Routes& nextRoute) {
    int aMin = timeToMinutesLocal(prevRoute.arrTime);
    int dMin = timeToMinutesLocal(nextRoute.depTime);
    const int MIN_LAY = 15;
    if (aMin == -1 || dMin == -1) return false;

    int prevDate = dateToIntLocal(prevRoute.date);
    int nextDate = dateToIntLocal(nextRoute.date);

    if (prevDate == nextDate) return dMin >= aMin + MIN_LAY;

    string expNext = getNextDateLocal(prevRoute.date);
    if (dateToIntLocal(expNext) == nextDate) {
        int untilMid = 24 * 60 - aMin;
        return (untilMid + dMin) >= MIN_LAY;
    }
    return false;
}

bool findGraphicalItinerary(const std::string& src, const std::string& dst, const std::string& date) {
    g_visualLegCount = 0;
    int s = findPortIndexByName(src);
    int d = findPortIndexByName(dst);
    if (s == -1 || d == -1 || src == dst) return false;

    const int MAX_DEPTH = MAX_VISUAL_LEGS;
    const int MAX_NODES_PER_LEVEL = 200;

    struct Node { int portIndex; int routeIndexFromParent; };
    struct Parent { int prevLevel; int prevIndex; };

    Node levelNodes[MAX_DEPTH + 1][MAX_NODES_PER_LEVEL];
    Parent parent[MAX_DEPTH + 1][MAX_NODES_PER_LEVEL];
    int levelCount[MAX_DEPTH + 1];

    levelNodes[0][0].portIndex = s;
    levelNodes[0][0].routeIndexFromParent = -1;
    parent[0][0].prevLevel = -1;
    parent[0][0].prevIndex = -1;
    levelCount[0] = 1;
    for (int i = 1; i <= MAX_DEPTH; ++i) levelCount[i] = 0;

    bool found = false;
    int fLevel = -1, fIndex = -1;

    for (int depth = 0; depth < MAX_DEPTH && !found; ++depth) {
        for (int idx = 0; idx < levelCount[depth] && !found; ++idx) {
            int curPort = levelNodes[depth][idx].portIndex;
            int inRoute = levelNodes[depth][idx].routeIndexFromParent;

            for (int r = 0; r < routeCount && !found; ++r) {
                if (!routes[r].valid) continue;
                if (routes[r].originIndex != curPort) continue;

                if (depth == 0 && routes[r].date != date) continue;
                if (depth > 0 && inRoute != -1) {
                    if (!isLayoverFeasibleLocal(routes[inRoute], routes[r])) continue;
                }

                int nextPort = routes[r].destinationIndex;
                int& nextCount = levelCount[depth + 1];
                if (nextCount >= MAX_NODES_PER_LEVEL) break;

                levelNodes[depth + 1][nextCount].portIndex = nextPort;
                levelNodes[depth + 1][nextCount].routeIndexFromParent = r;
                parent[depth + 1][nextCount].prevLevel = depth;
                parent[depth + 1][nextCount].prevIndex = idx;
                nextCount++;

                if (nextPort == d) {
                    found = true;
                    fLevel = depth + 1;
                    fIndex = nextCount - 1;
                    break;
                }
            }
        }
    }

    if (!found) return false;

    int routeIdx[MAX_VISUAL_LEGS];
    int len = 0;
    int cl = fLevel, ci = fIndex;
    while (cl > 0 && ci >= 0 && len < MAX_VISUAL_LEGS) {
        int ri = levelNodes[cl][ci].routeIndexFromParent;
        routeIdx[len++] = ri;
        int nl = parent[cl][ci].prevLevel;
        int ni = parent[cl][ci].prevIndex;
        cl = nl; ci = ni;
    }
    // reverse
    for (int i = 0; i < len / 2; ++i) {
        int tmp = routeIdx[i];
        routeIdx[i] = routeIdx[len - 1 - i];
        routeIdx[len - 1 - i] = tmp;
    }

    g_visualLegCount = len;
    for (int i = 0; i < len; ++i) {
        g_selectedItinerary[i].routeIndex = routeIdx[i];
        if (i + 1 < len) {
            g_selectedItinerary[i].layoverFeasible =
                isLayoverFeasibleLocal(routes[routeIdx[i]], routes[routeIdx[i + 1]]);
        }
        else {
            g_selectedItinerary[i].layoverFeasible = true;
        }
    }
    return true;
}


            const string routeFile = "routes.txt";
            const string mapFile   = "world_map.png";


        void runMap(sf::RenderWindow &window, Graph &g)
        {
            loadPorts();
            loadOceanWaypoints();
            if (!loadRoutes(routeFile)) return;

            sf::Texture mapTexture;
            if (!mapTexture.loadFromFile(mapFile)) {
                cerr << "Failed to load map: " << mapFile << endl;
                return;
            }

            sf::Sprite mapSprite(mapTexture);
            sf::Vector2u mapSize = mapTexture.getSize();

            for (int i = 0; i < portCount; ++i)
                ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);

            for (int i = 0; i < WAYPOINT_COUNT; ++i)
                oceanWaypoints[i].pixel = latlonToPixel(oceanWaypoints[i].lat, oceanWaypoints[i].lon, mapSize);

            float scaleX = float(window.getSize().x) / mapSize.x;
            float scaleY = float(window.getSize().y) / mapSize.y;
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
            while (window.isOpen())
            {
                sf::Event e;
                while (window.pollEvent(e)) 
                {
                    if (e.type == sf::Event::Closed) window.close();

                    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                        return;

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

                    float thickness = (isSel || isHover) ? 2.5f : 1.0f;
                    drawOceanRoute(window, pathPoints, pathLength, lineColor,thickness);
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
                window.display();
            }
}

void shipBooking(sf::RenderWindow &window, Graph &g)
{
            loadPorts();
            loadOceanWaypoints();
            if (!loadRoutes(routeFile)) return;

            sf::Texture mapTexture;
            if (!mapTexture.loadFromFile(mapFile)) {
                cerr << "Failed to load map: " << mapFile << endl;
                return;
            }

            sf::Font font;
            font.loadFromFile("Arial.ttf");

            sf::Sprite mapSprite(mapTexture);
            sf::Vector2u mapSize = mapTexture.getSize();

            for (int i = 0; i < portCount; ++i)
                ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);

            for (int i = 0; i < WAYPOINT_COUNT; ++i)
                oceanWaypoints[i].pixel = latlonToPixel(oceanWaypoints[i].lat, oceanWaypoints[i].lon, mapSize);

            float scaleX = float(window.getSize().x) / mapSize.x;
            float scaleY = float(window.getSize().y) / mapSize.y;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

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

            sf::CircleShape portShape(PORT_RADIUS);
            portShape.setOrigin(PORT_RADIUS, PORT_RADIUS);
            portShape.setFillColor(sf::Color::White);
            portShape.setOutlineThickness(1);
            portShape.setOutlineColor(sf::Color(120, 120, 120));

            sf::CircleShape portHighlight(HIGHLIGHT_RADIUS);
            portHighlight.setOrigin(HIGHLIGHT_RADIUS, HIGHLIGHT_RADIUS);
            portHighlight.setOutlineThickness(2);
            portHighlight.setOutlineColor(sf::Color::Yellow);
            portHighlight.setFillColor(sf::Color::Transparent);

            bool dragging = false;
            sf::Vector2i lastMouse;
            int hoveredPort = -1, hoveredRoute = -1, selectedRoute = -1;

            // Top booking controls
            TextBox srcBox(30.f, 15.f, 240.f, 45.f, font);
            TextBox dstBox(290.f, 15.f, 240.f, 45.f, font);
            TextBox dateBox(550.f, 15.f, 180.f, 45.f, font);
            dateBox.clear();
            dateBox.text.setString("DD/MM/YYYY");

            
            Button findBtn(750.f, 15.f, 200.f, 45.f, "FIND ROUTE", font,
               sf::Color(0, 120, 220), sf::Color::White);

            // ----------- MAP INNER LOOP -------------
            while (window.isOpen())
            {
                sf::Event e;
                while (window.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed) window.close();

                    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                        return;

                    if (e.type == sf::Event::MouseWheelScrolled) {
                        zoom *= (e.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                        zoom = std::max(0.1f, std::min(zoom, 10.f));
                        view.setSize(window.getDefaultView().getSize().x * zoom,
                            window.getDefaultView().getSize().y * zoom);
                    }

                    if (e.type == sf::Event::MouseButtonPressed &&
                        e.mouseButton.button == sf::Mouse::Left) {

                        dragging = true;
                        lastMouse = sf::Mouse::getPosition(window);

                        // GUI hit-testing in default view
                        sf::Vector2f guiPos = window.mapPixelToCoords(
                            sf::Mouse::getPosition(window), window.getDefaultView()
                        );

                        bool srcHit = srcBox.isMouseOver(guiPos);
                        bool dstHit = dstBox.isMouseOver(guiPos);
                        bool dateHit = dateBox.isMouseOver(guiPos);

                        srcBox.setActive(srcHit);
                        dstBox.setActive(dstHit);
                        dateBox.setActive(dateHit);

                        if (findBtn.isMouseOver(guiPos)) {
                        std::string src = srcBox.getString();
                        std::string dst = dstBox.getString();
                        std::string date = dateBox.getString();
                        bool found = findGraphicalItinerary(src, dst, date);
                        if (!found) {
                            sf::Text msg("No valid route found!", font, 22);
                            msg.setFillColor(sf::Color::Red);
                            msg.setPosition(30.f, 70.f);
                            window.draw(msg);
                            window.display();
                            sf::sleep(sf::seconds(2));
                        }
                    }

                    }

                    if (e.type == sf::Event::MouseButtonReleased &&
                        e.mouseButton.button == sf::Mouse::Left) {
                        dragging = false;
                        if (hoveredRoute != -1) {
                            if (selectedRoute == hoveredRoute) selectedRoute = -1;
                            else selectedRoute = hoveredRoute;
                        }
                    }

                    if (e.type == sf::Event::MouseMoved && dragging) {
                        sf::Vector2i now = sf::Mouse::getPosition(window);
                        sf::Vector2f delta =
                            window.mapPixelToCoords(lastMouse, view) -
                            window.mapPixelToCoords(now, view);
                        view.move(delta);
                        lastMouse = now;
                    }

                    if (e.type == sf::Event::TextEntered) {
                        srcBox.handleEvent(e);
                        dstBox.handleEvent(e);
                        dateBox.handleEvent(e);
                    }
                }

                // world-space mouse for hovering
                sf::Vector2i mousePix = sf::Mouse::getPosition(window);
                sf::Vector2f world = window.mapPixelToCoords(mousePix, view);

                hoveredPort = -1;
                hoveredRoute = -1;

                // HOVER PORTS
                for (int i = 0; i < portCount; ++i) {
                    float dx = ports[i].pixel.x - world.x;
                    float dy = ports[i].pixel.y - world.y;
                    if (std::sqrt(dx * dx + dy * dy) <= 10.f) {
                        hoveredPort = i;
                        break;
                    }
                }

                // HOVER ROUTES
                for (int i = 0; i < routeCount; ++i) {
                    if (!routes[i].valid) continue;
                    sf::Vector2f a = ports[routes[i].originIndex].pixel;
                    sf::Vector2f b = ports[routes[i].destinationIndex].pixel;
                    sf::Vector2f ap = world - a;
                    sf::Vector2f ab = b - a;
                    float ab2 = ab.x * ab.x + ab.y * ab.y;
                    if (ab2 == 0) continue;
                    float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
                    t = std::max(0.f, std::min(t, 1.f));
                    sf::Vector2f proj = a + ab * t;
                    float dist = std::sqrt((proj.x - world.x) * (proj.x - world.x) +
                        (proj.y - world.y) * (proj.y - world.y));
                    if (dist <= 8.f) {
                        hoveredRoute = i;
                        break;
                    }
                }

                // DRAW MAP & ROUTES
                window.clear(sf::Color(10, 25, 50));
                window.setView(view);
                window.draw(mapSprite);

                // All background routes
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
                        fromHover ? sf::Color(255, 140, 0) :
                        sf::Color(120, 150, 190, 120);

                    sf::Vector2f pathPoints[MAX_WAYPOINTS];
                    int pathLength = 0;
                    generateOceanPath(a, b,
                        routes[i].originIndex, routes[i].destinationIndex,
                        pathPoints, pathLength);

                    float thickness = (isSel || isHover) ? 2.5f : 1.0f;
                    drawOceanRoute(window, pathPoints, pathLength, lineColor, thickness);
                }

                // Highlight graphical itinerary
                if (g_visualLegCount > 0) {
                    // Thick cyan curves
                    for (int idx = 0; idx < g_visualLegCount; ++idx) {
                        const Routes& r = routes[g_selectedItinerary[idx].routeIndex];
                        sf::Vector2f a = ports[r.originIndex].pixel;
                        sf::Vector2f b = ports[r.destinationIndex].pixel;

                        sf::Vector2f pathPoints[MAX_WAYPOINTS];
                        int pathLength = 0;
                        generateOceanPath(a, b,
                            r.originIndex, r.destinationIndex,
                            pathPoints, pathLength);

                        drawOceanRoute(window, pathPoints, pathLength,
                            sf::Color(0, 255, 255), 4.f);
                    }

                    // Layover circles
                    sf::CircleShape layoverCircle(9.f);
                    layoverCircle.setOrigin(9.f, 9.f);
                    layoverCircle.setOutlineThickness(2.f);

                    for (int idx = 0; idx + 1 < g_visualLegCount; ++idx) {
                        const Routes& r = routes[g_selectedItinerary[idx].routeIndex];
                        int layoverPortIdx = r.destinationIndex;
                        layoverCircle.setPosition(ports[layoverPortIdx].pixel);

                        if (g_selectedItinerary[idx].layoverFeasible) {
                            layoverCircle.setFillColor(sf::Color(0, 150, 0, 90));
                            layoverCircle.setOutlineColor(sf::Color(0, 255, 0));
                        }
                        else {
                            layoverCircle.setFillColor(sf::Color(150, 0, 0, 90));
                            layoverCircle.setOutlineColor(sf::Color(255, 0, 0));
                        }
                        window.draw(layoverCircle);
                    }
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

                // Back to default view for UI overlays
                window.setView(window.getDefaultView());

                // Booking controls
                srcBox.draw(window);
                dstBox.draw(window);
                dateBox.draw(window);
                findBtn.draw(window);

                // Port dropdown / route tooltip
                if (hoveredPort != -1)
                    drawPortDropdown(window, font, hoveredPort, mousePix);
                else if (hoveredRoute != -1) {
                    Routes& r = routes[hoveredRoute];

                    sf::RectangleShape tooltip(sf::Vector2f(400, 60));
                    tooltip.setPosition(mousePix.x + 15, mousePix.y + 15);
                    tooltip.setFillColor(sf::Color(40, 40, 40, 240));
                    tooltip.setOutlineThickness(2);
                    tooltip.setOutlineColor(sf::Color::Yellow);
                    window.draw(tooltip);

                    sf::Text info1(
                        ports[r.originIndex].name + " -> " + ports[r.destinationIndex].name,
                        font, 13);
                    info1.setFillColor(sf::Color::White);
                    info1.setStyle(sf::Text::Bold);
                    info1.setPosition(mousePix.x + 25, mousePix.y + 20);
                    window.draw(info1);

                    sf::Text info2(r.date + " | " + r.depTime + " - " + r.arrTime,
                        font, 11);
                    info2.setFillColor(sf::Color(200, 200, 200));
                    info2.setPosition(mousePix.x + 25, mousePix.y + 40);
                    window.draw(info2);

                    sf::Text info3("Cost: $" + intToString(r.vCost) + " | " + r.company,
                        font, 11);
                    info3.setFillColor(sf::Color(150, 220, 255));
                    info3.setPosition(mousePix.x + 25, mousePix.y + 55);
                    window.draw(info3);
                }

                // ========== RIGHT-SIDE "Selected Route" PANEL ==========
                if (g_visualLegCount > 0) {
                    float panelWidth = 300.f;
                    sf::RectangleShape side(sf::Vector2f(panelWidth, window.getSize().y));
                    side.setPosition(window.getSize().x - panelWidth, 0.f);
                    side.setFillColor(sf::Color(5, 10, 30, 230));
                    window.draw(side);

                    sf::Text title("Selected Route", font, 28);
                    title.setFillColor(sf::Color(0, 220, 255));
                    title.setStyle(sf::Text::Bold);
                    title.setPosition(side.getPosition().x + 10.f, 10.f);
                    window.draw(title);

                    float y = 40.f;
                    int totalCost = 0;

                    // Starting port name for leg 1:
                    int firstLegRouteIndex = g_selectedItinerary[0].routeIndex;
                    string currentPort = ports[routes[firstLegRouteIndex].originIndex].name;

                    for (int i = 0; i < g_visualLegCount; ++i) {
                        const Routes& r = routes[g_selectedItinerary[i].routeIndex];
                        totalCost += r.vCost;

                        // Leg heading
                        sf::Text legTitle("Leg " + intToString(i + 1) + ": " +
                            currentPort + " -> " +
                            ports[r.destinationIndex].name,
                            font, 20);
                        legTitle.setFillColor(sf::Color::White);
                        legTitle.setPosition(side.getPosition().x + 10.f, y);
                        window.draw(legTitle);
                        y += 18.f;

                        // Date & times
                        sf::Text times("Date: " + r.date + "   " +
                            r.depTime + " - " + r.arrTime,
                            font, 18);
                        times.setFillColor(sf::Color(200, 200, 200));
                        times.setPosition(side.getPosition().x + 14.f, y);
                        window.draw(times);
                        y += 16.f;

                        // Cost & company
                        sf::Text cost("Cost: $" + intToString(r.vCost) +
                            "   Company: " + r.company,
                            font, 18);
                        cost.setFillColor(sf::Color(180, 220, 255));
                        cost.setPosition(side.getPosition().x + 14.f, y);
                        window.draw(cost);
                        y += 18.f;

                        // Layover info (for all but last leg)
                        if (i + 1 < g_visualLegCount) {
                            bool ok = g_selectedItinerary[i].layoverFeasible;
                            sf::Text layover(
                                std::string("Layover at ") +
                                ports[r.destinationIndex].name + ": " +
                                (ok ? "OK" : "Too short / invalid"),
                                font, 18);
                            layover.setFillColor(ok ? sf::Color(0, 230, 0)
                                : sf::Color(255, 80, 80));
                            layover.setPosition(side.getPosition().x + 18.f, y);
                            window.draw(layover);
                            y += 20.f;
                        }

                        y += 6.f;
                        currentPort = ports[r.destinationIndex].name;
                    }

                    // Total cost at bottom
                    sf::Text total("Total cost: $" + intToString(totalCost),
                        font, 18);
                    total.setFillColor(sf::Color(255, 220, 120));
                    total.setPosition(side.getPosition().x + 10.f,
                        window.getSize().y - 40.f);
                    window.draw(total);
                }

                // Instructions
                sf::Text instructions(
                    "Controls: Mouse Wheel = Zoom | Drag = Pan | ESC = Menu | Hover for details",
                    font, 12);
                instructions.setFillColor(sf::Color(200, 200, 200));
                instructions.setPosition(10.f, window.getSize().y - 22.f);
                window.draw(instructions);

                window.display();
            }
}