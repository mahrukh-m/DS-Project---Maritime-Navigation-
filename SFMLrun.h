#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;
int findPortIndexByName(const string& name);

struct SFMLPreferences;
extern SFMLPreferences sfmlPreferences;
const int totalPorts = 40;
const int totalRoutes = 2000;
const int totalWayPoints = 10;
const float portRadius = 5.f;
const float highlightRadius = 8.f;

struct ExploredEdge
{
    int fromPortIndex;
    int toPortIndex;
    bool isOptimal;        
    bool isBeingExplored;  
    float explorationTime;  
    float alpha;           
};

struct AlgorithmVisualization
{
    static const int totalExplored = 200;
    ExploredEdge exploredEdges[totalExplored];
    int exploredCount;

    int currentExploringPort;     
    float explorationProgress;      
    float explorationSpeed;         
    bool isAnimating;              
    bool showFinalPath;           
    float animationTimer;

    int explorationOrder[50];      
    int explorationOrderCount;
    int currentStep;

    int finalPath[50];
    int finalPathCount;

    AlgorithmVisualization()
    {
        exploredCount = 0;
        currentExploringPort = -1;
        explorationProgress = 0.f;
        explorationSpeed = 2.f;
        isAnimating = false;
        showFinalPath = false;
        animationTimer = 0.f;
        explorationOrderCount = 0;
        currentStep = 0;
        finalPathCount = 0;
    }

    void clear()
    {
        exploredCount = 0;
        currentExploringPort = -1;
        explorationProgress = 0.f;
        isAnimating = false;
        showFinalPath = false;
        animationTimer = 0.f;
        explorationOrderCount = 0;
        currentStep = 0;
        finalPathCount = 0;
    }

    void addExploredEdge(int from, int to, bool optimal = false)
    {
        if (exploredCount < totalExplored)
        {
            exploredEdges[exploredCount].fromPortIndex = from;
            exploredEdges[exploredCount].toPortIndex = to;
            exploredEdges[exploredCount].isOptimal = optimal;
            exploredEdges[exploredCount].isBeingExplored = true;
            exploredEdges[exploredCount].explorationTime = animationTimer;
            exploredEdges[exploredCount].alpha = 255.f;
            exploredCount++;
        }
    }

    void addExplorationStep(int portIndex)
    {
        if (explorationOrderCount < 50)
        {
            explorationOrder[explorationOrderCount++] = portIndex;
        }
    }

    void setFinalPath(int* path, int count)
    {
        finalPathCount = count;
        for (int i = 0; i < count && i < 50; i++)
        {
            finalPath[i] = path[i];
        }
    }

    void startAnimation()
    {
        isAnimating = true;
        showFinalPath = false;
        currentStep = 0;
        animationTimer = 0.f;
        explorationProgress = 0.f;

        for (int i = 0; i < exploredCount; i++)
        {
            exploredEdges[i].isBeingExplored = false;
            exploredEdges[i].alpha = 0.f;
        }
    }

    void update(float deltaTime)
    {
        if (!isAnimating) 
        {
            return;
        }

        animationTimer += deltaTime;
        explorationProgress += deltaTime * explorationSpeed;

        int targetStep = (int)(explorationProgress * explorationOrderCount);

        if (targetStep > currentStep && currentStep < explorationOrderCount)
        {
            currentStep = targetStep;
            currentExploringPort = explorationOrder[currentStep - 1];

            for (int i = 0; i < exploredCount; i++)
            {
                if (exploredEdges[i].fromPortIndex == currentExploringPort)
                {
                    exploredEdges[i].isBeingExplored = true;
                }
            }
        }

        for (int i = 0; i < exploredCount; i++)
        {
            if (exploredEdges[i].isBeingExplored)
            {
                if (exploredEdges[i].alpha < 255.f)
                {
                    exploredEdges[i].alpha += deltaTime * 500.f;
                    if (exploredEdges[i].alpha > 255.f) exploredEdges[i].alpha = 255.f;
                }
            }
            else
            {
                if (exploredEdges[i].alpha > 0.f)
                {
                    exploredEdges[i].alpha -= deltaTime * 100.f;
                    if (exploredEdges[i].alpha < 0.f) exploredEdges[i].alpha = 0.f;
                }
            }
        }

        if (currentStep >= explorationOrderCount)
        {
            if (animationTimer > explorationOrderCount / explorationSpeed + 1.0f)
            {
                showFinalPath = true;
                isAnimating = false;
            }
        }
    }

    void skipToEnd()
    {
        isAnimating = false;
        showFinalPath = true;
        currentStep = explorationOrderCount;

        for (int i = 0; i < exploredCount; i++)
        {
            exploredEdges[i].isBeingExplored = true;
            exploredEdges[i].alpha = 100.f; 
        }
    }
};

AlgorithmVisualization algorithmVisualization;
string intToString(int val);
void generateOceanPath(sf::Vector2f start, sf::Vector2f end, int oi, int di, sf::Vector2f* pts, int& len);

struct Port 
{
    string name;
    double lat;
    double lon;
    sf::Vector2f pixel;
};

struct Routes 
{
    int originIndex;
    int destinationIndex;
    string date;
    string depTime;
    string arrTime;
    int vCost;
    string company;
    bool valid;
};

struct Waypoint 
{
    double lat;
    double lon;
    sf::Vector2f pixel;
};

Port ports[totalPorts];
int portCount = 0;
Routes routes[totalRoutes];
int routeCount = 0;

const int waypointCount = 15;
Waypoint oceanWaypoints[waypointCount];

struct VisualLeg 
{
    int routeIndex;
    bool layoverFeasible;
};

const int totalVisualLegs = 10;
VisualLeg selectedItinerary[totalVisualLegs];
int visualLegCount = 0;

struct AlgorithmLeg 
{
    int fromPortIndex;
    int toPortIndex;
    string date;
    string depTime;
    string arrTime;
    int cost;
    string company;
};

const int totalAlgorithmLegs = 10;
AlgorithmLeg algorithmPart[totalAlgorithmLegs];
int algorithmLegCount = 0;
string algorithmName = "";
string algorithmMetric = "";
int algorithmTotalValue = 0;

struct DockEvent
{
    int portIndex;
    string shipName;
    string eventType;  
    string time;
    float animationProgress;  
    bool isActive;
};

const int MAX_DOCK_EVENTS = 50;
DockEvent dockEvents[MAX_DOCK_EVENTS];
int dockEventCount = 0;

struct PortDockStatus
{
    int shipsInQueue;
    int shipsDocking;
    int maxSlots;
    string shipNames[10];
    bool isProcessing[10];
    int waitTimes[10];

    PortDockStatus()
    {
        shipsInQueue = 0;
        shipsDocking = 0;
        maxSlots = 2;
        for (int i = 0; i < 10; i++)
        {
            shipNames[i] = "";
            isProcessing[i] = false;
            waitTimes[i] = 0;
        }
    }
};

PortDockStatus portDockStatus[totalPorts];
void addDockEvent(int portIndex, const string& shipName, const string& eventType, const string& time)
{
    if (dockEventCount < MAX_DOCK_EVENTS)
    {
        dockEvents[dockEventCount].portIndex = portIndex;
        dockEvents[dockEventCount].shipName = shipName;
        dockEvents[dockEventCount].eventType = eventType;
        dockEvents[dockEventCount].time = time;
        dockEvents[dockEventCount].animationProgress = 0.0f;
        dockEvents[dockEventCount].isActive = true;
        dockEventCount++;
    }
}

void clearDockEvents()
{
    dockEventCount = 0;
}

void updatePortDockStatus(int portIndex, int inQueue, int docking, int maxSlots)
{
    if (portIndex >= 0 && portIndex < totalPorts)
    {
        portDockStatus[portIndex].shipsInQueue = inQueue;
        portDockStatus[portIndex].shipsDocking = docking;
        portDockStatus[portIndex].maxSlots = maxSlots;
    }
}

class AnimatedDockView
{
private:
    sf::Font* fontPtr;
    float viewX, viewY, viewWidth, viewHeight;
    float animPhase;

    struct ShipBox
    {
        string name;
        string routeId;          
        sf::Vector2f position;
        sf::Vector2f targetPosition;
        float progress;
        string status;     
        bool isActive;
        int slotIndex;          

        ShipBox()
        {
            name = "";
            routeId = "";
            progress = 0.f;
            status = "";
            isActive = false;
            slotIndex = -1;
        }
    };

    static const int totalShips = 10;
    ShipBox ships[totalShips];
    int shipCount;

    static const int totalDockSlots = 4;
    int maxDockSlots;
    int currentlyDocked;
    string currentPortName;

    float queueStartX;
    float queueY;
    float dockStartX;
    float dockY;
    float boxWidth;
    float boxHeight;
    float boxSpacing;

public:
    bool isVisible;

    AnimatedDockView()
    {
        fontPtr = nullptr;
        viewX = viewY = viewWidth = viewHeight = 0;
        animPhase = 0.f;
        shipCount = 0;
        maxDockSlots = 2;
        currentlyDocked = 0;
        currentPortName = "";
        isVisible = true;

        queueStartX = 0.f;
        queueY = 0.f;
        dockStartX = 0.f;
        dockY = 0.f;
        boxWidth = 60.f;
        boxHeight = 35.f;
        boxSpacing = 8.f;
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;

        viewWidth = windowWidth - 330.f;
        viewHeight = 130.f;
        viewX = 320.f;
        viewY = windowHeight - viewHeight - 10.f;

        queueStartX = viewX + 120.f;
        queueY = viewY + 75.f;
        dockStartX = viewX + viewWidth - 200.f;
        dockY = viewY + 75.f;

        for (int i = 0; i < totalShips; i++)
        {
            ships[i].isActive = false;
        }
    }

    void setPort(const string& portName, int dockSlots)
    {
        currentPortName = portName;
        maxDockSlots = dockSlots;
        if (maxDockSlots < 1) 
        {
            maxDockSlots = 2;
        }
        if (maxDockSlots > totalDockSlots) 
        {
            maxDockSlots = totalDockSlots;
        }
    }

    void setDockedCount(int count)
    {
        currentlyDocked = count;
        if (currentlyDocked > maxDockSlots) 
        {
            currentlyDocked = maxDockSlots;
        }
    }

    bool hasShipForRoute(const string& routeId)
    {
        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && ships[i].routeId == routeId)
            {
                return true;
            }
        }
        return false;
    }

    void enqueueShip(const string& shipName, const string& source, const string& destination)
    {
        string routeId = source + "_" + destination;

        if (hasShipForRoute(routeId))
        {
            return;  
        }

        int index = -1;
        for (int i = 0; i < totalShips; i++)
        {
            if (!ships[i].isActive)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            return;
        }

        int queuePos = 0;
        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && (ships[i].status == "QUEUED" || ships[i].status == "ENTERING"))
            {
                queuePos++;
            }
        }

        ships[index].name = shipName;
        ships[index].routeId = routeId;
        ships[index].isActive = true;
        ships[index].status = "ENTERING";
        ships[index].progress = 0.f;
        ships[index].slotIndex = -1;

        ships[index].position = sf::Vector2f(viewX - boxWidth, queueY);
        ships[index].targetPosition = sf::Vector2f(queueStartX + queuePos * (boxWidth + boxSpacing), queueY);
        shipCount++;
        tryDockNextShip();
    }

    void tryDockNextShip()
    {
        if (currentlyDocked >= maxDockSlots) 
        {
            return;
        }

        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && ships[i].status == "QUEUED")
            {
                int emptySlot = -1;
                for (int s = 0; s < maxDockSlots; s++)
                {
                    bool slotTaken = false;
                    for (int j = 0; j < totalShips; j++)
                    {
                        if (ships[j].isActive && ships[j].status == "DOCKING" && ships[j].slotIndex == s)
                        {
                            slotTaken = true;
                            break;
                        }
                    }
                    if (!slotTaken)
                    {
                        emptySlot = s;
                        break;
                    }
                }

                if (emptySlot != -1)
                {
                    ships[i].status = "DOCKING";
                    ships[i].slotIndex = emptySlot;
                    ships[i].progress = 0.f;
                    ships[i].targetPosition = sf::Vector2f(dockStartX + emptySlot * (boxWidth + boxSpacing), dockY);
                    currentlyDocked++;
                }
                break;
            }
        }
    }

    void dequeueShip(const string& source, const string& destination)
    {
        string routeId = source + "_" + destination;

        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && ships[i].routeId == routeId)
            {
                ships[i].status = "DEPARTING";
                ships[i].progress = 0.f;
                ships[i].targetPosition = sf::Vector2f(viewX + viewWidth + boxWidth, ships[i].position.y);

                if (ships[i].slotIndex != -1)
                {
                    currentlyDocked--;
                    ships[i].slotIndex = -1;
                }
                break;
            }
        }
    }

    void clear()
    {
        for (int i = 0; i < totalShips; i++)
        {
            ships[i].isActive = false;
        }
        shipCount = 0;
        currentlyDocked = 0;
    }

    void update(float deltaTime)
    {
        if (!isVisible) 
        {
            return;
        }

        animPhase += deltaTime * 2.f;
        if (animPhase > 6.28318f) animPhase -= 6.28318f;

        for (int i = 0; i < totalShips; i++)
        {
            if (!ships[i].isActive) 
            {
                continue;
            }

            if (ships[i].status == "ENTERING" || ships[i].status == "DOCKING" || ships[i].status == "DEPARTING")
            {
                ships[i].progress += deltaTime * 2.f;

                if (ships[i].progress >= 1.f)
                {
                    ships[i].progress = 1.f;
                    ships[i].position = ships[i].targetPosition;

                    if (ships[i].status == "ENTERING")
                    {
                        ships[i].status = "QUEUED";
                        tryDockNextShip();
                    }
                    else if (ships[i].status == "DEPARTING")
                    {
                        ships[i].isActive = false;
                        shipCount--;
                        tryDockNextShip();
                    }
                }
                else
                {
                    float t = ships[i].progress;
                    float smoothT = t * t * (3.f - 2.f * t);

                    ships[i].position.x = ships[i].position.x + (ships[i].targetPosition.x - ships[i].position.x) * smoothT * deltaTime * 5.f;
                    ships[i].position.y = ships[i].position.y + (ships[i].targetPosition.y - ships[i].position.y) * smoothT * deltaTime * 5.f;
                }
            }
        }

        int queuePos = 0;
        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && ships[i].status == "QUEUED")
            {
                float targetX = queueStartX + queuePos * (boxWidth + boxSpacing);
                if (abs(ships[i].position.x - targetX) > 1.f)
                {
                    ships[i].position.x += (targetX - ships[i].position.x) * deltaTime * 5.f;
                }
                queuePos++;
            }
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible) 
        {
            return;
        }

        sf::RectangleShape bg(sf::Vector2f(viewWidth, viewHeight));
        bg.setPosition(viewX, viewY);
        bg.setFillColor(sf::Color(15, 25, 45, 240));
        bg.setOutlineThickness(2.f);
        bg.setOutlineColor(sf::Color(0, 150, 200));
        window.draw(bg);

        sf::RectangleShape titleBar(sf::Vector2f(viewWidth, 28.f));
        titleBar.setPosition(viewX, viewY);
        float glow = (sin(animPhase) + 1.f) / 2.f;
        titleBar.setFillColor(sf::Color(0, (sf::Uint8)(80 + glow * 30), (sf::Uint8)(120 + glow * 30), 230));
        window.draw(titleBar);

        if (fontPtr == nullptr) 
        {
            return;
        }

        sf::Text title("DOCKING QUEUE", *fontPtr, 13);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        title.setPosition(viewX + 15.f, viewY + 6.f);
        window.draw(title);

        if (!currentPortName.empty())
        {
            sf::Text portText("Port: " + currentPortName, *fontPtr, 11);
            portText.setFillColor(sf::Color(100, 200, 255));
            portText.setPosition(viewX + 160.f, viewY + 8.f);
            window.draw(portText);
        }

        sf::Text countText("Ships: " + intToString(shipCount), *fontPtr, 11);
        countText.setFillColor(sf::Color(150, 200, 255));
        countText.setPosition(viewX + viewWidth - 80.f, viewY + 8.f);
        window.draw(countText);

        sf::Text queueLabel("QUEUE", *fontPtr, 11);
        queueLabel.setFillColor(sf::Color(255, 200, 100));
        queueLabel.setStyle(sf::Text::Bold);
        queueLabel.setPosition(viewX + 20.f, viewY + 40.f);
        window.draw(queueLabel);

        float queueAreaWidth = (boxWidth + boxSpacing) * 5 + 10.f;
        sf::RectangleShape queueArea(sf::Vector2f(queueAreaWidth, boxHeight + 20.f));
        queueArea.setPosition(queueStartX - 10.f, queueY - 10.f);
        queueArea.setFillColor(sf::Color(30, 40, 60, 150));
        queueArea.setOutlineThickness(1.f);
        queueArea.setOutlineColor(sf::Color(80, 100, 130));
        window.draw(queueArea);

        for (int i = 0; i < 5; i++)
        {
            sf::RectangleShape emptySlot(sf::Vector2f(boxWidth, boxHeight));
            emptySlot.setPosition(queueStartX + i * (boxWidth + boxSpacing), queueY);
            emptySlot.setFillColor(sf::Color(25, 35, 50, 100));
            emptySlot.setOutlineThickness(1.f);
            emptySlot.setOutlineColor(sf::Color(50, 60, 80, 150));
            window.draw(emptySlot);
        }

        float arrowX = queueStartX + queueAreaWidth + 10.f;
        float arrowY = queueY + boxHeight / 2.f;

        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, sf::Vector2f(20.f, 0.f));
        arrow.setPoint(1, sf::Vector2f(0.f, -10.f));
        arrow.setPoint(2, sf::Vector2f(0.f, 10.f));
        arrow.setPosition(arrowX, arrowY);

        float arrowPulse = (sin(animPhase * 3.f) + 1.f) / 2.f;
        arrow.setFillColor(sf::Color(100 + (int)(arrowPulse * 100), 200, 100, 200));
        window.draw(arrow);

        sf::Text dockLabel("DOCK SLOTS", *fontPtr, 11);
        dockLabel.setFillColor(sf::Color(100, 255, 150));
        dockLabel.setStyle(sf::Text::Bold);
        dockLabel.setPosition(dockStartX - 10.f, viewY + 40.f);
        window.draw(dockLabel);

        for (int i = 0; i < maxDockSlots; i++)
        {
            float slotX = dockStartX + i * (boxWidth + boxSpacing);

            sf::RectangleShape dockSlot(sf::Vector2f(boxWidth, boxHeight));
            dockSlot.setPosition(slotX, dockY);
            dockSlot.setOutlineThickness(2.f);

            bool occupied = false;
            for (int j = 0; j < totalShips; j++)
            {
                if (ships[j].isActive && ships[j].status == "DOCKING" && ships[j].slotIndex == i)
                {
                    occupied = true;
                    break;
                }
            }

            if (occupied)
            {
                dockSlot.setFillColor(sf::Color(0, 150, 80, 200));
                dockSlot.setOutlineColor(sf::Color(0, 255, 100));
            }
            else
            {
                dockSlot.setFillColor(sf::Color(40, 50, 70, 150));
                dockSlot.setOutlineColor(sf::Color(80, 100, 130));
            }
            window.draw(dockSlot);

            sf::Text slotNum(intToString(i + 1), *fontPtr, 10);
            slotNum.setFillColor(sf::Color(150, 150, 170));
            slotNum.setPosition(slotX + boxWidth / 2.f - 4.f, dockY + boxHeight + 2.f);
            window.draw(slotNum);
        }

        for (int i = 0; i < totalShips; i++)
        {
            if (!ships[i].isActive) 
            {
                continue;
            }

            sf::RectangleShape shipBox(sf::Vector2f(boxWidth - 4.f, boxHeight - 4.f));
            shipBox.setPosition(ships[i].position.x + 2.f, ships[i].position.y + 2.f);
            shipBox.setOutlineThickness(2.f);

            if (ships[i].status == "ENTERING")
            {
                shipBox.setFillColor(sf::Color(80, 150, 255, 220));
                shipBox.setOutlineColor(sf::Color(100, 180, 255));
            }
            else if (ships[i].status == "QUEUED")
            {
                float pulse = (sin(animPhase + i * 0.5f) + 1.f) / 2.f;
                shipBox.setFillColor(sf::Color(255, 180 + (int)(pulse * 40), 50, 220));
                shipBox.setOutlineColor(sf::Color(255, 220, 100));
            }
            else if (ships[i].status == "DOCKING")
            {
                float pulse = (sin(animPhase * 2.f + i * 0.5f) + 1.f) / 2.f;
                shipBox.setFillColor(sf::Color(50, 200 + (int)(pulse * 55), 100, 220));
                shipBox.setOutlineColor(sf::Color(100, 255, 150));
            }
            else if (ships[i].status == "DEPARTING")
            {
                shipBox.setFillColor(sf::Color(100, 200, 200, 180));
                shipBox.setOutlineColor(sf::Color(150, 255, 255));
            }

            window.draw(shipBox);

            sf::Text nameText(ships[i].name, *fontPtr, 9);
            nameText.setFillColor(sf::Color::White);
            nameText.setStyle(sf::Text::Bold);
            sf::FloatRect bounds = nameText.getLocalBounds();
            nameText.setPosition(
                ships[i].position.x + (boxWidth - bounds.width) / 2.f,
                ships[i].position.y + 8.f
            );
            window.draw(nameText);

            string statusStr = ships[i].status;
            sf::Text statusText(statusStr, *fontPtr, 7);
            statusText.setFillColor(sf::Color(200, 200, 200));
            bounds = statusText.getLocalBounds();
            statusText.setPosition(
                ships[i].position.x + (boxWidth - bounds.width) / 2.f,
                ships[i].position.y + 22.f
            );
            window.draw(statusText);
        }

        int waiting = 0;
        for (int i = 0; i < totalShips; i++)
        {
            if (ships[i].isActive && (ships[i].status == "QUEUED" || ships[i].status == "ENTERING"))
            {
                waiting++;
            }
        }

        string statusStr = "Docked: " + intToString(currentlyDocked) + "/" + intToString(maxDockSlots) +
            "  | Waiting: " + intToString(waiting);
        sf::Text statusText(statusStr, *fontPtr, 10);
        statusText.setFillColor(sf::Color(150, 180, 220));
        statusText.setPosition(viewX + 20.f, viewY + viewHeight - 20.f);
        window.draw(statusText);
    }
};

AnimatedDockView animatedDockView;


class DockQueuePanel
{
private:
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::Text titleText;
    sf::RectangleShape queueDisplay;

    float panelX, panelY, panelWidth, panelHeight;
    sf::Font* fontPtr;
    float animPhase;
    struct ShipIcon
    {
        sf::ConvexShape shape;
        sf::Vector2f position;
        sf::Vector2f targetPosition;
        float progress;
        string name;
        string status;
        bool isActive;
    };

    ShipIcon shipIcons[10];
    int shipIconCount;

    int currentPortIndex;
    string currentPortName;

public:
    DockQueuePanel()
    {
        fontPtr = nullptr;
        animPhase = 0.f;
        panelX = panelY = panelWidth = panelHeight = 0;
        shipIconCount = 0;
        currentPortIndex = -1;
        currentPortName = "";
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;

        panelWidth = 350.f;
        panelHeight = 200.f;
        panelX = windowWidth - panelWidth - 20.f;
        panelY = windowHeight - panelHeight - 20.f;

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(10, 20, 40, 230));
        background.setOutlineThickness(2.f);
        background.setOutlineColor(sf::Color(0, 150, 200));

        headerBar.setSize(sf::Vector2f(panelWidth, 35.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(0, 100, 150, 220));

        titleText.setFont(font);
        titleText.setString("DOCK QUEUE");
        titleText.setCharacterSize(14);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(panelX + 15.f, panelY + 8.f);

        queueDisplay.setSize(sf::Vector2f(panelWidth - 20.f, panelHeight - 50.f));
        queueDisplay.setPosition(panelX + 10.f, panelY + 42.f);
        queueDisplay.setFillColor(sf::Color(5, 15, 30, 200));
        queueDisplay.setOutlineThickness(1.f);
        queueDisplay.setOutlineColor(sf::Color(40, 80, 120));

        for (int i = 0; i < 10; i++)
        {
            shipIcons[i].shape.setPointCount(5);
            shipIcons[i].shape.setPoint(0, sf::Vector2f(15.f, 0.f));
            shipIcons[i].shape.setPoint(1, sf::Vector2f(10.f, -6.f));
            shipIcons[i].shape.setPoint(2, sf::Vector2f(-10.f, -6.f));
            shipIcons[i].shape.setPoint(3, sf::Vector2f(-10.f, 6.f));
            shipIcons[i].shape.setPoint(4, sf::Vector2f(10.f, 6.f));
            shipIcons[i].isActive = false;
            shipIcons[i].progress = 0.f;
        }
    }

    void setPort(int portIndex, const string& portName)
    {
        currentPortIndex = portIndex;
        currentPortName = portName;
        titleText.setString("DOCK QUEUE - " + portName);
    }

    void updateQueue(int queueCount, int dockingCount, int maxSlots,
        const string shipNames[], const bool isProcessing[], int count)
    {
        shipIconCount = 0;

        float queueX = queueDisplay.getPosition().x + 20.f;
        float queueY = queueDisplay.getPosition().y + 30.f;
        float dockX = queueDisplay.getPosition().x + queueDisplay.getSize().x - 80.f;
        float dockY = queueDisplay.getPosition().y + 50.f;

        for (int i = 0; i < count && i < 10; i++)
        {
            shipIcons[shipIconCount].name = shipNames[i];
            shipIcons[shipIconCount].isActive = true;

            if (isProcessing[i])
            {
                shipIcons[shipIconCount].status = "DOCKING";
                shipIcons[shipIconCount].position = sf::Vector2f(dockX, dockY + shipIconCount * 25.f);
                shipIcons[shipIconCount].shape.setFillColor(sf::Color(0, 200, 100));
            }
            else
            {
                int waitingPos = shipIconCount;
                shipIcons[shipIconCount].status = "WAITING";
                shipIcons[shipIconCount].position = sf::Vector2f(queueX + waitingPos * 35.f, queueY);
                shipIcons[shipIconCount].shape.setFillColor(sf::Color(255, 200, 0));
            }

            shipIconCount++;
        }
    }

    void enqueueShip(const string& shipName)
    {
        if (shipIconCount < 10)
        {
            float queueX = queueDisplay.getPosition().x + 20.f;
            float queueY = queueDisplay.getPosition().y + 30.f;

            shipIcons[shipIconCount].name = shipName;
            shipIcons[shipIconCount].isActive = true;
            shipIcons[shipIconCount].status = "ENTERING";
            shipIcons[shipIconCount].progress = 0.f;

            shipIcons[shipIconCount].position = sf::Vector2f(queueX - 50.f, queueY);
            shipIcons[shipIconCount].targetPosition = sf::Vector2f(queueX + shipIconCount * 35.f, queueY);
            shipIcons[shipIconCount].shape.setFillColor(sf::Color(100, 200, 255));

            shipIconCount++;
        }
    }

    void dequeueShip(int index)
    {
        if (index >= 0 && index < shipIconCount)
        {
            shipIcons[index].status = "DEPARTING";
            shipIcons[index].progress = 0.f;
            shipIcons[index].targetPosition = sf::Vector2f(
                queueDisplay.getPosition().x + queueDisplay.getSize().x + 50.f,
                shipIcons[index].position.y
            );
            shipIcons[index].shape.setFillColor(sf::Color(100, 255, 100));
        }
    }

    void update(float deltaTime)
    {
        animPhase += deltaTime * 2.f;
        if (animPhase > 6.28318f) animPhase -= 6.28318f;

        for (int i = 0; i < shipIconCount; i++)
        {
            if (shipIcons[i].status == "ENTERING" || shipIcons[i].status == "DEPARTING")
            {
                shipIcons[i].progress += deltaTime * 2.f;

                if (shipIcons[i].progress >= 1.f)
                {
                    shipIcons[i].progress = 1.f;
                    shipIcons[i].position = shipIcons[i].targetPosition;

                    if (shipIcons[i].status == "ENTERING")
                    {
                        shipIcons[i].status = "WAITING";
                        shipIcons[i].shape.setFillColor(sf::Color(255, 200, 0));
                    }
                    else if (shipIcons[i].status == "DEPARTING")
                    {
                        shipIcons[i].isActive = false;
                    }
                }
                else
                {
                    float t = shipIcons[i].progress;
                    sf::Vector2f start = shipIcons[i].position;
                    sf::Vector2f end = shipIcons[i].targetPosition;
                    shipIcons[i].position = start + (end - start) * t;
                }
            }

            if (shipIcons[i].status == "WAITING")
            {
                float bob = sin(animPhase + i * 0.5f) * 2.f;
                shipIcons[i].shape.setPosition(
                    shipIcons[i].position.x,
                    shipIcons[i].position.y + bob
                );
            }
            else
            {
                shipIcons[i].shape.setPosition(shipIcons[i].position);
            }
        }

        float glow = (sin(animPhase) + 1.f) / 2.f;
        headerBar.setFillColor(sf::Color(0, (sf::Uint8)(80 + glow * 40), (sf::Uint8)(130 + glow * 40), 220));
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background);
        window.draw(headerBar);
        window.draw(titleText);
        window.draw(queueDisplay);

        if (fontPtr == nullptr) 
        {
            return;
        }

        float displayX = queueDisplay.getPosition().x;
        float displayY = queueDisplay.getPosition().y;

        if (currentPortIndex == -1)
        {
            sf::Text emptyText("Run an algorithm to see dock queue", *fontPtr, 11);
            emptyText.setFillColor(sf::Color(150, 150, 170));
            emptyText.setStyle(sf::Text::Italic);
            emptyText.setPosition(displayX + 40.f, displayY + 50.f);
            window.draw(emptyText);

            sf::Text hintText("(Dijkstra or A* buttons)", *fontPtr, 10);
            hintText.setFillColor(sf::Color(100, 100, 120));
            hintText.setPosition(displayX + 70.f, displayY + 75.f);
            window.draw(hintText);

            return;
        }

        sf::Text dockLabel("DOCK SLOTS:", *fontPtr, 10);
        dockLabel.setFillColor(sf::Color(0, 200, 255));
        dockLabel.setPosition(displayX + queueDisplay.getSize().x - 100.f, displayY + 5.f);
        window.draw(dockLabel);

        int maxSlots = portDockStatus[currentPortIndex].maxSlots;
        if (maxSlots <= 0) 
        {
            maxSlots = 2;
        }


        for (int i = 0; i < maxSlots && i < 4; i++)
        {
            float slotX = displayX + queueDisplay.getSize().x - 90.f + i * 22.f;
            float slotY = displayY + 22.f;

            sf::RectangleShape slot(sf::Vector2f(18.f, 18.f));
            slot.setPosition(slotX, slotY);
            slot.setOutlineThickness(1.f);

            if (i < portDockStatus[currentPortIndex].shipsDocking)
            {
                slot.setFillColor(sf::Color(0, 180, 100, 200));
                slot.setOutlineColor(sf::Color(0, 255, 150));
            }
            else
            {
                slot.setFillColor(sf::Color(40, 40, 60));
                slot.setOutlineColor(sf::Color(80, 80, 100));
            }
            window.draw(slot);
        }

        sf::Text queueLabel("QUEUE:", *fontPtr, 10);
        queueLabel.setFillColor(sf::Color(255, 200, 100));
        queueLabel.setPosition(displayX + 10.f, displayY + 5.f);
        window.draw(queueLabel);

        float lineY = displayY + 35.f;
        for (float x = displayX + 15.f; x < displayX + queueDisplay.getSize().x - 120.f; x += 12.f)
        {
            sf::RectangleShape dash(sf::Vector2f(8.f, 2.f));
            dash.setPosition(x, lineY);
            dash.setFillColor(sf::Color(100, 150, 200, 150));
            window.draw(dash);
        }

        for (int i = 0; i < shipIconCount; i++)
        {
            if (!shipIcons[i].isActive) 
            {
                continue;
            }

            window.draw(shipIcons[i].shape);

            sf::Text nameLabel(shipIcons[i].name, *fontPtr, 8);
            nameLabel.setFillColor(sf::Color::White);
            nameLabel.setPosition(
                shipIcons[i].position.x - 10.f,
                shipIcons[i].position.y + 10.f
            );
            window.draw(nameLabel);
        }

        int shipsInQueue = portDockStatus[currentPortIndex].shipsInQueue;
        int shipsDocking = portDockStatus[currentPortIndex].shipsDocking;
        int waiting = shipsInQueue - shipsDocking;
        if (waiting < 0) waiting = 0;

        string statusStr = "Ships: " + intToString(shipsInQueue) +
            " | Docking: " + intToString(shipsDocking) +
            " | Waiting: " + intToString(waiting);

        sf::Text statusText(statusStr, *fontPtr, 10);
        statusText.setFillColor(sf::Color(150, 200, 255));
        statusText.setPosition(displayX + 10.f, displayY + queueDisplay.getSize().y - 20.f);
        window.draw(statusText);

        float arrowX = displayX + queueDisplay.getSize().x - 115.f;
        float arrowY = displayY + 35.f;

        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, sf::Vector2f(10.f, 0.f));
        arrow.setPoint(1, sf::Vector2f(0.f, -6.f));
        arrow.setPoint(2, sf::Vector2f(0.f, 6.f));
        arrow.setPosition(arrowX, arrowY);

        float arrowPulse = (sin(animPhase * 3.f) + 1.f) / 2.f;
        arrow.setFillColor(sf::Color(0, (sf::Uint8)(200 + arrowPulse * 55), (sf::Uint8)(100 + arrowPulse * 55)));
        window.draw(arrow);
    }
};

DockQueuePanel dockQueuePanel;
struct SFMLPreferences
{
    string preferredCompanies[10];
    int preferredCompanyCount;
    string avoidedPorts[10];
    int avoidedPortCount;
    int totalVoyageTime;
    bool usePreferredCompanies;
    int maxTotalTime;
    bool useAvoidedPorts;
    bool useTotalTime;

    SFMLPreferences()
    {
        preferredCompanyCount = 0;
        avoidedPortCount = 0;
        maxTotalTime = 0;
        totalVoyageTime = 0;
        usePreferredCompanies = false;
        useAvoidedPorts = false;
        useTotalTime = false;
    }

    void clear()
    {
        preferredCompanyCount = 0;
        avoidedPortCount = 0;
        totalVoyageTime = 0;
        usePreferredCompanies = false;
        useAvoidedPorts = false;
        useTotalTime = false;
    }

    void addPreferredCompany(const string& company)
    {
        if (preferredCompanyCount < 10)
        {
            preferredCompanies[preferredCompanyCount] = company;
            preferredCompanyCount++;
            usePreferredCompanies = true;
        }
    }

    void addAvoidedPort(const string& port)
    {
        if (avoidedPortCount < 10)
        {
            avoidedPorts[avoidedPortCount] = port;
            avoidedPortCount++;
            useAvoidedPorts = true;
        }
    }

    void setTotalTime(int minutes)
    {
        totalVoyageTime = minutes;
        useTotalTime = (minutes > 0);
    }

    bool isCompanyPreferred(const string& company)
    {
        if (!usePreferredCompanies) 
        {
            return true;
        }
        for (int i = 0; i < preferredCompanyCount; i++)
        {
            if (preferredCompanies[i] == company) 
            {
                return true;
            }
        }
        return false;
    }

    bool isPortAvoided(const string& port)
    {
        if (!useAvoidedPorts) 
        {
            return false;
        }
        for (int i = 0; i < avoidedPortCount; i++)
        {
            if (avoidedPorts[i] == port) 
            {
                return true;
            }
        }
        return false;
    }

    void setMaxTime(int minutes)
    {
        maxTotalTime = minutes;
        useTotalTime = (minutes > 0);
    }
    int getMaxTime()
    {
        return maxTotalTime;
    }

    bool isPortAvoidedByIndex(int portIndex)
    {
        if (!useAvoidedPorts) 
        {
            return false;
        }
        if (portIndex < 0 || portIndex >= portCount) 
        {
            return false;
        }
        return isPortAvoided(ports[portIndex].name);
    }
};

struct LegNode
{
    int portIndex;             
    string portName;           
    int routeToNextIndex;
    LegNode* next;              
    LegNode* prev;             

    string date;
    string depTime;
    string arrTime;
    int cost;
    string company;

    LegNode()
    {
        portIndex = -1;
        portName = "";
        routeToNextIndex = -1;
        next = nullptr;
        prev = nullptr;
        date = "";
        depTime = "";
        arrTime = "";
        cost = 0;
        company = "";
    }

    LegNode(int pIndex, const string& pName)
    {
        portIndex = pIndex;
        portName = pName;
        routeToNextIndex = -1;
        next = nullptr;
        prev = nullptr;
        date = "";
        depTime = "";
        arrTime = "";
        cost = 0;
        company = "";
    }
};

int dateToInt(const string& date)
{
    if (date.length() < 10) 
    {
        return 0;
    }

    int day = 0, month = 0, year = 0;
    int slashCount = 0;
    string part = "";

    for (int i = 0; i <= date.length(); i++)
    {
        if (i == date.length() || date[i] == '/')
        {
            int val = 0;
            for (int j = 0; j < part.length(); j++)
            {
                val = val * 10 + (part[j] - '0');
            }

            if (slashCount == 0) 
            {
                day = val;
            }
            else if (slashCount == 1) 
            {
                month = val;
            }
            else if (slashCount == 2) 
            {
                year = val;
            }

            slashCount++;
            part = "";
        }
        else
        {
            part += date[i];
        }
    }
    return year * 10000 + month * 100 + day;
}

int timeToMinutes(const string& time)
{
    if (time.length() < 5) 
    {
        return 0;
    }

    int hours = 0, minutes = 0;
    bool pastColon = false;
    string part = "";

    for (int i = 0; i <= time.length(); i++)
    {
        if (i == time.length() || time[i] == ':')
        {
            int val = 0;
            for (int j = 0; j < part.length(); j++)
            {
                val = val * 10 + (part[j] - '0');
            }

            if (!pastColon) 
            {
                hours = val;
            }
            else
            {
                minutes = val;
            }

            pastColon = true;
            part = "";
        }
        else
        {
            part += time[i];
        }
    }

    return hours * 60 + minutes;
}

bool isDateSameOrAfter(const string& date1, const string& date2)
{
    return dateToInt(date2) >= dateToInt(date1);
}

bool isLegSequenceFeasible(const string& arrDate1, const string& arrTime1, const string& depDate2, const string& depTime2)
{
    int date1Int = dateToInt(arrDate1);
    int date2Int = dateToInt(depDate2);

    if (date2Int > date1Int)
    {
        return true;
    }

    if (date2Int == date1Int)
    {
        int arrMinutes = timeToMinutes(arrTime1);
        int depMinutes = timeToMinutes(depTime2);

        return depMinutes >= arrMinutes + 30;
    }
    return false;
}

class MultiLegRoute
{
private:
    LegNode* head;
    LegNode* tail;
    int legCount;
    int totalCost;
    int totalTimeMinutes;

public:
    MultiLegRoute()
    {
        head = nullptr;
        tail = nullptr;
        legCount = 0;
        totalCost = 0;
        totalTimeMinutes = 0;
    }

    ~MultiLegRoute()
    {
        clear();
    }

    void clear()
    {
        LegNode* current = head;
        while (current != nullptr)
        {
            LegNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        legCount = 0;
        totalCost = 0;
        totalTimeMinutes = 0;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    int getCount() const
    {
        return legCount;
    }

    int getTotalCost() const
    {
        return totalCost;
    }

    LegNode* getHead() const
    {
        return head;
    }

    LegNode* getTail() const
    {
        return tail;
    }

    bool addPort(int portIndex, const string& portName)
    {
        LegNode* newNode = new LegNode(portIndex, portName);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        legCount++;
        return true;
    }

    bool addPortAtPosition(LegNode* afterNode, int portIndex, const string& portName)
    {
        if (afterNode == nullptr)
        {
            return false;
        }

        LegNode* newNode = new LegNode(portIndex, portName);

        newNode->next = afterNode->next;
        newNode->prev = afterNode;

        if (afterNode->next != nullptr)
        {
            afterNode->next->prev = newNode;
        }
        else
        {
            tail = newNode;
        }

        afterNode->next = newNode;
        legCount++;

        afterNode->routeToNextIndex = -1;
        afterNode->date = "";
        afterNode->depTime = "";
        afterNode->arrTime = "";
        afterNode->cost = 0;
        afterNode->company = "";
        return true;
    }

    bool addPortAtPosition(int position, int portIndex, const string& portName)
    {
        if (position < 0 || position > legCount)
        {
            return false;
        }

        if (position == 0)
        {
            LegNode* newNode = new LegNode(portIndex, portName);
            newNode->next = head;
            if (head != nullptr)
            {
                head->prev = newNode;
            }
            head = newNode;
            if (tail == nullptr)
            {
                tail = newNode;
            }
            legCount++;
            return true;
        }

        if (position == legCount)
        {
            return addPort(portIndex, portName);
        }
        LegNode* current = head;
        for (int i = 0; i < position - 1; i++)
        {
            current = current->next;
        }

        return addPortAtPosition(current, portIndex, portName);
    }

    bool removePortAtPosition(int position)
    {
        if (position < 0 || position >= legCount || head == nullptr)
        {
            return false;
        }

        LegNode* toRemove = head;
        for (int i = 0; i < position; i++)
        {
            toRemove = toRemove->next;
        }

        if (toRemove->prev != nullptr)
        {
            toRemove->prev->next = toRemove->next;
            toRemove->prev->routeToNextIndex = -1;
            toRemove->prev->date = "";
            toRemove->prev->depTime = "";
            toRemove->prev->arrTime = "";
            toRemove->prev->cost = 0;
            toRemove->prev->company = "";
        }
        else
        {
            head = toRemove->next;
        }

        if (toRemove->next != nullptr)
        {
            toRemove->next->prev = toRemove->prev;
        }
        else
        {
            tail = toRemove->prev;
        }

        delete toRemove;
        legCount--;

        recalculateTotals();
        return true;
    }

    bool removeNode(LegNode* node)
    {
        if (node == nullptr)
        {
            return false;
        }

        if (node->prev != nullptr)
        {
            node->prev->next = node->next;
            node->prev->routeToNextIndex = -1;
        }
        else
        {
            head = node->next;
        }

        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
        }
        else
        {
            tail = node->prev;
        }

        delete node;
        legCount--;
        recalculateTotals();
        return true;
    }

    LegNode* getNodeAtPosition(int position)
    {
        if (position < 0 || position >= legCount)
        {
            return nullptr;
        }

        LegNode* current = head;
        for (int i = 0; i < position; i++)
        {
            current = current->next;
        }
        return current;
    }

    bool setRouteDetails(int position, int routeIndex, const string& date, const string& depTime, const string& arrTime, int cost, const string& company)
    {
        LegNode* node = getNodeAtPosition(position);
        if (node == nullptr || node->next == nullptr)
        {
            return false;
        }

        node->routeToNextIndex = routeIndex;
        node->date = date;
        node->depTime = depTime;
        node->arrTime = arrTime;
        node->cost = cost;
        node->company = company;

        recalculateTotals();
        return true;
    }

    int findRouteBetween(int fromPortIndex, int toPortIndex, const string& preferredDate = "")
    {
        for (int i = 0; i < routeCount; i++)
        {
            if (!routes[i].valid)
            {
                continue;
            }
            if (routes[i].originIndex == fromPortIndex && routes[i].destinationIndex == toPortIndex)
            {
                if (!sfmlPreferences.isCompanyPreferred(routes[i].company))
                {
                    continue;
                }

                if (!preferredDate.empty() && routes[i].date != preferredDate)
                {
                    continue;
                }
                return i;
            }
        }
        return -1;
    }

    bool autoFindRoutes()
    {
        if (legCount < 2)
        {
            return false;
        }

        LegNode* current = head;
        bool allFound = true;

        while (current != nullptr && current->next != nullptr)
        {
            int bestRouteIndex = -1;
            int bestCost = 999999999;

            for (int i = 0; i < routeCount; i++)
            {
                if (!routes[i].valid)
                {
                    continue;
                }

                if (routes[i].originIndex != current->portIndex || routes[i].destinationIndex != current->next->portIndex)
                {
                    continue;
                }

                if (sfmlPreferences.usePreferredCompanies)
                {
                    if (!sfmlPreferences.isCompanyPreferred(routes[i].company))
                    {
                        continue;
                    }
                }

                if (bestRouteIndex == -1 || routes[i].vCost < bestCost)
                {
                    bestRouteIndex = i;
                    bestCost = routes[i].vCost;
                }
            }

            if (bestRouteIndex != -1)
            {
                current->routeToNextIndex = bestRouteIndex;
                current->date = routes[bestRouteIndex].date;
                current->depTime = routes[bestRouteIndex].depTime;
                current->arrTime = routes[bestRouteIndex].arrTime;
                current->cost = routes[bestRouteIndex].vCost;
                current->company = routes[bestRouteIndex].company;
            }
            else
            {
                current->routeToNextIndex = -1;
                current->date = "";
                current->depTime = "";
                current->arrTime = "";
                current->cost = 0;
                current->company = "";
                allFound = false;
            }

            current = current->next;
        }

        recalculateTotals();
        return allFound;
    }

    void recalculateTotals()
    {
        totalCost = 0;
        totalTimeMinutes = 0;

        LegNode* current = head;
        while (current != nullptr)
        {
            if (current->routeToNextIndex != -1)
            {
                totalCost += current->cost;
            }
            current = current->next;
        }
    }

    bool isComplete() const
    {
        if (legCount < 2)
        {
            return false;
        }

        LegNode* current = head;
        while (current != nullptr && current->next != nullptr)
        {
            if (current->routeToNextIndex == -1)
            {
                return false;
            }

            if (current->prev != nullptr && current->prev->routeToNextIndex != -1)
            {
                if (!isLegSequenceFeasible(current->prev->date, current->prev->arrTime, current->date, current->depTime))
                {
                    return false;
                }
            }

            current = current->next;
        }
        return true;
    }


    void printRoute() const
    {
        cout << "Total Stops: " << legCount << endl;
        cout << "Total Cost: $" << totalCost << endl;
        cout << "Route Complete: " << (isComplete() ? "Yes" : "No") << endl;
        cout << "\nRoute Chain:" << endl;

        LegNode* current = head;
        int legNum = 1;

        while (current != nullptr)
        {
            cout << "  [" << legNum << "] " << current->portName;

            if (current->next != nullptr)
            {
                cout << endl;
                if (current->routeToNextIndex != -1)
                {
                    cout << "      --> " << current->date << " | "
                        << current->depTime << " - " << current->arrTime
                        << " | $" << current->cost << " | " << current->company;
                }
                else
                {
                    cout << "      --> (No route found)";
                }
                cout << endl;
            }

            current = current->next;
            legNum++;
        }
    }
};

MultiLegRoute multiLegRoute;
SFMLPreferences sfmlPreferences;

string intToString(int val) {
    if (val == 0) return "0";
    string result;
    bool neg = val < 0;
    if (neg) val = -val;
    while (val > 0) {
        result = char('0' + (val % 10)) + result;
        val /= 10;
    }
    if (neg) result = "-" + result;
    return result;
}

class ShipAnimation 
{
private:
    sf::ConvexShape shipShape;
    sf::CircleShape shipGlow;
    sf::CircleShape wakeParticles[20];

    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    float rotation;

    sf::Vector2f pathPoints[100];
    int totalPathPoints;
    int currentPathIndex;
    float progressBetweenPoints;

    bool isAnimating;
    bool isPaused;
    float speed;
    float baseSpeed;

    float bobPhase;
    float glowPhase;
    float wakePhase;

    sf::Vector2f trailPositions[50];
    int trailCount;
    float trailTimer;

    sf::Font* fontPtr;

    string currentFromPort;
    string currentToPort;
    int currentLegIndex;
    int totalLegs;

public:
    ShipAnimation() 
    {
        isAnimating = false;
        isPaused = false;
        speed = 50.f;
        baseSpeed = 50.f;
        currentPathIndex = 0;
        totalPathPoints = 0;
        progressBetweenPoints = 0.f;
        bobPhase = 0.f;
        glowPhase = 0.f;
        wakePhase = 0.f;
        rotation = 0.f;
        trailCount = 0;
        trailTimer = 0.f;
        fontPtr = nullptr;
        currentLegIndex = 0;
        totalLegs = 0;
    }

    void initialize(sf::Font& font) 
    {
        fontPtr = &font;

        shipShape.setPointCount(7);
        shipShape.setPoint(0, sf::Vector2f(20.f, 0.f));    
        shipShape.setPoint(1, sf::Vector2f(12.f, -8.f));   
        shipShape.setPoint(2, sf::Vector2f(-15.f, -8.f));  
        shipShape.setPoint(3, sf::Vector2f(-20.f, -5.f));  
        shipShape.setPoint(4, sf::Vector2f(-20.f, 5.f));   
        shipShape.setPoint(5, sf::Vector2f(-15.f, 8.f));   
        shipShape.setPoint(6, sf::Vector2f(12.f, 8.f));    

        shipShape.setFillColor(sf::Color(50, 60, 80));
        shipShape.setOutlineThickness(2.f);
        shipShape.setOutlineColor(sf::Color(200, 220, 255));
        shipShape.setOrigin(0.f, 0.f);

        shipGlow.setRadius(25.f);
        shipGlow.setOrigin(25.f, 25.f);
        shipGlow.setFillColor(sf::Color(0, 150, 255, 60));

        for (int i = 0; i < 20; i++) 
        {
            wakeParticles[i].setRadius(3.f + (rand() % 3));
            wakeParticles[i].setOrigin(3.f, 3.f);
            wakeParticles[i].setFillColor(sf::Color(150, 200, 255, 0));
        }

        for (int i = 0; i < 50; i++) 
        {
            trailPositions[i] = sf::Vector2f(0, 0);
        }
    }

    void startAnimation(int legCount) 
    {
        if (legCount <= 0) 
        {
            return;
        }

        totalPathPoints = 0;
        currentLegIndex = 0;
        totalLegs = legCount;

        if (algorithmLegCount > 0) 
        {
            for (int leg = 0; leg < algorithmLegCount; leg++) 
            {
                int fromIdx = algorithmPart[leg].fromPortIndex;
                int toIdx = algorithmPart[leg].toPortIndex;

                sf::Vector2f start = ports[fromIdx].pixel;
                sf::Vector2f end = ports[toIdx].pixel;

                sf::Vector2f legPoints[totalWayPoints];
                int legPointCount = 0;
                generateOceanPath(start, end, fromIdx, toIdx, legPoints, legPointCount);
                int startIndex = (leg == 0) ? 0 : 1;
                for (int i = startIndex; i < legPointCount && totalPathPoints < 100; i++) 
                {
                    pathPoints[totalPathPoints++] = legPoints[i];
                }
            }
            currentFromPort = ports[algorithmPart[0].fromPortIndex].name;
            currentToPort = ports[algorithmPart[0].toPortIndex].name;
        }
        else if (visualLegCount > 0) 
        {
            for (int leg = 0; leg < visualLegCount; leg++) 
            {
                const Routes& r = routes[selectedItinerary[leg].routeIndex];

                sf::Vector2f start = ports[r.originIndex].pixel;
                sf::Vector2f end = ports[r.destinationIndex].pixel;

                sf::Vector2f legPoints[totalWayPoints];
                int legPointCount = 0;
                generateOceanPath(start, end, r.originIndex, r.destinationIndex, legPoints, legPointCount);

                int startIdx = (leg == 0) ? 0 : 1;
                for (int i = startIdx; i < legPointCount && totalPathPoints < 100; i++) 
                {
                    pathPoints[totalPathPoints++] = legPoints[i];
                }
            }
            const Routes& firstRoute = routes[selectedItinerary[0].routeIndex];
            currentFromPort = ports[firstRoute.originIndex].name;
            currentToPort = ports[firstRoute.destinationIndex].name;
        }

        if (totalPathPoints < 2) 
        {
            return;
        }

        currentPos = pathPoints[0];
        currentPathIndex = 0;
        progressBetweenPoints = 0.f;
        isAnimating = true;
        isPaused = false;
        trailCount = 0;

        if (totalPathPoints > 1) 
        {
            sf::Vector2f dir = pathPoints[1] - pathPoints[0];
            rotation = atan2(dir.y, dir.x) * 180.f / 3.14159f;
        }
    }

    void stopAnimation() 
    {
        isAnimating = false;
        isPaused = false;
    }

    void togglePause() 
    {
        if (isAnimating) {
            isPaused = !isPaused;
        }
    }

    void setSpeed(float multiplier) 
    {
        speed = baseSpeed * multiplier;
    }

    bool isRunning() const 
    {
        return isAnimating && !isPaused;
    }

    bool isActive() const 
    {
        return isAnimating;
    }

    void update(float deltaTime) 
    {
        if (!isAnimating) 
        {
            return;
        }

        bobPhase += deltaTime * 4.f;
        glowPhase += deltaTime * 3.f;
        wakePhase += deltaTime * 8.f;

        if (bobPhase > 6.28318f) bobPhase -= 6.28318f;
        if (glowPhase > 6.28318f) glowPhase -= 6.28318f;
        if (wakePhase > 6.28318f) wakePhase -= 6.28318f;

        if (isPaused) 
        {
            return;
        }
\
        if (currentPathIndex < totalPathPoints - 1) 
        {
            sf::Vector2f start = pathPoints[currentPathIndex];
            sf::Vector2f end = pathPoints[currentPathIndex + 1];
            sf::Vector2f segment = end - start;
            float segmentLength = sqrt(segment.x * segment.x + segment.y * segment.y);

            if (segmentLength > 0) 
            {
                float moveAmount = speed * deltaTime;
                progressBetweenPoints += moveAmount / segmentLength;

                updateCurrentLegInfo();

                if (progressBetweenPoints >= 1.f) 
                {
                    progressBetweenPoints = 0.f;
                    currentPathIndex++;

                    if (currentPathIndex >= totalPathPoints - 1) 
                    {
                        currentPos = pathPoints[totalPathPoints - 1];
                        isAnimating = false;
                        return;
                    }

                    start = pathPoints[currentPathIndex];
                    end = pathPoints[currentPathIndex + 1];
                }

                currentPos = start + (end - start) * progressBetweenPoints;

                sf::Vector2f dir = end - start;
                float targetRotation = atan2(dir.y, dir.x) * 180.f / 3.14159f;
                float rotDiff = targetRotation - rotation;
                while (rotDiff > 180.f) rotDiff -= 360.f;
                while (rotDiff < -180.f) rotDiff += 360.f;
                rotation += rotDiff * deltaTime * 3.f;
            }
        }

        trailTimer += deltaTime;
        if (trailTimer > 0.05f)
        {
            trailTimer = 0.f;
            for (int i = 49; i > 0; i--) 
            {
                trailPositions[i] = trailPositions[i - 1];
            }
            trailPositions[0] = currentPos;

            if (trailCount < 50) trailCount++;
        }
    }

    void updateCurrentLegInfo() 
    {
        if (algorithmLegCount > 0)
        {
            int pointsPerLeg = totalPathPoints / algorithmLegCount;
            if (pointsPerLeg > 0) {
                int newLegIndex = currentPathIndex / pointsPerLeg;
                if (newLegIndex >= algorithmLegCount) newLegIndex = algorithmLegCount - 1;

                if (newLegIndex != currentLegIndex) {
                    currentLegIndex = newLegIndex;
                    currentFromPort = ports[algorithmPart[currentLegIndex].fromPortIndex].name;
                    currentToPort = ports[algorithmPart[currentLegIndex].toPortIndex].name;
                }
            }
        }
        else if (visualLegCount > 0) 
        {
            int pointsPerLeg = totalPathPoints / visualLegCount;
            if (pointsPerLeg > 0) 
            {
                int newLegIndex = currentPathIndex / pointsPerLeg;
                if (newLegIndex >= visualLegCount) 
                {
                    newLegIndex = visualLegCount - 1;
                }

                if (newLegIndex != currentLegIndex) 
                {
                    currentLegIndex = newLegIndex;
                    const Routes& r = routes[selectedItinerary[currentLegIndex].routeIndex];
                    currentFromPort = ports[r.originIndex].name;
                    currentToPort = ports[r.destinationIndex].name;
                }
            }
        }
    }

    void draw(sf::RenderWindow& window, sf::View& mapView) 
    {
        {
            if (!isAnimating && trailCount == 0) 
            {
                return;
            }
        }

        window.setView(mapView);

        for (int i = 0; i < trailCount; i++) 
        {
            float alpha = (1.f - (float)i / trailCount) * 120.f;
            float radius = 4.f * (1.f - (float)i / trailCount);

            sf::CircleShape trail(radius);
            trail.setOrigin(radius, radius);
            trail.setPosition(trailPositions[i]);
            trail.setFillColor(sf::Color(150, 200, 255, (sf::Uint8)alpha));
            window.draw(trail);
        }

        if (!isAnimating) 
        {
            return;
        }
        float glowPulse = (sin(glowPhase) + 1.f) / 2.f;
        float glowRadius = 25.f + glowPulse * 8.f;
        shipGlow.setRadius(glowRadius);
        shipGlow.setOrigin(glowRadius, glowRadius);
        shipGlow.setPosition(currentPos);
        shipGlow.setFillColor(sf::Color(0, 150, 255, (sf::Uint8)(40 + glowPulse * 40)));
        window.draw(shipGlow);

        if (!isPaused) 
        {
            for (int i = 0; i < 10; i++) 
            {
                float angle = rotation * 3.14159f / 180.f + 3.14159f; 
                float spread = (float)(i - 5) * 0.15f;
                float dist = 25.f + sin(wakePhase + i * 0.5f) * 5.f + i * 3.f;

                sf::Vector2f wakePos = currentPos;
                wakePos.x -= cos(angle + spread) * dist;
                wakePos.y -= sin(angle + spread) * dist;

                float alpha = 100.f * (1.f - (float)i / 10.f);
                wakeParticles[i].setPosition(wakePos);
                wakeParticles[i].setFillColor(sf::Color(200, 230, 255, (sf::Uint8)alpha));
                window.draw(wakeParticles[i]);
            }
        }

        float bobOffset = sin(bobPhase) * 2.f;

        shipShape.setPosition(currentPos.x, currentPos.y + bobOffset);
        shipShape.setRotation(rotation);

        if (isPaused) 
        {
            shipShape.setFillColor(sf::Color(80, 80, 40));
            shipShape.setOutlineColor(sf::Color(255, 255, 100));
        }
        else 
        {
            shipShape.setFillColor(sf::Color(50, 60, 80));
            shipShape.setOutlineColor(sf::Color(200, 220, 255));
        }

        window.draw(shipShape);
        sf::RectangleShape cabin(sf::Vector2f(8.f, 10.f));
        cabin.setOrigin(4.f, 5.f);
        cabin.setFillColor(sf::Color(80, 90, 110));
        cabin.setOutlineThickness(1.f);
        cabin.setOutlineColor(sf::Color(150, 170, 200));

        float cabinAngle = rotation * 3.14159f / 180.f;
        sf::Vector2f cabinOffset(-5.f, 0.f);
        sf::Vector2f rotatedOffset(
            cabinOffset.x * cos(cabinAngle) - cabinOffset.y * sin(cabinAngle),
            cabinOffset.x * sin(cabinAngle) + cabinOffset.y * cos(cabinAngle)
        );
        cabin.setPosition(currentPos + rotatedOffset + sf::Vector2f(0, bobOffset));
        cabin.setRotation(rotation);
        window.draw(cabin);

        sf::RectangleShape smokestack(sf::Vector2f(4.f, 6.f));
        smokestack.setOrigin(2.f, 3.f);
        smokestack.setFillColor(sf::Color(60, 60, 60));
        smokestack.setPosition(currentPos + rotatedOffset + sf::Vector2f(0, bobOffset - 2.f));
        smokestack.setRotation(rotation);
        window.draw(smokestack);
    }

    void drawUI(sf::RenderWindow& window, float panelWidth) 
    {
        if (!isAnimating || !fontPtr) 
        {
            return;
        }

        window.setView(window.getDefaultView());
        float panelHeight = 60.f;
        float panelY = window.getSize().y - panelHeight - 10.f;
        float panelX = panelWidth + 20.f;
        float panelW = window.getSize().x - panelWidth - 40.f;

        sf::RectangleShape statusBg(sf::Vector2f(panelW, panelHeight));
        statusBg.setPosition(panelX, panelY);
        statusBg.setFillColor(sf::Color(10, 20, 40, 230));
        statusBg.setOutlineThickness(2.f);
        statusBg.setOutlineColor(sf::Color(0, 200, 255, 180));
        window.draw(statusBg);

        sf::Text shipIcon(L"🚢", *fontPtr, 24);
        shipIcon.setPosition(panelX + 15.f, panelY + 15.f);

        sf::Text shipLabel("Ship Enroute", *fontPtr, 12);
        shipLabel.setFillColor(sf::Color(0, 200, 255));
        shipLabel.setStyle(sf::Text::Bold);
        shipLabel.setPosition(panelX + 15.f, panelY + 8.f);
        window.draw(shipLabel);

        sf::Text legInfo("Leg " + intToString(currentLegIndex + 1) + "/" + intToString(totalLegs) +
            ": " + currentFromPort + " -> " + currentToPort, *fontPtr, 14);
        legInfo.setFillColor(sf::Color::White);
        legInfo.setPosition(panelX + 15.f, panelY + 25.f);
        window.draw(legInfo);

        float progressBarWidth = panelW - 180.f;
        float progress = (float)currentPathIndex / (float)(totalPathPoints - 1);

        sf::RectangleShape progressBg(sf::Vector2f(progressBarWidth, 10.f));
        progressBg.setPosition(panelX + 150.f, panelY + 42.f);
        progressBg.setFillColor(sf::Color(30, 40, 60));
        progressBg.setOutlineThickness(1.f);
        progressBg.setOutlineColor(sf::Color(60, 80, 100));
        window.draw(progressBg);

        sf::RectangleShape progressFill(sf::Vector2f(progressBarWidth * progress, 10.f));
        progressFill.setPosition(panelX + 150.f, panelY + 42.f);
        progressFill.setFillColor(sf::Color(0, 200, 255));
        window.draw(progressFill);

        sf::Text percentText(intToString((int)(progress * 100)) + "%", *fontPtr, 11);
        percentText.setFillColor(sf::Color(200, 220, 255));
        percentText.setPosition(panelX + 155.f + progressBarWidth, panelY + 40.f);
        window.draw(percentText);

        sf::Text controls("SPACE = Pause | +/- = Speed | S = Stop", *fontPtr, 10);
        controls.setFillColor(sf::Color(120, 140, 160));
        controls.setPosition(panelX + 15.f, panelY + 43.f);
        window.draw(controls);

        float speedMultiplier = speed / baseSpeed;
        sf::Text speedText("Speed: " + intToString((int)(speedMultiplier * 100)) + "%", *fontPtr, 10);
        speedText.setFillColor(isPaused ? sf::Color(255, 255, 100) : sf::Color(100, 255, 100));
        speedText.setPosition(panelX + panelW - 80.f, panelY + 8.f);
        window.draw(speedText);

        if (isPaused) 
        {
            sf::Text pausedText("Paused", *fontPtr, 14);
            pausedText.setFillColor(sf::Color(255, 255, 100));
            pausedText.setStyle(sf::Text::Bold);
            pausedText.setPosition(panelX + panelW - 80.f, panelY + 25.f);
            window.draw(pausedText);
        }
    }
};

ShipAnimation shipAnimation;

class PreferencesPanel
{
private:
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::Text titleText;
    sf::Text companyLabel, avoidPortLabel, maxTimeLabel;
    sf::RectangleShape companyInputBox, avoidPortInputBox, maxTimeInputBox;
    sf::Text companyInputText, avoidPortInputText, maxTimeInputText;
    string companyInputStr, avoidPortInputStr, maxTimeInputStr;
    sf::RectangleShape addCompanyBtn, addAvoidPortBtn, setMaxTimeBtn;
    sf::Text addCompanyBtnText, addAvoidPortBtnText, setMaxTimeBtnText;
    sf::RectangleShape clearAllBtn, closeBtn;
    sf::Text clearAllBtnText, closeBtnText;
    sf::RectangleShape listsPanel;
    int activeInput;
    bool isVisible;
    float panelX, panelY, panelWidth, panelHeight;
    sf::Font* fontPtr;
    float animPhase;

public:
    PreferencesPanel()
    {
        activeInput = 0;
        isVisible = false;
        fontPtr = nullptr;
        animPhase = 0.f;
        panelX = panelY = panelWidth = panelHeight = 0;
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;
        panelWidth = 500.f;
        panelHeight = 520.f;
        panelX = (windowWidth - panelWidth) / 2.f;
        panelY = (windowHeight - panelHeight) / 2.f;

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(12, 20, 40, 250));
        background.setOutlineThickness(3.f);
        background.setOutlineColor(sf::Color(255, 180, 0));

        headerBar.setSize(sf::Vector2f(panelWidth, 50.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(255, 140, 0, 200));

        titleText.setFont(font);
        titleText.setString("Ship Preferences");
        titleText.setCharacterSize(22);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(panelX + 20.f, panelY + 12.f);

        float yOffset = 70.f;
        float inputWidth = panelWidth - 140.f;
        float inputHeight = 32.f;
        float btnWidth = 80.f;

        companyLabel.setFont(font);
        companyLabel.setString("Preferred Company:");
        companyLabel.setCharacterSize(14);
        companyLabel.setFillColor(sf::Color(255, 200, 100));
        companyLabel.setPosition(panelX + 20.f, panelY + yOffset);

        companyInputBox.setSize(sf::Vector2f(inputWidth - btnWidth - 10.f, inputHeight));
        companyInputBox.setPosition(panelX + 20.f, panelY + yOffset + 22.f);
        companyInputBox.setFillColor(sf::Color(20, 30, 50));
        companyInputBox.setOutlineThickness(2.f);
        companyInputBox.setOutlineColor(sf::Color(80, 100, 140));

        companyInputText.setFont(font);
        companyInputText.setCharacterSize(14);
        companyInputText.setFillColor(sf::Color(200, 220, 255));
        companyInputText.setPosition(panelX + 28.f, panelY + yOffset + 28.f);

        addCompanyBtn.setSize(sf::Vector2f(btnWidth, inputHeight));
        addCompanyBtn.setPosition(panelX + panelWidth - btnWidth - 30.f, panelY + yOffset + 22.f);
        addCompanyBtn.setFillColor(sf::Color(50, 150, 50));
        addCompanyBtn.setOutlineThickness(2.f);
        addCompanyBtn.setOutlineColor(sf::Color(100, 200, 100));

        addCompanyBtnText.setFont(font);
        addCompanyBtnText.setString("Add");
        addCompanyBtnText.setCharacterSize(12);
        addCompanyBtnText.setFillColor(sf::Color::White);
        addCompanyBtnText.setPosition(panelX + panelWidth - btnWidth - 10.f, panelY + yOffset + 28.f);

        yOffset += 70.f;

        avoidPortLabel.setFont(font);
        avoidPortLabel.setString("Avoid Port:");
        avoidPortLabel.setCharacterSize(14);
        avoidPortLabel.setFillColor(sf::Color(255, 100, 100));
        avoidPortLabel.setPosition(panelX + 20.f, panelY + yOffset);

        avoidPortInputBox.setSize(sf::Vector2f(inputWidth - btnWidth - 10.f, inputHeight));
        avoidPortInputBox.setPosition(panelX + 20.f, panelY + yOffset + 22.f);
        avoidPortInputBox.setFillColor(sf::Color(20, 30, 50));
        avoidPortInputBox.setOutlineThickness(2.f);
        avoidPortInputBox.setOutlineColor(sf::Color(80, 100, 140));

        avoidPortInputText.setFont(font);
        avoidPortInputText.setCharacterSize(14);
        avoidPortInputText.setFillColor(sf::Color(200, 220, 255));
        avoidPortInputText.setPosition(panelX + 28.f, panelY + yOffset + 28.f);

        addAvoidPortBtn.setSize(sf::Vector2f(btnWidth, inputHeight));
        addAvoidPortBtn.setPosition(panelX + panelWidth - btnWidth - 30.f, panelY + yOffset + 22.f);
        addAvoidPortBtn.setFillColor(sf::Color(180, 50, 50));
        addAvoidPortBtn.setOutlineThickness(2.f);
        addAvoidPortBtn.setOutlineColor(sf::Color(255, 100, 100));

        addAvoidPortBtnText.setFont(font);
        addAvoidPortBtnText.setString("Add");
        addAvoidPortBtnText.setCharacterSize(12);
        addAvoidPortBtnText.setFillColor(sf::Color::White);
        addAvoidPortBtnText.setPosition(panelX + panelWidth - btnWidth - 10.f, panelY + yOffset + 28.f);

        yOffset += 70.f;

        maxTimeLabel.setFont(font);
        maxTimeLabel.setString("Max Voyage Time (minutes):");
        maxTimeLabel.setCharacterSize(14);
        maxTimeLabel.setFillColor(sf::Color(100, 200, 255));
        maxTimeLabel.setPosition(panelX + 20.f, panelY + yOffset);

        maxTimeInputBox.setSize(sf::Vector2f(inputWidth - btnWidth - 10.f, inputHeight));
        maxTimeInputBox.setPosition(panelX + 20.f, panelY + yOffset + 22.f);
        maxTimeInputBox.setFillColor(sf::Color(20, 30, 50));
        maxTimeInputBox.setOutlineThickness(2.f);
        maxTimeInputBox.setOutlineColor(sf::Color(80, 100, 140));

        maxTimeInputText.setFont(font);
        maxTimeInputText.setCharacterSize(14);
        maxTimeInputText.setFillColor(sf::Color(200, 220, 255));
        maxTimeInputText.setPosition(panelX + 28.f, panelY + yOffset + 28.f);

        setMaxTimeBtn.setSize(sf::Vector2f(btnWidth, inputHeight));
        setMaxTimeBtn.setPosition(panelX + panelWidth - btnWidth - 30.f, panelY + yOffset + 22.f);
        setMaxTimeBtn.setFillColor(sf::Color(50, 100, 180));
        setMaxTimeBtn.setOutlineThickness(2.f);
        setMaxTimeBtn.setOutlineColor(sf::Color(100, 150, 255));

        setMaxTimeBtnText.setFont(font);
        setMaxTimeBtnText.setString("Set");
        setMaxTimeBtnText.setCharacterSize(12);
        setMaxTimeBtnText.setFillColor(sf::Color::White);
        setMaxTimeBtnText.setPosition(panelX + panelWidth - btnWidth - 8.f, panelY + yOffset + 28.f);

        yOffset += 70.f;

        listsPanel.setSize(sf::Vector2f(panelWidth - 40.f, 160.f));
        listsPanel.setPosition(panelX + 20.f, panelY + yOffset);
        listsPanel.setFillColor(sf::Color(8, 15, 30, 200));
        listsPanel.setOutlineThickness(1.f);
        listsPanel.setOutlineColor(sf::Color(60, 80, 120));

        yOffset += 180.f;

        clearAllBtn.setSize(sf::Vector2f(120.f, 35.f));
        clearAllBtn.setPosition(panelX + 20.f, panelY + yOffset);
        clearAllBtn.setFillColor(sf::Color(120, 60, 60));
        clearAllBtn.setOutlineThickness(2.f);
        clearAllBtn.setOutlineColor(sf::Color(180, 100, 100));

        clearAllBtnText.setFont(font);
        clearAllBtnText.setString("Clear All");
        clearAllBtnText.setCharacterSize(12);
        clearAllBtnText.setFillColor(sf::Color::White);
        clearAllBtnText.setPosition(panelX + 35.f, panelY + yOffset + 8.f);

        closeBtn.setSize(sf::Vector2f(100.f, 35.f));
        closeBtn.setPosition(panelX + panelWidth - 120.f, panelY + yOffset);
        closeBtn.setFillColor(sf::Color(80, 80, 80));
        closeBtn.setOutlineThickness(2.f);
        closeBtn.setOutlineColor(sf::Color(150, 150, 150));

        closeBtnText.setFont(font);
        closeBtnText.setString("Close");
        closeBtnText.setCharacterSize(12);
        closeBtnText.setFillColor(sf::Color::White);
        closeBtnText.setPosition(panelX + panelWidth - 100.f, panelY + yOffset + 8.f);
    }

    void show() 
    { 
        isVisible = true; 
    }
    void hide() 
    { 
        isVisible = false; activeInput = 0; 
    }
    bool visible() const
    { 
        return isVisible; 
    }

    void update(float deltaTime)
    {
        animPhase += deltaTime * 3.f;
        if (animPhase > 6.28318f) animPhase -= 6.28318f;

        float glow = (sin(animPhase) + 1.f) / 2.f;
        headerBar.setFillColor(sf::Color(255, (sf::Uint8)(120 + glow * 40), (sf::Uint8)(glow * 50), 200));

        sf::Color activeOutline((sf::Uint8)(150 + glow * 105), (sf::Uint8)(200 + glow * 55), 255);
        sf::Color inactiveOutline(80, 100, 140);
        companyInputBox.setOutlineColor(activeInput == 1 ? activeOutline : inactiveOutline);
        avoidPortInputBox.setOutlineColor(activeInput == 2 ? activeOutline : inactiveOutline);
        maxTimeInputBox.setOutlineColor(activeInput == 3 ? activeOutline : inactiveOutline);
    }

    void handleTextInput(sf::Uint32 unicode)
    {
        string* targetStr = nullptr;
        if (activeInput == 1) 
        {
            targetStr = &companyInputStr;
        }
        else if (activeInput == 2) 
        {
            targetStr = &avoidPortInputStr;
        }
        else if (activeInput == 3) 
        {
            targetStr = &maxTimeInputStr;
        }
        if (!targetStr) 
        {
            return;
        }

        if (unicode == 8)
        {
            if (!targetStr->empty()) targetStr->pop_back();
        }
        else if (unicode >= 32 && unicode <= 126)
        {
            if (activeInput == 3)
            {
                if (unicode >= '0' && unicode <= '9' && targetStr->length() < 6)
                {
                    targetStr->push_back(static_cast<char>(unicode));
                }
            }
            else if (targetStr->length() < 20)
            {
                targetStr->push_back(static_cast<char>(unicode));
            }
        }
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (!isVisible) 
        {
            return 0;
        }

        if (companyInputBox.getGlobalBounds().contains(mousePos))
        { 
            activeInput = 1; 
            return 0; 
        }
        if (avoidPortInputBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 2; 
            return 0; 
        }
        if (maxTimeInputBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 3;
            return 0; 
        }

        if (addCompanyBtn.getGlobalBounds().contains(mousePos) && !companyInputStr.empty())
        {
            sfmlPreferences.addPreferredCompany(companyInputStr);
            companyInputStr = "";
            return 1;
        }
        if (addAvoidPortBtn.getGlobalBounds().contains(mousePos) && !avoidPortInputStr.empty())
        {
            sfmlPreferences.addAvoidedPort(avoidPortInputStr);
            avoidPortInputStr = "";
            return 2;
        }
        if (setMaxTimeBtn.getGlobalBounds().contains(mousePos) && !maxTimeInputStr.empty())
        {
            int minutes = 0;
            for (int i = 0; i < maxTimeInputStr.length(); i++)
            {
                minutes = minutes * 10 + (maxTimeInputStr[i] - '0');
            }
            sfmlPreferences.setTotalTime(minutes);
            maxTimeInputStr = "";
            return 3;
        }
        if (clearAllBtn.getGlobalBounds().contains(mousePos))
        {
            sfmlPreferences.clear();
            companyInputStr = avoidPortInputStr = maxTimeInputStr = "";
            return 4;
        }
        if (closeBtn.getGlobalBounds().contains(mousePos)) 
        { 
            hide(); 
            return 5; 
        }

        activeInput = 0;
        return 0;
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible || !fontPtr) 
        {
            return;
        }

        sf::RectangleShape overlay(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        window.draw(background);
        window.draw(headerBar);
        window.draw(titleText);

        float glow = (sin(animPhase * 2.f) + 1.f) / 2.f;
        sf::CircleShape star(12.f, 5);
        star.setOrigin(12.f, 12.f);
        star.setPosition(panelX + panelWidth - 45.f, panelY + 25.f);
        star.setFillColor(sf::Color(255, 220, 50, (sf::Uint8)(200 + glow * 55)));
        star.setRotation(animPhase * 20.f);
        window.draw(star);

        window.draw(companyLabel);
        window.draw(companyInputBox);
        companyInputText.setString(companyInputStr.empty() ? "e.g.  MaerskLine" : companyInputStr);
        companyInputText.setFillColor(companyInputStr.empty() ? sf::Color(100, 120, 150) : sf::Color(200, 220, 255));
        window.draw(companyInputText);
        window.draw(addCompanyBtn);
        window.draw(addCompanyBtnText);

        window.draw(avoidPortLabel);
        window.draw(avoidPortInputBox);
        avoidPortInputText.setString(avoidPortInputStr.empty() ? "e.g.  Singapore" : avoidPortInputStr);
        avoidPortInputText.setFillColor(avoidPortInputStr.empty() ? sf::Color(100, 120, 150) : sf::Color(200, 220, 255));
        window.draw(avoidPortInputText);
        window.draw(addAvoidPortBtn);
        window.draw(addAvoidPortBtnText);

        window.draw(maxTimeLabel);
        window.draw(maxTimeInputBox);
        maxTimeInputText.setString(maxTimeInputStr.empty() ? "e.g. 480" : maxTimeInputStr);
        maxTimeInputText.setFillColor(maxTimeInputStr.empty() ? sf::Color(100, 120, 150) : sf::Color(200, 220, 255));
        window.draw(maxTimeInputText);
        window.draw(setMaxTimeBtn);
        window.draw(setMaxTimeBtnText);

        window.draw(listsPanel);
        float listX = listsPanel.getPosition().x + 10.f;
        float listY = listsPanel.getPosition().y + 10.f;

        sf::Text prefTitle("Preferred Companies:", *fontPtr, 12);
        prefTitle.setFillColor(sf::Color(255, 200, 100));
        prefTitle.setStyle(sf::Text::Bold);
        prefTitle.setPosition(listX, listY);
        window.draw(prefTitle);
        listY += 18.f;

        if (sfmlPreferences.preferredCompanyCount == 0)
        {
            sf::Text t("  (none - all allowed)", *fontPtr, 11);
            t.setFillColor(sf::Color(120, 120, 140));
            t.setStyle(sf::Text::Italic);
            t.setPosition(listX, listY);
            window.draw(t);
            listY += 16.f;
        }
        else
        {
            for (int i = 0; i < sfmlPreferences.preferredCompanyCount && i < 3; i++)
            {
                sf::Text t("  * " + sfmlPreferences.preferredCompanies[i], *fontPtr, 11);
                t.setFillColor(sf::Color(150, 255, 150));
                t.setPosition(listX, listY);
                window.draw(t);
                listY += 16.f;
            }
        }

        listY += 8.f;
        sf::Text avoidTitle("Avoided Ports:", *fontPtr, 12);
        avoidTitle.setFillColor(sf::Color(255, 100, 100));
        avoidTitle.setStyle(sf::Text::Bold);
        avoidTitle.setPosition(listX, listY);
        window.draw(avoidTitle);
        listY += 18.f;

        if (sfmlPreferences.avoidedPortCount == 0)
        {
            sf::Text t("  (none)", *fontPtr, 11);
            t.setFillColor(sf::Color(120, 120, 140));
            t.setStyle(sf::Text::Italic);
            t.setPosition(listX, listY);
            window.draw(t);
            listY += 16.f;
        }
        else
        {
            for (int i = 0; i < sfmlPreferences.avoidedPortCount && i < 3; i++)
            {
                sf::Text t("  X " + sfmlPreferences.avoidedPorts[i], *fontPtr, 11);
                t.setFillColor(sf::Color(255, 150, 150));
                t.setPosition(listX, listY);
                window.draw(t);
                listY += 16.f;
            }
        }

        listY += 8.f;

        sf::Text timeTitle("Max Voyage Time:", *fontPtr, 12);
        timeTitle.setFillColor(sf::Color(100, 200, 255));
        timeTitle.setStyle(sf::Text::Bold);
        timeTitle.setPosition(listX, listY);
        window.draw(timeTitle);
        listY += 18.f;

        string timeStr = sfmlPreferences.useTotalTime ?
            ("  " + intToString(sfmlPreferences.totalVoyageTime) + " minutes") : "  (no limit)";
        sf::Text timeText(timeStr, *fontPtr, 11);
        timeText.setFillColor(sfmlPreferences.useTotalTime ? sf::Color(150, 220, 255) : sf::Color(120, 120, 140));
        timeText.setPosition(listX, listY);
        window.draw(timeText);

        window.draw(clearAllBtn);
        window.draw(clearAllBtnText);
        window.draw(closeBtn);
        window.draw(closeBtnText);
    }
};

PreferencesPanel preferencesPanel;
class MultiLegBuilderPanel
{
private:
    sf::RectangleShape overlay;
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::RectangleShape headerGlow;

    sf::Text titleText;
    sf::Text instructionText;

    sf::RectangleShape inputBox;
    sf::RectangleShape inputAccent;
    sf::Text inputLabel;
    sf::Text inputText;
    string portInput;
    bool inputFocused;

    sf::RectangleShape btnAddPort;
    sf::Text btnAddPortText;
    float addHoverProgress;

    sf::RectangleShape btnFindRoutes;
    sf::Text btnFindRoutesText;
    float findHoverProgress;

    sf::RectangleShape btnClear;
    sf::Text btnClearText;
    float clearHoverProgress;

    sf::RectangleShape btnClose;
    float closeHoverProgress;

    sf::RectangleShape linkedListPanel;
    sf::Text linkedListTitle;
    sf::Text statusText;
    sf::Text totalCostText;

    float panelWidth, panelHeight, panelX, panelY;
    float animPhase;
    sf::Font* fontPtr;

    bool isVisible;

public:
    MultiLegBuilderPanel() : isVisible(false), inputFocused(false), animPhase(0.f),
        addHoverProgress(0.f), findHoverProgress(0.f), clearHoverProgress(0.f),
        closeHoverProgress(0.f), fontPtr(nullptr)
    {
        portInput = "";
        panelWidth = 700.f;
        panelHeight = 550.f;
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;

        panelX = (windowWidth - panelWidth) / 2.f;
        panelY = (windowHeight - panelHeight) / 2.f;

        overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
        overlay.setPosition(0, 0);
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(12, 22, 38, 252));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color(0, 120, 120));

        headerBar.setSize(sf::Vector2f(panelWidth, 50.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(15, 30, 50));

        headerGlow.setSize(sf::Vector2f(panelWidth - 40.f, 2.f));
        headerGlow.setPosition(panelX + 20.f, panelY + 50.f);
        headerGlow.setFillColor(sf::Color(0, 200, 180, 150));

        titleText.setFont(font);
        titleText.setString("MULTI-LEG ROUTE BUILDER");
        titleText.setCharacterSize(18);
        titleText.setFillColor(sf::Color(0, 220, 200));
        titleText.setStyle(sf::Text::Bold);
        titleText.setLetterSpacing(2.f);
        titleText.setPosition(panelX + 25.f, panelY + 13.f);

        btnClose.setSize(sf::Vector2f(30.f, 30.f));
        btnClose.setPosition(panelX + panelWidth - 42.f, panelY + 10.f);
        btnClose.setFillColor(sf::Color(40, 50, 65));
        btnClose.setOutlineThickness(1.f);
        btnClose.setOutlineColor(sf::Color(60, 80, 100));

        instructionText.setFont(font);
        instructionText.setString("Add ports to build your multi-leg route.  Routes will be found automatically.");
        instructionText.setCharacterSize(12);
        instructionText.setFillColor(sf::Color(140, 160, 180));
        instructionText.setPosition(panelX + 25.f, panelY + 60.f);

        float inputX = panelX + 25.f;
        float inputY = panelY + 90.f;
        float inputW = panelWidth - 180.f;
        float inputH = 35.f;

        inputBox.setSize(sf::Vector2f(inputW, inputH));
        inputBox.setPosition(inputX, inputY + 16.f);
        inputBox.setFillColor(sf::Color(20, 35, 55));
        inputBox.setOutlineThickness(1.f);
        inputBox.setOutlineColor(sf::Color(40, 70, 100));

        inputAccent.setSize(sf::Vector2f(3.f, inputH));
        inputAccent.setPosition(inputX, inputY + 16.f);
        inputAccent.setFillColor(sf::Color(0, 200, 180));

        inputLabel.setFont(font);
        inputLabel.setString("PORT NAME");
        inputLabel.setCharacterSize(10);
        inputLabel.setFillColor(sf::Color(0, 200, 180));
        inputLabel.setLetterSpacing(1.2f);
        inputLabel.setPosition(inputX + 5.f, inputY);

        inputText.setFont(font);
        inputText.setString("Enter port name.. .");
        inputText.setCharacterSize(13);
        inputText.setFillColor(sf::Color(80, 100, 120));
        inputText.setPosition(inputX + 12.f, inputY + 24.f);

        btnAddPort.setSize(sf::Vector2f(120.f, 35.f));
        btnAddPort.setPosition(panelX + panelWidth - 145.f, inputY + 16.f);
        btnAddPort.setFillColor(sf::Color(0, 100, 90));

        btnAddPortText.setFont(font);
        btnAddPortText.setString("ADD PORT");
        btnAddPortText.setCharacterSize(12);
        btnAddPortText.setFillColor(sf::Color::White);
        btnAddPortText.setStyle(sf::Text::Bold);
        sf::FloatRect addBounds = btnAddPortText.getLocalBounds();
        btnAddPortText.setPosition(
            btnAddPort.getPosition().x + (120.f - addBounds.width) / 2.f,
            btnAddPort.getPosition().y + 8.f
        );

        linkedListPanel.setSize(sf::Vector2f(panelWidth - 50.f, 280.f));
        linkedListPanel.setPosition(panelX + 25.f, panelY + 150.f);
        linkedListPanel.setFillColor(sf::Color(8, 18, 32));
        linkedListPanel.setOutlineThickness(1.f);
        linkedListPanel.setOutlineColor(sf::Color(40, 70, 100));

        linkedListTitle.setFont(font);
        linkedListTitle.setString("LINKED LIST VISUALIZATION");
        linkedListTitle.setCharacterSize(11);
        linkedListTitle.setFillColor(sf::Color(0, 180, 160));
        linkedListTitle.setLetterSpacing(1.5f);
        linkedListTitle.setPosition(panelX + 35.f, panelY + 155.f);

        float btnY = panelY + panelHeight - 80.f;
        float btnW = 150.f;
        float btnH = 40.f;
        float btnSpacing = 20.f;
        float totalBtnWidth = btnW * 2 + btnSpacing;
        float btnStartX = panelX + (panelWidth - totalBtnWidth) / 2.f;

        btnFindRoutes.setSize(sf::Vector2f(btnW, btnH));
        btnFindRoutes.setPosition(btnStartX, btnY);
        btnFindRoutes.setFillColor(sf::Color(0, 120, 100));

        btnFindRoutesText.setFont(font);
        btnFindRoutesText.setString("FIND ROUTES");
        btnFindRoutesText.setCharacterSize(13);
        btnFindRoutesText.setFillColor(sf::Color::White);
        btnFindRoutesText.setStyle(sf::Text::Bold);
        sf::FloatRect findBounds = btnFindRoutesText.getLocalBounds();
        btnFindRoutesText.setPosition(
            btnStartX + (btnW - findBounds.width) / 2.f,
            btnY + 10.f
        );

        btnClear.setSize(sf::Vector2f(btnW, btnH));
        btnClear.setPosition(btnStartX + btnW + btnSpacing, btnY);
        btnClear.setFillColor(sf::Color(100, 50, 50));

        btnClearText.setFont(font);
        btnClearText.setString("CLEAR ALL");
        btnClearText.setCharacterSize(13);
        btnClearText.setFillColor(sf::Color::White);
        btnClearText.setStyle(sf::Text::Bold);
        sf::FloatRect clearBounds = btnClearText.getLocalBounds();
        btnClearText.setPosition(
            btnStartX + btnW + btnSpacing + (btnW - clearBounds.width) / 2.f,
            btnY + 10.f
        );

        statusText.setFont(font);
        statusText.setString("");
        statusText.setCharacterSize(12);
        statusText.setFillColor(sf::Color(150, 200, 150));
        statusText.setPosition(panelX + 25.f, panelY + panelHeight - 35.f);

        totalCostText.setFont(font);
        totalCostText.setString("");
        totalCostText.setCharacterSize(12);
        totalCostText.setFillColor(sf::Color(200, 200, 100));
        totalCostText.setPosition(panelX + panelWidth - 200.f, panelY + panelHeight - 35.f);
    }

    void show() { isVisible = true; inputFocused = false; portInput = ""; }
    void hide() { isVisible = false; }
    bool visible() const { return isVisible; }

    void handleTextInput(sf::Uint32 unicode)
    {
        if (!isVisible || !inputFocused) return;

        if (unicode == 8)
        {
            if (!portInput.empty()) portInput.pop_back();
        }
        else if (unicode == 13)
        {
            addCurrentPort();
        }
        else if (unicode >= 32 && unicode < 128 && portInput.length() < 30)
        {
            portInput += static_cast<char>(unicode);
        }
    }

    void addCurrentPort()
    {
        if (portInput.empty()) return;

        int portIdx = findPortIndexByName(portInput);
        if (portIdx != -1)
        {
            multiLegRoute.addPort(portIdx, portInput);
            portInput = "";
            updateStatus("Port added: " + ports[portIdx].name, sf::Color(100, 255, 150));
        }
        else
        {
            updateStatus("Port not found: " + portInput, sf::Color(255, 100, 100));
        }
    }

    void updateStatus(const string& msg, sf::Color color)
    {
        statusText.setString(msg);
        statusText.setFillColor(color);
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (!isVisible) return -1;

        inputFocused = false;

        if (inputBox.getGlobalBounds().contains(mousePos))
        {
            inputFocused = true;
            return -1;
        }

        if (btnAddPort.getGlobalBounds().contains(mousePos))
        {
            addCurrentPort();
            return -1;
        }

        if (btnFindRoutes.getGlobalBounds().contains(mousePos))
        {
            if (multiLegRoute.getCount() >= 2)
            {
                bool success = multiLegRoute.autoFindRoutes();  // No date parameter
                if (success)
                {
                    updateStatus("All routes found successfully!", sf::Color(100, 255, 150));
                }
                else
                {
                    updateStatus("Some routes could not be found", sf::Color(255, 200, 100));
                }
                totalCostText.setString("Total Cost: $" + intToString(multiLegRoute.getTotalCost()));
            }
            else
            {
                updateStatus("Add at least 2 ports", sf::Color(255, 100, 100));
            }
            return -1;
        }

        if (btnClear.getGlobalBounds().contains(mousePos))
        {
            multiLegRoute.clear();
            updateStatus("Route cleared", sf::Color(150, 150, 150));
            totalCostText.setString("");
            return -1;
        }

        if (btnClose.getGlobalBounds().contains(mousePos))
        {
            hide();
            return 0;
        }

        if (linkedListPanel.getGlobalBounds().contains(mousePos))
        {
            handleNodeClick(mousePos);
            return -1;
        }

        return -1;
    }

    void handleNodeClick(sf::Vector2f mousePos)
    {
        float nodeWidth = 120.f;
        float nodeHeight = 60.f;
        float startX = linkedListPanel.getPosition().x + 20.f;
        float startY = linkedListPanel.getPosition().y + 40.f;
        float spacing = 50.f;

        LegNode* current = multiLegRoute.getHead();
        int index = 0;
        float x = startX;
        float y = startY;

        while (current != nullptr)
        {
            if (x + nodeWidth > linkedListPanel.getPosition().x + linkedListPanel.getSize().x - 40.f)
            {
                x = startX;
                y += nodeHeight + 60.f;
            }

            sf::FloatRect nodeRect(x, y, nodeWidth, nodeHeight);
            if (nodeRect.contains(mousePos))
            {
                multiLegRoute.removePortAtPosition(index);
                updateStatus("Removed port at position " + intToString(index + 1), sf::Color(255, 200, 100));
                return;
            }

            x += nodeWidth + spacing;
            current = current->next;
            index++;
        }
    }

    void update(float deltaTime)
    {
        if (!isVisible) return;

        animPhase += deltaTime * 2.5f;
        if (animPhase > 6.28318f) animPhase -= 6.28318f;

        if (portInput.empty())
        {
            inputText.setString("Enter port name...");
            inputText.setFillColor(sf::Color(80, 100, 120));
        }
        else
        {
            inputText.setString(portInput);
            inputText.setFillColor(sf::Color(220, 235, 255));
        }

        inputBox.setOutlineColor(inputFocused ? sf::Color(0, 200, 180) : sf::Color(40, 70, 100));
        inputBox.setOutlineThickness(inputFocused ? 2.f : 1.f);
        inputAccent.setSize(sf::Vector2f(inputFocused ? 5.f : 3.f, 35.f));

        float pulse = (sin(animPhase) + 1.f) / 2.f;
        headerGlow.setFillColor(sf::Color(0, (sf::Uint8)(180 + pulse * 40), (sf::Uint8)(160 + pulse * 40), (sf::Uint8)(120 + pulse * 80)));
    }

    void updateHover(sf::Vector2f mousePos, float deltaTime)
    {
        if (!isVisible) return;

        bool hover = btnAddPort.getGlobalBounds().contains(mousePos);
        addHoverProgress = hover ? min(1.f, addHoverProgress + deltaTime * 8.f) : max(0.f, addHoverProgress - deltaTime * 5.f);
        btnAddPort.setFillColor(sf::Color((sf::Uint8)(0 + addHoverProgress * 20), (sf::Uint8)(100 + addHoverProgress * 50), (sf::Uint8)(90 + addHoverProgress * 50)));

        hover = btnFindRoutes.getGlobalBounds().contains(mousePos);
        findHoverProgress = hover ? min(1.f, findHoverProgress + deltaTime * 8.f) : max(0.f, findHoverProgress - deltaTime * 5.f);
        btnFindRoutes.setFillColor(sf::Color((sf::Uint8)(0 + findHoverProgress * 20), (sf::Uint8)(120 + findHoverProgress * 50), (sf::Uint8)(100 + findHoverProgress * 50)));

        hover = btnClear.getGlobalBounds().contains(mousePos);
        clearHoverProgress = hover ? min(1.f, clearHoverProgress + deltaTime * 8.f) : max(0.f, clearHoverProgress - deltaTime * 5.f);
        btnClear.setFillColor(sf::Color((sf::Uint8)(100 + clearHoverProgress * 50), (sf::Uint8)(50 + clearHoverProgress * 20), (sf::Uint8)(50 + clearHoverProgress * 20)));

        hover = btnClose.getGlobalBounds().contains(mousePos);
        closeHoverProgress = hover ? min(1.f, closeHoverProgress + deltaTime * 8.f) : max(0.f, closeHoverProgress - deltaTime * 5.f);
        btnClose.setFillColor(sf::Color((sf::Uint8)(40 + closeHoverProgress * 80), (sf::Uint8)(50 - closeHoverProgress * 20), (sf::Uint8)(65 - closeHoverProgress * 30)));
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible) return;

        window.draw(overlay);

        sf::RectangleShape shadow = background;
        shadow.setPosition(panelX + 6.f, panelY + 6.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        shadow.setOutlineThickness(0);
        window.draw(shadow);

        window.draw(background);
        window.draw(headerBar);
        window.draw(headerGlow);
        window.draw(titleText);

        window.draw(btnClose);
        float cx = btnClose.getPosition().x + 15.f;
        float cy = btnClose.getPosition().y + 15.f;
        sf::RectangleShape xLine1(sf::Vector2f(14.f, 2.f));
        sf::RectangleShape xLine2(sf::Vector2f(14.f, 2.f));
        xLine1.setOrigin(7.f, 1.f);
        xLine2.setOrigin(7.f, 1.f);
        xLine1.setPosition(cx, cy);
        xLine2.setPosition(cx, cy);
        xLine1.setRotation(45.f);
        xLine2.setRotation(-45.f);
        sf::Color xColor(150 + (int)(closeHoverProgress * 105), 150 - (int)(closeHoverProgress * 50), 150 - (int)(closeHoverProgress * 50));
        xLine1.setFillColor(xColor);
        xLine2.setFillColor(xColor);
        window.draw(xLine1);
        window.draw(xLine2);

        window.draw(instructionText);

        window.draw(inputBox);
        if (inputFocused)
        {
            float pulse = (sin(animPhase * 3.f) + 1.f) / 2.f;
            sf::RectangleShape glow = inputAccent;
            glow.setSize(sf::Vector2f(inputAccent.getSize().x + 6.f, inputAccent.getSize().y));
            glow.setPosition(inputAccent.getPosition().x - 3.f, inputAccent.getPosition().y);
            glow.setFillColor(sf::Color(0, 200, 180, (sf::Uint8)(40 + pulse * 50)));
            window.draw(glow);

            if (pulse > 0.5f)
            {
                sf::RectangleShape cursor(sf::Vector2f(2.f, 16.f));
                float textWidth = portInput.empty() ? 0 : inputText.getLocalBounds().width;
                cursor.setPosition(inputText.getPosition().x + textWidth + 2.f, inputText.getPosition().y + 2.f);
                cursor.setFillColor(sf::Color(0, 220, 200));
                window.draw(cursor);
            }
        }
        window.draw(inputAccent);
        window.draw(inputLabel);
        window.draw(inputText);

        window.draw(btnAddPort);
        sf::RectangleShape addHighlight(sf::Vector2f(btnAddPort.getSize().x, 2.f));
        addHighlight.setPosition(btnAddPort.getPosition());
        addHighlight.setFillColor(sf::Color(255, 255, 255, 30));
        window.draw(addHighlight);
        window.draw(btnAddPortText);

        window.draw(linkedListPanel);
        window.draw(linkedListTitle);

        drawLinkedList(window);

        window.draw(btnFindRoutes);
        sf::RectangleShape findHighlight(sf::Vector2f(btnFindRoutes.getSize().x, 2.f));
        findHighlight.setPosition(btnFindRoutes.getPosition());
        findHighlight.setFillColor(sf::Color(255, 255, 255, 30));
        window.draw(findHighlight);
        window.draw(btnFindRoutesText);

        window.draw(btnClear);
        sf::RectangleShape clearHighlight(sf::Vector2f(btnClear.getSize().x, 2.f));
        clearHighlight.setPosition(btnClear.getPosition());
        clearHighlight.setFillColor(sf::Color(255, 255, 255, 30));
        window.draw(clearHighlight);
        window.draw(btnClearText);

        window.draw(statusText);
        window.draw(totalCostText);

        drawCorners(window);
    }

    void drawLinkedList(sf::RenderWindow& window)
    {
        if (multiLegRoute.isEmpty())
        {
            sf::Text emptyText;
            emptyText.setFont(*fontPtr);
            emptyText.setString("No ports added yet.  Add ports to build your route.");
            emptyText.setCharacterSize(13);
            emptyText.setFillColor(sf::Color(100, 120, 140));
            emptyText.setPosition(linkedListPanel.getPosition().x + 30.f, linkedListPanel.getPosition().y + 130.f);
            window.draw(emptyText);

            sf::Text headText;
            headText.setFont(*fontPtr);
            headText.setString("HEAD -> NULL");
            headText.setCharacterSize(12);
            headText.setFillColor(sf::Color(150, 100, 100));
            headText.setPosition(linkedListPanel.getPosition().x + 30.f, linkedListPanel.getPosition().y + 160.f);
            window.draw(headText);
            return;
        }

        float nodeWidth = 110.f;
        float nodeHeight = 55.f;
        float startX = linkedListPanel.getPosition().x + 25.f;
        float startY = linkedListPanel.getPosition().y + 45.f;
        float arrowWidth = 40.f;

        sf::Text headLabel;
        headLabel.setFont(*fontPtr);
        headLabel.setString("HEAD");
        headLabel.setCharacterSize(10);
        headLabel.setFillColor(sf::Color(100, 200, 150));
        headLabel.setStyle(sf::Text::Bold);
        headLabel.setPosition(startX, startY - 15.f);
        window.draw(headLabel);

        LegNode* current = multiLegRoute.getHead();
        float x = startX;
        float y = startY;
        int nodeNum = 0;

        while (current != nullptr)
        {
            if (x + nodeWidth > linkedListPanel.getPosition().x + linkedListPanel.getSize().x - 30.f)
            {
                x = startX;
                y += nodeHeight + 50.f;
            }

            bool hasRoute = (current->routeToNextIndex != -1);
            bool isLast = (current->next == nullptr);

            sf::RectangleShape nodeBox(sf::Vector2f(nodeWidth, nodeHeight));
            nodeBox.setPosition(x, y);
            nodeBox.setFillColor(sf::Color(25, 45, 70));
            nodeBox.setOutlineThickness(2.f);

            if (isLast)
                nodeBox.setOutlineColor(sf::Color(200, 100, 100));
            else if (hasRoute)
                nodeBox.setOutlineColor(sf::Color(100, 200, 150));
            else
                nodeBox.setOutlineColor(sf::Color(200, 180, 100));

            window.draw(nodeBox);

            sf::RectangleShape divider(sf::Vector2f(nodeWidth - 4.f, 1.f));
            divider.setPosition(x + 2.f, y + 30.f);
            divider.setFillColor(sf::Color(60, 90, 120));
            window.draw(divider);

            string portName = current->portName;
            if (portName.length() > 12)
                portName = portName.substr(0, 10) + ".. ";

            sf::Text nameText;
            nameText.setFont(*fontPtr);
            nameText.setString(portName);
            nameText.setCharacterSize(11);
            nameText.setFillColor(sf::Color(220, 240, 255));
            nameText.setStyle(sf::Text::Bold);
            nameText.setPosition(x + 5.f, y + 5.f);
            window.draw(nameText);

            stringstream ss;
            ss << current;
            string addrStr = ss.str();
            if (addrStr.length() > 10)
                addrStr = addrStr.substr(addrStr.length() - 8);

            sf::Text addrText;
            addrText.setFont(*fontPtr);
            addrText.setString(addrStr);
            addrText.setCharacterSize(9);
            addrText.setFillColor(sf::Color(150, 180, 200));
            addrText.setPosition(x + 5.f, y + 18.f);
            window.draw(addrText);

            sf::Text nextText;
            nextText.setFont(*fontPtr);
            if (isLast)
            {
                nextText.setString("next: NULL");
                nextText.setFillColor(sf::Color(200, 100, 100));
            }
            else
            {
                stringstream ss2;
                ss2 << current->next;
                string nextAddrStr = ss2.str();
                if (nextAddrStr.length() > 8)
                    nextAddrStr = nextAddrStr.substr(nextAddrStr.length() - 6);
                nextText.setString("next: " + nextAddrStr);
                nextText.setFillColor(sf::Color(100, 180, 200));
            }
            nextText.setCharacterSize(8);
            nextText.setPosition(x + 5.f, y + 38.f);
            window.draw(nextText);

            if (!isLast)
            {
                float arrowStartX = x + nodeWidth;
                float arrowY = y + nodeHeight / 2.f;
                float arrowEndX = arrowStartX + arrowWidth;

                if (arrowEndX + nodeWidth > linkedListPanel.getPosition().x + linkedListPanel.getSize().x - 20.f)
                {
                    sf::RectangleShape arrowDown(sf::Vector2f(2.f, 30.f));
                    arrowDown.setPosition(x + nodeWidth / 2.f, y + nodeHeight);
                    arrowDown.setFillColor(hasRoute ? sf::Color(100, 200, 150) : sf::Color(200, 180, 100));
                    window.draw(arrowDown);

                    sf::ConvexShape arrowHead;
                    arrowHead.setPointCount(3);
                    arrowHead.setPoint(0, sf::Vector2f(0.f, 0.f));
                    arrowHead.setPoint(1, sf::Vector2f(-6.f, -10.f));
                    arrowHead.setPoint(2, sf::Vector2f(6.f, -10.f));
                    arrowHead.setPosition(x + nodeWidth / 2.f, y + nodeHeight + 30.f);
                    arrowHead.setFillColor(hasRoute ? sf::Color(100, 200, 150) : sf::Color(200, 180, 100));
                    window.draw(arrowHead);
                }
                else
                {
                    sf::RectangleShape arrowLine(sf::Vector2f(arrowWidth - 10.f, 2.f));
                    arrowLine.setPosition(arrowStartX, arrowY);
                    arrowLine.setFillColor(hasRoute ? sf::Color(100, 200, 150) : sf::Color(200, 180, 100));
                    window.draw(arrowLine);

                    sf::ConvexShape arrowHead;
                    arrowHead.setPointCount(3);
                    arrowHead.setPoint(0, sf::Vector2f(10.f, 0.f));
                    arrowHead.setPoint(1, sf::Vector2f(0.f, -6.f));
                    arrowHead.setPoint(2, sf::Vector2f(0.f, 6.f));
                    arrowHead.setPosition(arrowEndX - 10.f, arrowY);
                    arrowHead.setFillColor(hasRoute ? sf::Color(100, 200, 150) : sf::Color(200, 180, 100));
                    window.draw(arrowHead);
                }

                if (hasRoute)
                {
                    sf::Text routeInfo;
                    routeInfo.setFont(*fontPtr);
                    routeInfo.setString("$" + intToString(current->cost));
                    routeInfo.setCharacterSize(9);
                    routeInfo.setFillColor(sf::Color(100, 200, 150));
                    routeInfo.setPosition(arrowStartX + 5.f, arrowY - 15.f);
                    window.draw(routeInfo);
                }
            }

            x += nodeWidth + arrowWidth;
            current = current->next;
            nodeNum++;
        }

        sf::Text countText;
        countText.setFont(*fontPtr);
        countText.setString("Nodes: " + intToString(multiLegRoute.getCount()) + " | Complete: " + (multiLegRoute.isComplete() ? "Yes" : "No"));
        countText.setCharacterSize(10);
        countText.setFillColor(sf::Color(120, 150, 180));
        countText.setPosition(linkedListPanel.getPosition().x + linkedListPanel.getSize().x - 180.f, linkedListPanel.getPosition().y + linkedListPanel.getSize().y - 20.f);
        window.draw(countText);
    }

    void drawCorners(sf::RenderWindow& window)
    {
        sf::Color color(0, 180, 160, 100);
        float len = 12.f, thick = 2.f;

        sf::RectangleShape h(sf::Vector2f(len, thick));
        sf::RectangleShape v(sf::Vector2f(thick, len));
        h.setFillColor(color);
        v.setFillColor(color);

        h.setPosition(panelX + 2.f, panelY + 2.f);
        v.setPosition(panelX + 2.f, panelY + 2.f);
        window.draw(h); window.draw(v);

        h.setPosition(panelX + panelWidth - len - 2.f, panelY + 2.f);
        v.setPosition(panelX + panelWidth - thick - 2.f, panelY + 2.f);
        window.draw(h); window.draw(v);

        h.setPosition(panelX + 2.f, panelY + panelHeight - thick - 2.f);
        v.setPosition(panelX + 2.f, panelY + panelHeight - len - 2.f);
        window.draw(h); window.draw(v);

        h.setPosition(panelX + panelWidth - len - 2.f, panelY + panelHeight - thick - 2.f);
        v.setPosition(panelX + panelWidth - thick - 2.f, panelY + panelHeight - len - 2.f);
        window.draw(h); window.draw(v);
    }
};

MultiLegBuilderPanel multiLegBuilderPanel;

void clearAlgorithmVisualization() 
{
    algorithmLegCount = 0;
    algorithmName = "";
    algorithmMetric = "";
    algorithmTotalValue = 0;
}

int findPortIndexByName(const string& name) 
{
    for (int i = 0; i < portCount; i++)
    {
        if (ports[i].name == name) 
        {
            return i;
        }
    }
    return -1;
}

sf::Vector2f latlonToPixel(double lat, double lon, const sf::Vector2u& imgSize) 
{
    double x = (lon + 180.0) / 360.0 * imgSize.x;
    double y = (90.0 - lat) / 180.0 * imgSize.y;
    return sf::Vector2f((float)x, (float)y);
}

void loadOceanWaypoints() 
{
    oceanWaypoints[0].lat = 30.0;  oceanWaypoints[0].lon = 32.0;
    oceanWaypoints[1].lat = -35.0; oceanWaypoints[1].lon = 20.0;
    oceanWaypoints[2].lat = 36.0;  oceanWaypoints[2].lon = -5.0;
    oceanWaypoints[3].lat = 9.0;   oceanWaypoints[3].lon = -80.0;
    oceanWaypoints[4].lat = 2.0;   oceanWaypoints[4].lon = 100.0;
    oceanWaypoints[5].lat = 12.0;  oceanWaypoints[5].lon = 50.0;
    oceanWaypoints[6].lat = 0.0;   oceanWaypoints[6].lon = -30.0;
    oceanWaypoints[7].lat = 0.0;   oceanWaypoints[7].lon = -150.0;
    oceanWaypoints[8].lat = -10.0; oceanWaypoints[8].lon = 80.0;
    oceanWaypoints[9].lat = 50.0;  oceanWaypoints[9].lon = -30.0;
    oceanWaypoints[10].lat = 40.0; oceanWaypoints[10].lon = -160.0;
    oceanWaypoints[11].lat = -20.0; oceanWaypoints[11].lon = 160.0;
    oceanWaypoints[12].lat = 60.0; oceanWaypoints[12].lon = -170.0;
    oceanWaypoints[13].lat = -5.0; oceanWaypoints[13].lon = 115.0;
    oceanWaypoints[14].lat = 15.0; oceanWaypoints[14].lon = 65.0;
}

void loadPorts() 
{
    string names[totalPorts] = 
    {
        "AbuDhabi","Alexandria","Antwerp","Athens","Busan","CapeTown","Chittagong","Colombo",
        "Copenhagen","Doha","Dubai","Dublin","Durban","Genoa","Hamburg","Helsinki","HongKong",
        "Istanbul","Jakarta","Jeddah","Karachi","Lisbon","London","LosAngeles","Manila",
        "Marseille","Melbourne","Montreal","Mumbai","NewYork","Osaka","Oslo","PortLouis",
        "Rotterdam","Shanghai","Singapore","Stockholm","Sydney","Tokyo","Vancouver"
    };

    double latitudes[totalPorts] = 
    {
        24.4539,31.2001,51.2194,37.9838,35.1796,-33.9249,22.3569,6.9271,
        55.6761,25.2854,25.2048,53.3498,-29.8587,44.4056,53.5511,60.1699,22.3193,
        41.0082,-6.2088,21.4858,24.8607,38.7223,51.5074,34.0522,14.5995,
        43.2965,-37.8136,45.5017,19.0760,40.7128,34.6937,59.9139,-20.1609,
        51.9244,31.2304,1.3521,59.3293,-33.8688,35.6762,49.2827
    };

    double longitudes[totalPorts] = 
    {
        54.3773,29.9187,4.4025,23.7275,129.0756,18.4241,91.7832,79.8612,
        12.5683,51.5310,55.2708,-6.2603,31.0218,8.9463,9.9937,24.9384,114.1694,
        28.9784,106.8456,39.1925,67.0011,-9.1393,-0.1278,-118.2437,120.9842,
        5.3698,144.9631,-73.5673,72.8777,-74.0060,135.5023,10.7522,57.5012,
        4.4777,121.4737,103.8198,18.0686,151.2093,139.6503,-123.1207
    };

    portCount = 0;
    for (int i = 0; i < totalPorts; i++) 
    {
        ports[i].name = names[i];
        ports[i].lat = latitudes[i];
        ports[i].lon = longitudes[i];
        portCount++;
    }
}

bool loadRoutes(const string& fname) 
{
    routeCount = 0;
    ifstream fin(fname.c_str());
    if (!fin.is_open()) 
    {
        cout << "Cannot open routes file: " << fname << endl;
        return false;
    }

    string origin, destination, date, depTime, arrTime, company;
    int vCost;
    while (fin >> origin >> destination >> date >> depTime >> arrTime >> vCost >> company) 
    {
        if (routeCount >= totalRoutes) 
        {
            break;
        }
        int originIndex = findPortIndexByName(origin);
        int destinationIndex = findPortIndexByName(destination);
        if (originIndex == -1 || destinationIndex == -1) 
        {
            continue;
        }

        routes[routeCount].originIndex = originIndex;
        routes[routeCount].destinationIndex = destinationIndex;
        routes[routeCount].date = date;
        routes[routeCount].depTime = depTime;
        routes[routeCount].arrTime = arrTime;
        routes[routeCount].vCost = vCost;
        routes[routeCount].company = company;
        routes[routeCount].valid = true;
        routeCount++;
    }
    fin.close();
    return true;
}

sf::VideoMode getDesktopMode() 
{
    return sf::VideoMode::getDesktopMode();
}

int timeToMinutesLocal(const string& time) 
{
    int hrs = 0, mins = 0;
    int colon = time.find(':');
    if (colon == string::npos) 
    {
        return -1;
    }
    for (int i = 0; i < colon; i++) 
    {
        hrs = hrs * 10 + (time[i] - '0');
    }
    for (int i = colon + 1; i < time.size(); i++)  
    {
        mins = mins * 10 + (time[i] - '0');
    }
    return hrs * 60 + mins;
}

int dateToIntLocal(const string& date) 
{
    int d = 0, m = 0, y = 0;
    int p = 0;
    while (p < date.size() && date[p] != '/') 
    {
        d = d * 10 + (date[p++] - '0');
    }
    p++;
    while (p < date.size() && date[p] != '/') 
    {
        m = m * 10 + (date[p++] - '0');
    }
    p++;
    while (p < date.size()) 
    {
        y = y * 10 + (date[p++] - '0');
    }
    return y * 10000 + m * 100 + d;
}

string getNextDateLocal(string currentDate) 
{
    int day = 0, month = 0, year = 0;
    size_t pos = 0;
    int dim[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    while (pos < currentDate.size() && currentDate[pos] != '/')
    {
        day = day * 10 + (currentDate[pos++] - '0');
    }
    pos++;
    while (pos < currentDate.size() && currentDate[pos] != '/')
    {
        month = month * 10 + (currentDate[pos++] - '0');
    }
    pos++;
    while (pos < currentDate.size())
    {
        year = year * 10 + (currentDate[pos++] - '0');
    }

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        dim[2] = 29;
    }
    day++;
    if (day > dim[month]) 
    {
        day = 1; 
        month++;
        if (month > 12) 
        {
            month = 1; 
            year++; 
        }
    }

    string r;
    if (day < 10) 
    {
        r += "0";
    }
    r += to_string(day) + "/";
    if (month < 10) 
    {
        r += "0";
    }
    r += to_string(month) + "/";
    r += to_string(year);
    return r;
}

bool isTimeValidLocal(const string& prevRouteDate, const string& prevDepTime, const string& prevArrTime, const string& nextDepDate, const string& nextDepTime)
{
    int prevDepMins = timeToMinutesLocal(prevDepTime);
    int prevArrMins = timeToMinutesLocal(prevArrTime);
    int nextDepMins = timeToMinutesLocal(nextDepTime);

    if (prevDepMins == -1 || prevArrMins == -1 || nextDepMins == -1)
    {
        return false;
    }

    int prevDate = dateToIntLocal(prevRouteDate);
    int nextDate = dateToIntLocal(nextDepDate);

    int actualArrivalDate = prevDate;
    if (prevArrMins < prevDepMins)
    {
        string nextDay = getNextDateLocal(prevRouteDate);
        actualArrivalDate = dateToIntLocal(nextDay);
    }

    const int minLayover = 15;
    const int maxLayoverDays = 15;

    if (nextDate == actualArrivalDate)
    {
        return (nextDepMins >= prevArrMins + minLayover);
    }

    if (nextDate > actualArrivalDate)
    {
        int daysDifference = nextDate - actualArrivalDate;
        return (daysDifference <= maxLayoverDays);
    }

    return false;
}

bool isLayoverFeasibleLocal(const Routes& prevRoute, const Routes& nextRoute)
{
    return isTimeValidLocal(
        prevRoute.date,       
        prevRoute.depTime,    
        prevRoute.arrTime,    
        nextRoute.date,       
        nextRoute.depTime 
    );
}

int determineOceanRoute(const Port& src, const Port& dest) 
{
    if ((src.lon < 30 && src.lat > 30) && (dest.lon > 50 && dest.lat > 10)) 
    {
        return 1;
    }
    if ((src.lon < 30 && src.lat > 30) && (dest.lon > 100)) 
    {
        return 2;
    }
    if ((src.lon < -60 && src.lon > -130) && (dest.lon > 100))
    {
        return 3;
    }
    if ((src.lon > 0 && src.lon < 40 && src.lat < 0) && (dest.lon > 60 && dest.lat < 30)) 
    {
        return 4;
    }
    if ((src.lon < -30 && src.lon > -80) && (dest.lon > -20 && dest.lon < 20)) 
    {
        return 5;
    }
    if ((src.lon < 20 && src.lat > 30) && (dest.lon > 30 && dest.lat < 0)) 
    {
        return 6;
    }
    return 0;
}

void generateOceanPath(sf::Vector2f start, sf::Vector2f end, int srcIndex, int destIndex, sf::Vector2f* pts, int& len) 
{
    pts[0] = start;
    len = 1;
    int type = determineOceanRoute(ports[srcIndex], ports[destIndex]);

    if (type == 1) 
    { 
        pts[len++] = oceanWaypoints[2].pixel; 
        pts[len++] = oceanWaypoints[0].pixel; 
    }
    else if (type == 2) 
    {
        pts[len++] = oceanWaypoints[2].pixel; 
        pts[len++] = oceanWaypoints[0].pixel; 
        pts[len++] = oceanWaypoints[4].pixel; 
    }
    else if (type == 3) 
    {
        pts[len++] = oceanWaypoints[3].pixel; 
        pts[len++] = oceanWaypoints[7].pixel; 
    }
    else if (type == 4) 
    {
        pts[len++] = oceanWaypoints[1].pixel;
        pts[len++] = oceanWaypoints[8].pixel; 
    }
    else if (type == 5) 
    {
        pts[len++] = oceanWaypoints[6].pixel; 
    }
    else if (type == 6) 
    { 
        pts[len++] = oceanWaypoints[1].pixel; 
    }
    else 
    {
        sf::Vector2f mid = (start + end) / 2.f;
        sf::Vector2f diff = end - start;
        sf::Vector2f perp(-diff.y, diff.x);
        float l = std::sqrt(perp.x * perp.x + perp.y * perp.y);
        if (l > 0) 
        {
            perp.x /= l; 
            perp.y /= l;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            mid += perp * (dist * 0.1f);
            pts[len++] = mid;
        }
    }
    pts[len++] = end;
}

void drawOceanRoute(sf::RenderWindow& window, sf::Vector2f* pts, int count, sf::Color color, float thickness = 1.f) {
    if (count < 2) 
    {
        return;
    }

    int layers = (int)std::max(1.f, thickness);
    float step = 0.5f;
    for (int i = 0; i < layers; i++) 
    {
        float off = (i - layers / 2.f) * step;
        for (int i = 0; i < count - 1; i++) 
        {
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
            for (int j = 0; j < segs; j++) 
            {
                float t1 = (float)j / segs;
                float t2 = (float)(j + 1) / segs;
                float u1 = 1 - t1;
                sf::Vector2f p1 = u1 * u1 * s + 2.f * u1 * t1 * c + t1 * t1 * e;
                float u2 = 1 - t2;
                sf::Vector2f p2 = u2 * u2 * s + 2.f * u2 * t2 * c + t2 * t2 * e;

                sf::Vector2f seg = p2 - p1;
                sf::Vector2f segPerp(-seg.y, seg.x);
                float sl = std::sqrt(segPerp.x * segPerp.x + segPerp.y * segPerp.y);
                if (sl > 0) 
                {
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

void drawAlgorithmExploration(sf::RenderWindow& window, float glowTime)
{
    if (algorithmVisualization.exploredCount == 0) 
    {
        return;
    }
    for (int i = 0; i < algorithmVisualization.exploredCount; i++)
    {
        ExploredEdge& edge = algorithmVisualization.exploredEdges[i];

        if (edge.alpha <= 0) 
        {
            continue;
        }

        int fromIndex = edge.fromPortIndex;
        int toIndex = edge.toPortIndex;

        if (fromIndex < 0 || fromIndex >= portCount || toIndex < 0 || toIndex >= portCount) 
        {
            continue;
        }

        sf::Vector2f a = ports[fromIndex].pixel;
        sf::Vector2f b = ports[toIndex].pixel;

        sf::Vector2f pathPoints[totalWayPoints];
        int pathLength = 0;
        generateOceanPath(a, b, fromIndex, toIndex, pathPoints, pathLength);

        sf::Color edgeColor;
        float thickness;

        if (edge.isOptimal && algorithmVisualization.showFinalPath)
        {
            float pulse = (sin(glowTime * 3.f) + 1.f) / 2.f;
            edgeColor = sf::Color(0, 255, 100, (sf::Uint8)(200 + pulse * 55));
            thickness = 5.f + pulse * 2.f;
        }
        else if (edge.isBeingExplored)
        {
            float pulse = (sin(glowTime * 4.f + i * 0.3f) + 1.f) / 2.f;
            edgeColor = sf::Color(255, 150 + (int)(pulse * 50), 0, (sf::Uint8)edge.alpha);
            thickness = 2.f + pulse;
        }
        else
        {
            edgeColor = sf::Color(100, 100, 150, (sf::Uint8)(edge.alpha * 0.3f));
            thickness = 1.f;
        }

        drawOceanRoute(window, pathPoints, pathLength, edgeColor, thickness);
    }
    if (algorithmVisualization.isAnimating && algorithmVisualization.currentExploringPort >= 0)
    {
        int portIndex = algorithmVisualization.currentExploringPort;
        if (portIndex < portCount)
        {
            float pulse = (sin(glowTime * 5.f) + 1.f) / 2.f;
            float outerRadius = 20.f + pulse * 10.f;
            sf::CircleShape outerGlow(outerRadius);
            outerGlow.setOrigin(outerRadius, outerRadius);
            outerGlow.setPosition(ports[portIndex].pixel);
            outerGlow.setFillColor(sf::Color(255, 200, 0, (sf::Uint8)(50 + pulse * 50)));
            outerGlow.setOutlineThickness(2.f);
            outerGlow.setOutlineColor(sf::Color(255, 150, 0, (sf::Uint8)(150 + pulse * 100)));
            window.draw(outerGlow);

            float innerRadius = 12.f + pulse * 4.f;
            sf::CircleShape innerPulse(innerRadius);
            innerPulse.setOrigin(innerRadius, innerRadius);
            innerPulse.setPosition(ports[portIndex].pixel);
            innerPulse.setFillColor(sf::Color(255, 220, 100, (sf::Uint8)(180 + pulse * 75)));
            window.draw(innerPulse);
        }
    }

    if (algorithmVisualization.isAnimating)
    {
        for (int i = 0; i < algorithmVisualization.currentStep && i < algorithmVisualization.explorationOrderCount; i++)
        {
            int portIndex = algorithmVisualization.explorationOrder[i];
            if (portIndex >= 0 && portIndex < portCount)
            {
                sf::CircleShape orderBg(8.f);
                orderBg.setOrigin(8.f, 8.f);
                orderBg.setPosition(ports[portIndex].pixel.x + 12.f, ports[portIndex].pixel.y - 12.f);
                orderBg.setFillColor(sf::Color(50, 50, 100, 200));
                orderBg.setOutlineThickness(1.f);
                orderBg.setOutlineColor(sf::Color(100, 100, 200));
                window.draw(orderBg);
            }
        }
    }

    if (algorithmVisualization.showFinalPath && algorithmVisualization.finalPathCount > 0)
    {
        for (int i = 0; i < algorithmVisualization.finalPathCount; i++)
        {
            int portIndex = algorithmVisualization.finalPath[i];
            if (portIndex < 0 || portIndex >= portCount) 
            {
                continue;
            }

            float pulse = (sin(glowTime * 2.f + i * 0.5f) + 1.f) / 2.f;
            float radius = 12.f + pulse * 4.f;

            sf::CircleShape pathNode(radius);
            pathNode.setOrigin(radius, radius);
            pathNode.setPosition(ports[portIndex].pixel);

            if (i == 0)
            {
                pathNode.setFillColor(sf::Color(0, 200, 255, (sf::Uint8)(200 + pulse * 55)));
                pathNode.setOutlineThickness(3.f);
                pathNode.setOutlineColor(sf::Color(0, 255, 255));
            }
            else if (i == algorithmVisualization.finalPathCount - 1) 
            {
                pathNode.setFillColor(sf::Color(0, 255, 100, (sf::Uint8)(200 + pulse * 55)));
                pathNode.setOutlineThickness(3.f);
                pathNode.setOutlineColor(sf::Color(100, 255, 150));
            }
            else 
            {
                pathNode.setFillColor(sf::Color(255, 200, 0, (sf::Uint8)(180 + pulse * 75)));
                pathNode.setOutlineThickness(2.f);
                pathNode.setOutlineColor(sf::Color(255, 150, 0));
            }

            window.draw(pathNode);
        }
    }
}

void drawPortDropdown(sf::RenderWindow& window, sf::Font& font, int portIndex, sf::Vector2i mousePos) {
    int out = 0;
    for (int i = 0; i < routeCount; ++i)
    {
        if (routes[i].valid && routes[i].originIndex == portIndex)
        {
            out++;
        }
    }
        
    float w = 320.f, lh = 20.f, hh = 25.f;
    int maxVis = 8;
    int vis = (out < maxVis) ? out : maxVis;
    float h = hh + (vis + 1) * lh + 14.f;

    sf::RectangleShape bg(sf::Vector2f(w, h));
    bg.setPosition((float)mousePos.x + 18.f, (float)mousePos.y + 18.f);
    bg.setFillColor(sf::Color(10, 20, 35, 240));
    bg.setOutlineThickness(2.f);
    bg.setOutlineColor(sf::Color(0, 200, 255));
    window.draw(bg);

    sf::Text header(ports[portIndex].name, font, 16);
    header.setFillColor(sf::Color(0, 220, 255));
    header.setStyle(sf::Text::Bold);
    header.setPosition(bg.getPosition().x + 10.f, bg.getPosition().y + 6.f);
    window.draw(header);

    sf::Text coords("", font, 12);
    coords.setString("Lat: " + intToString((int)ports[portIndex].lat) +
        ", Lon: " + intToString((int)ports[portIndex].lon));
    coords.setFillColor(sf::Color(190, 200, 210));
    coords.setPosition(bg.getPosition().x + 10.f, bg.getPosition().y + 28.f);
    window.draw(coords);

    sf::Text rHead("", font, 13);
    rHead.setString("Outgoing routes (" + intToString(out) + "):");
    rHead.setFillColor(sf::Color(150, 220, 255));
    rHead.setStyle(sf::Text::Bold);
    rHead.setPosition(bg.getPosition().x + 10.f, bg.getPosition().y + 46.f);
    window.draw(rHead);

    int shown = 0;
    float y = 64.f;
    for (int i = 0; i < routeCount && shown < maxVis; i++) 
    {
        if (routes[i].valid && routes[i].originIndex == portIndex) 
        {
            sf::Text t("", font, 11);
            string s = "-> " + ports[routes[i].destinationIndex].name +
                " | " + routes[i].date + " | $" + intToString(routes[i].vCost);
            t.setString(s);
            t.setFillColor(sf::Color(220, 220, 220));
            t.setPosition(bg.getPosition().x + 12.f, bg.getPosition().y + y);
            window.draw(t);
            y += lh;
            shown++;
        }
    }
    if (out > maxVis) 
    {
        sf::Text m("", font, 11);
        m.setString("...  and " + intToString(out - maxVis) + " more");
        m.setFillColor(sf::Color(150, 150, 150));
        m.setStyle(sf::Text::Italic);
        m.setPosition(bg.getPosition().x + 12.f, bg.getPosition().y + y);
        window.draw(m);
    }
}

bool routePassesPreferences(int routeIndex)
{
    if (routeIndex < 0 || routeIndex >= routeCount)
    {
        return false;
    }

    const Routes& r = routes[routeIndex];

    if (!sfmlPreferences.isCompanyPreferred(r.company))
    {
        return false;
    }

    if (sfmlPreferences.isPortAvoided(ports[r.destinationIndex].name))
    {
        return false;
    }
    if (sfmlPreferences.isPortAvoided(ports[r.originIndex].name))
    {
        return false;
    }
    return true;
}

bool findGraphicalItinerary(const std::string& src, const std::string& dst, const std::string& date) 
{
    visualLegCount = 0;
    int s = findPortIndexByName(src);
    int d = findPortIndexByName(dst);
    if (s == -1 || d == -1 || src == dst) 
    {
        return false;
    }

    if(sfmlPreferences.isPortAvoided(src) || sfmlPreferences.isPortAvoided(dst))
    {
        return false;
    }

    const int totalDepth = totalVisualLegs;
    const int totalNodesPerLevel = 200;

    struct Node 
    { 
        int portIndex; 
        int routeIndexFromParent; 
    };
    struct Parent 
    {
        int prevLevel; 
        int prevIndex; 
    };

    Node levelNodes[totalDepth + 1][totalNodesPerLevel];
    Parent parent[totalDepth + 1][totalNodesPerLevel];
    int levelCount[totalDepth + 1];

    levelNodes[0][0].portIndex = s;
    levelNodes[0][0].routeIndexFromParent = -1;
    parent[0][0].prevLevel = -1;
    parent[0][0].prevIndex = -1;
    levelCount[0] = 1;
    for (int i = 1; i <= totalDepth; i++) 
    {
        levelCount[i] = 0;
    }

    bool found = false;
    int fLevel = -1, fIndex = -1;

    for (int depth = 0; depth < totalDepth && !found; depth++) 
    {
        for (int index = 0; index < levelCount[depth] && !found; index++) 
        {
            int curPort = levelNodes[depth][index].portIndex;
            int inRoute = levelNodes[depth][index].routeIndexFromParent;

            for (int r = 0; r < routeCount && !found; r++) 
            {
                if (!routes[r].valid) 
                {
                    continue;
                }
                if (routes[r].originIndex != curPort) 
                {
                    continue;
                }
                if (!sfmlPreferences.isCompanyPreferred(routes[r].company))
                {
                    continue;
                }
                if (sfmlPreferences.isPortAvoided(ports[routes[r].destinationIndex].name))
                {
                    continue;
                }
                if (depth == 0 && routes[r].date != date) 
                {
                    continue;
                }
                if (depth > 0 && inRoute != -1) 
                {
                    if (!isLayoverFeasibleLocal(routes[inRoute], routes[r])) 
                    {
                        continue;
                    }
                }

                int nextPort = routes[r].destinationIndex;
                int& nextCount = levelCount[depth + 1];
                if (nextCount >= totalNodesPerLevel) 
                {
                    break;
                }

                levelNodes[depth + 1][nextCount].portIndex = nextPort;
                levelNodes[depth + 1][nextCount].routeIndexFromParent = r;
                parent[depth + 1][nextCount].prevLevel = depth;
                parent[depth + 1][nextCount].prevIndex = index;
                nextCount++;

                if (nextPort == d) 
                {
                    found = true;
                    fLevel = depth + 1;
                    fIndex = nextCount - 1;
                    break;
                }
            }
        }
    }

    if (!found) 
    {
        return false;
    }

    int routeIdx[totalVisualLegs];
    int len = 0;
    int cl = fLevel, ci = fIndex;
    while (cl > 0 && ci >= 0 && len < totalVisualLegs) 
    {
        int ri = levelNodes[cl][ci].routeIndexFromParent;
        routeIdx[len++] = ri;
        int nl = parent[cl][ci].prevLevel;
        int ni = parent[cl][ci].prevIndex;
        cl = nl; ci = ni;
    }

    for (int i = 0; i < len / 2; i++) 
    {
        int temp = routeIdx[i];
        routeIdx[i] = routeIdx[len - 1 - i];
        routeIdx[len - 1 - i] = temp;
    }

    visualLegCount = len;
    for (int i = 0; i < len; i++) 
    {
        selectedItinerary[i].routeIndex = routeIdx[i];
        if (i + 1 < len) 
        {
            selectedItinerary[i].layoverFeasible = isLayoverFeasibleLocal(routes[routeIdx[i]], routes[routeIdx[i + 1]]);
        }
        else 
        {
            selectedItinerary[i].layoverFeasible = true;
        }
    }
    return true;
}