#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "MenuButton.h"
#include "SFMLrun.h"
#include "graph.h"
#include "AlgorithmPanel.h"

using namespace std;

enum Page { MENU, MAP };

int main()
{
    Graph g;
    Page currentPage = MENU;

    const string routeFile = "routes.txt";
    const string mapFile = "world_map.png";
    const string menuBG = "menuBG.png";

    sf::VideoMode desktop = getDesktopMode();
    unsigned int windowWidth = static_cast<unsigned int>(desktop.width * 0.9f);
    unsigned int windowHeight = static_cast<unsigned int>(desktop.height * 0.9f);

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
    font.loadFromFile("Sansation_Regular.ttf");

    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float centerX = (windowWidth - buttonWidth) / 2.f;
    float centerY = windowHeight * 0.45f;

    sf::Font menuTitleFont;
    menuTitleFont.loadFromFile("Navigator.ttf");
    sf::Text menuTitle;
    menuTitle.setFont(menuTitleFont);
    menuTitle.setString("OCEANROUTE NAVIGATION SYSTEM");
    menuTitle.setCharacterSize(100);
    menuTitle.setFillColor(sf::Color::White);
    menuTitle.setStyle(sf::Text::Bold);
    menuTitle.setPosition(windowWidth / 2.f - menuTitle.getLocalBounds().width / 2.f, 150.f);


    sf::Text menuSubHeading;
    menuSubHeading.setFont(font);
    menuSubHeading.setString("Maritime Navigation Optimizer");
    menuSubHeading.setCharacterSize(50);
    menuSubHeading.setFillColor(sf::Color::White);
    menuSubHeading.setPosition(windowWidth / 2.f - menuSubHeading.getLocalBounds().width / 2.f, 350.f);
    
    Button mapBtn(centerX, centerY + 70.f, buttonWidth, buttonHeight, "START", menuTitleFont, sf::Color(0, 120, 215), sf::Color::White);
    float shipSpeedMultiplier = 1.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (currentPage == MENU)
        {
            window.clear();
            window.draw(menuSprite);
            window.draw(menuTitle);
            window.draw(menuSubHeading);
            mapBtn.draw(window);


            static float memberAnimPhase = 0.f;
            memberAnimPhase += 0.016f * 2.f;
            if (memberAnimPhase > 6.28318f) memberAnimPhase -= 6.28318f;

            float pulse = (sin(memberAnimPhase) + 1.f) / 2.f;

            float panelWidth = 320.f;
            float panelHeight = 70.f;
            float panelX = (windowWidth - panelWidth) / 2.f;
            float panelY = windowHeight - panelHeight - 25.f;

            sf::RectangleShape membersPanel(sf::Vector2f(panelWidth, panelHeight));
            membersPanel.setPosition(panelX, panelY);
            membersPanel.setFillColor(sf::Color(10, 20, 40, 220));
            membersPanel.setOutlineThickness(1.f);
            membersPanel.setOutlineColor(sf::Color(0, 150, 200, (sf::Uint8)(100 + pulse * 80)));
            window.draw(membersPanel);

            sf::RectangleShape topLine(sf::Vector2f(panelWidth - 20.f, 2.f));
            topLine.setPosition(panelX + 10.f, panelY);
            topLine.setFillColor(sf::Color(0, 180, 255, (sf::Uint8)(150 + pulse * 80)));
            window.draw(topLine);

            sf::Text devLabel;
            devLabel.setFont(font);
            devLabel.setString("Developed By");
            devLabel.setCharacterSize(16);
            devLabel.setFillColor(sf::Color(0, 180, 220));
            devLabel.setStyle(sf::Text::Bold);
            sf::FloatRect devBounds = devLabel.getLocalBounds();
            devLabel.setPosition(panelX + (panelWidth - devBounds.width) / 2.f, panelY + 8.f);
            window.draw(devLabel);


            sf::Text member1;
            member1.setFont(font);
            member1.setString("Shayan Tariq            (24I-3024)");
            member1.setCharacterSize(20);
            member1.setFillColor(sf::Color(220, 235, 255));
            sf::FloatRect m1Bounds = member1.getLocalBounds();
            member1.setPosition(panelX + (panelWidth - m1Bounds.width) / 2.f, panelY + 26.f);
            window.draw(member1);

            sf::Text member2;
            member2.setFont(font);
            member2.setString("Mahrukh Masroor   (24I-0778)");
            member2.setCharacterSize(20);
            member2.setFillColor(sf::Color(220, 235, 255));
            sf::FloatRect m2Bounds = member2.getLocalBounds();
            member2.setPosition(panelX + (panelWidth - m2Bounds.width) / 2.f, panelY + 46.f);
            window.draw(member2);

            float dotRadius = 3.f;
            sf::CircleShape cornerDot(dotRadius);
            cornerDot.setOrigin(dotRadius, dotRadius);
            cornerDot.setFillColor(sf::Color(0, 200, 255, (sf::Uint8)(150 + pulse * 100)));

            cornerDot.setPosition(panelX + 8.f, panelY + 8.f);
            window.draw(cornerDot);
            cornerDot.setPosition(panelX + panelWidth - 8.f, panelY + 8.f);
            window.draw(cornerDot);
            cornerDot.setPosition(panelX + 8.f, panelY + panelHeight - 8.f);
            window.draw(cornerDot);
            cornerDot.setPosition(panelX + panelWidth - 8.f, panelY + panelHeight - 8.f);
            window.draw(cornerDot);


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (mapBtn.isMouseOver(mousePos)) currentPage = MAP;
            }

            window.display();
            continue;
        }

        if (currentPage == MAP)
        {
            loadPorts();
            loadOceanWaypoints();
            if (!loadRoutes(routeFile))
            {
                return 1;
            }

            g.loadFile("Routes.txt");

            sf::Texture mapTexture;
            if (!mapTexture.loadFromFile(mapFile))
            {
                cout << "Failed to load map: " << mapFile << endl;
                return 1;
            }

            sf::Sprite mapSprite(mapTexture);
            sf::Vector2u mapSize = mapTexture.getSize();

            SimpleControlPanel controlPanel;
            BookingPanel bookingPanel;
            OptimizationPanel optimizationPanel;
            SubgraphPanel subgraphPanel;

            controlPanel.initialize((float)windowWidth, (float)windowHeight, font);
            bookingPanel.initialize((float)windowWidth, (float)windowHeight, font);
            optimizationPanel.initialize((float)windowWidth, (float)windowHeight, font);
            subgraphPanel.initialize((float)windowWidth, (float)windowHeight, font);
            preferencesPanel.initialize((float)windowWidth, (float)windowHeight, font);
            multiLegBuilderPanel.initialize((float)windowWidth, (float)windowHeight, font);
            dockQueuePanel.initialize((float)windowWidth, (float)windowHeight, font);

            float panelWidth = controlPanel.getPanelWidth();

            shipAnimation.initialize(font);

            SimulateButton simBtn;
            simBtn.initialize(panelWidth + 20.f, 8.f, font);

            float mapAreaWidth = windowWidth - panelWidth;

            for (int i = 0; i < portCount; i++)
            {
                ports[i].pixel = latlonToPixel(ports[i].lat, ports[i].lon, mapSize);
            }

            for (int i = 0; i < waypointCount; i++)
            {
                oceanWaypoints[i].pixel = latlonToPixel(oceanWaypoints[i].lat, oceanWaypoints[i].lon, mapSize);
            }

            float scaleX = mapAreaWidth / mapSize.x;
            float scaleY = float(windowHeight) / mapSize.y;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            mapSprite.setScale(scale, scale);
            mapSprite.setPosition(panelWidth, 0);

            for (int i = 0; i < portCount; i++)
            {
                ports[i].pixel.x = ports[i].pixel.x * scale + panelWidth;
                ports[i].pixel.y *= scale;
            }
            for (int i = 0; i < waypointCount; ++i)
            {
                oceanWaypoints[i].pixel.x = oceanWaypoints[i].pixel.x * scale + panelWidth;
                oceanWaypoints[i].pixel.y *= scale;
            }

            sf::View view = window.getDefaultView();
            float zoom = 1.f;

            sf::CircleShape portShape(portRadius);
            portShape.setOrigin(portRadius, portRadius);
            portShape.setFillColor(sf::Color::White);
            portShape.setOutlineThickness(1);
            portShape.setOutlineColor(sf::Color(120, 120, 120));

            sf::CircleShape portHighlight(highlightRadius);
            portHighlight.setOrigin(highlightRadius, highlightRadius);
            portHighlight.setOutlineThickness(2);
            portHighlight.setOutlineColor(sf::Color::Yellow);
            portHighlight.setFillColor(sf::Color::Transparent);

            bool dragging = false;
            sf::Vector2i lastMouse;
            int hoveredPort = -1;
            int hoveredRoute = -1;
            int selectedRoute = -1;

            sf::Clock animClock;
            shipSpeedMultiplier = 1.f;

            while (window.isOpen() && currentPage == MAP)
            {
                float deltaTime = animClock.restart().asSeconds();

                sf::Event e;
                while (window.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed)
                    {
                        window.close();
                    }

                    if (bookingPanel.isVisible)
                    {
                        if (e.type == sf::Event::TextEntered)
                        {
                            bookingPanel.handleTextInput(e.text.unicode);
                        }

                        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                            int result = bookingPanel.handleClick(mousePos);

                            if (result == 0)
                            {
                                bookingPanel.hide();
                            }
                            else if (result == 1)
                            {
                                string src = bookingPanel.getSource();
                                string dst = bookingPanel.getDestination();
                                string date = bookingPanel.getDate();
                                string company = bookingPanel.getCompany();
                                string maxTime = bookingPanel.getMaxTime();

                                if (src.empty() || dst.empty())
                                {
                                    controlPanel.setStatus("Enter source & destination!", sf::Color(255, 100, 100));
                                }
                                else
                                {
                                    if (!company.empty())
                                    {
                                        sfmlPreferences.clear();
                                        sfmlPreferences.addPreferredCompany(company);
                                    }
                                    if (!maxTime.empty())
                                    {
                                        int maxMins = 0;
                                        for (int i = 0; i < maxTime.length(); i++)
                                        {
                                            if (maxTime[i] >= '0' && maxTime[i] <= '9')
                                                maxMins = maxMins * 10 + (maxTime[i] - '0');
                                        }
                                        sfmlPreferences.setMaxTime(maxMins);
                                    }

                                    clearAlgorithmVisualization();
                                    shipAnimation.stopAnimation();
                                    bool found = findGraphicalItinerary(src, dst, date);

                                    if (found)
                                    {
                                        controlPanel.setStatus("Route found: " + intToString(visualLegCount) + " legs", sf::Color(100, 255, 100));
                                        controlPanel.setResults("Booking: " + src + " -> " + dst + "\nDate: " + date + "\nLegs: " + intToString(visualLegCount));

                                        int destPortIdx = findPortIndexByName(dst);
                                        if (destPortIdx != -1)
                                        {
                                            dockQueuePanel.setPort(destPortIdx, dst);
                                        }
                                    }
                                    else
                                    {
                                        controlPanel.setStatus("No route found", sf::Color(255, 150, 50));
                                    }
                                    bookingPanel.hide();
                                }
                            }
                        }
                        continue;
                    }

                    if (optimizationPanel.isVisible)
                    {
                        if (e.type == sf::Event::TextEntered)
                        {
                            optimizationPanel.handleTextInput(e.text.unicode);
                        }

                        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                            int result = optimizationPanel.handleClick(mousePos);

                            string src = optimizationPanel.getSource();
                            string dst = optimizationPanel.getDestination();

                            if (result == 0)
                            {
                                optimizationPanel.hide();
                            }
                            else if (result >= 1 && result <= 4)
                            {
                                if (src.empty() || dst.empty())
                                {
                                    controlPanel.setStatus("Enter source & destination!", sf::Color(255, 100, 100));
                                }
                                else
                                {
                                    visualLegCount = 0;
                                    clearAlgorithmVisualization();
                                    shipAnimation.stopAnimation();

                                    if (result == 1) 
                                    {
                                        g.dijkstraMinCost(src, dst);
                                    }
                                    else if (result == 2) 
                                    {
                                        g.dijkstraMinTime(src, dst);
                                    }
                                    else if (result == 3) 
                                    {
                                        g.aStarMinCost(src, dst);
                                    }
                                    else if (result == 4) 
                                    {
                                        g.aStarMinTime(src, dst);
                                    }

                                    if (algorithmLegCount > 0)
                                    {
                                        string algoName = (result <= 2) ? "Dijkstra" : "A*";
                                        string metric = (result == 1 || result == 3) ? "Cost: $" : "Time: ";
                                        string unit = (result == 1 || result == 3) ? "" : " mins";

                                        controlPanel.setStatus(algoName + " " + metric + intToString(algorithmTotalValue) + unit, sf::Color(100, 255, 100));
                                        controlPanel.setResults(algoName + "\n" + src + " -> " + dst + "\n" + metric + intToString(algorithmTotalValue) + unit);

                                        int destPortIdx = findPortIndexByName(dst);
                                        if (destPortIdx != -1)
                                        {
                                            dockQueuePanel.setPort(destPortIdx, dst);
                                        }
                                    }
                                    else
                                    {
                                        controlPanel.setStatus("No path found", sf::Color(255, 150, 50));
                                    }
                                    optimizationPanel.hide();
                                }
                            }
                        }
                        continue;
                    }

                    if (subgraphPanel.isVisible)
                    {
                        if (e.type == sf::Event::TextEntered)
                        {
                            subgraphPanel.handleTextInput(e.text.unicode);
                        }

                        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                            int result = subgraphPanel.handleClick(mousePos);

                            if (result == 0)
                            {
                                subgraphPanel.hide();
                            }
                            else if (result == 1)
                            {
                                string company = subgraphPanel.getCompany();

                                if (company.empty())
                                {
                                    controlPanel.setStatus("Enter company name!", sf::Color(255, 100, 100));
                                }
                                else
                                {
                                    sfmlPreferences.clear();
                                    sfmlPreferences.addPreferredCompany(company);

                                    controlPanel.setStatus("Subgraph: " + company, sf::Color(100, 255, 100));
                                    controlPanel.setResults("Subgraph generated\nfor: " + company + "\n\nRoutes filtered on map");
                                    subgraphPanel.hide();
                                }
                            }
                        }
                        continue;
                    }

                    if (preferencesPanel.visible())
                    {
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                            preferencesPanel.handleClick(mousePos);
                        }

                        if (e.type == sf::Event::TextEntered)
                        {
                            preferencesPanel.handleTextInput(e.text.unicode);
                        }
                        continue;
                    }

                    if (multiLegBuilderPanel.visible())
                    {
                        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                            multiLegBuilderPanel.handleClick(mousePos);
                        }

                        if (e.type == sf::Event::TextEntered)
                        {
                            multiLegBuilderPanel.handleTextInput(e.text.unicode);
                        }
                        continue;
                    }

                    if (e.type == sf::Event::KeyPressed)
                    {
                        if (e.key.code == sf::Keyboard::Escape)
                        {
                            currentPage = MENU;
                        }
                        if (e.key.code == sf::Keyboard::Space)
                        {
                            if (algorithmVisualization.isAnimating)
                            {
                                algorithmVisualization.skipToEnd();
                            }
                            else
                            {
                                shipAnimation.togglePause();
                            }
                        }
                        if (e.key.code == sf::Keyboard::S)
                        {
                            shipAnimation.stopAnimation();
                            controlPanel.setStatus("Simulation stopped", sf::Color(150, 150, 150));
                        }
                        if (e.key.code == sf::Keyboard::Add || e.key.code == sf::Keyboard::Equal)
                        {
                            shipSpeedMultiplier = std::min(shipSpeedMultiplier + 0.25f, 3.f);
                            shipAnimation.setSpeed(shipSpeedMultiplier);
                            controlPanel.setStatus("Speed: " + intToString((int)(shipSpeedMultiplier * 100)) + "%", sf::Color(100, 255, 100));
                        }
                        if (e.key.code == sf::Keyboard::Subtract || e.key.code == sf::Keyboard::Dash)
                        {
                            shipSpeedMultiplier = std::max(shipSpeedMultiplier - 0.25f, 0.25f);
                            shipAnimation.setSpeed(shipSpeedMultiplier);
                            controlPanel.setStatus("Speed: " + intToString((int)(shipSpeedMultiplier * 100)) + "%", sf::Color(100, 255, 100));
                        }
                    }

                    if (e.type == sf::Event::MouseWheelScrolled)
                    {
                        zoom *= (e.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                        zoom = std::max(0.1f, std::min(zoom, 10.f));
                        view.setSize(window.getDefaultView().getSize().x * zoom,
                            window.getDefaultView().getSize().y * zoom);
                    }

                    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f guiPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

                        if (simBtn.isMouseOver(guiPos))
                        {
                            if (algorithmLegCount > 0)
                            {
                                shipAnimation.startAnimation(algorithmLegCount);
                                controlPanel.setStatus("Simulation started", sf::Color(100, 255, 100));
                            }
                            else if (visualLegCount > 0)
                            {
                                shipAnimation.startAnimation(visualLegCount);
                                controlPanel.setStatus("Simulation started", sf::Color(100, 255, 100));
                            }
                            else
                            {
                                controlPanel.setStatus("Find a route first!", sf::Color(255, 100, 100));
                            }
                            continue;
                        }

                        if (guiPos.x < panelWidth)
                        {
                            int action = controlPanel.handleClick(guiPos);

                            switch (action)
                            {
                            case 1:
                                bookingPanel.show();
                                break;

                            case 2:
                                optimizationPanel.show();
                                break;

                            case 3:
                                multiLegBuilderPanel.show();
                                break;

                            case 4:
                                subgraphPanel.show();
                                break;

                            case 5:
                                visualLegCount = 0;
                                clearAlgorithmVisualization();
                                algorithmVisualization.clear();
                                shipAnimation.stopAnimation();
                                dockQueuePanel.setPort(-1, "");
                                sfmlPreferences.clear();
                                bookingPanel.clear();
                                optimizationPanel.clear();
                                subgraphPanel.clear();
                                controlPanel.setStatus("Cleared", sf::Color(150, 150, 150));
                                controlPanel.setResults("No route selected\n\nSelect an option above");
                                break;

                            default:
                                break;
                            }
                        }
                        else
                        {
                            dragging = true;
                            lastMouse = sf::Mouse::getPosition(window);
                        }
                    }

                    if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
                    {
                        dragging = false;

                        sf::Vector2f guiPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

                        if (guiPos.x > panelWidth)
                        {
                            if (hoveredRoute != -1)
                            {
                                if (selectedRoute == hoveredRoute)
                                {
                                    selectedRoute = -1;
                                }
                                else
                                {
                                    selectedRoute = hoveredRoute;
                                }
                            }
                            else if (hoveredPort == -1)
                            {
                                selectedRoute = -1;
                            }
                        }
                    }

                    if (e.type == sf::Event::MouseMoved && dragging)
                    {
                        sf::Vector2i now = sf::Mouse::getPosition(window);
                        sf::Vector2f delta = window.mapPixelToCoords(lastMouse, view) - window.mapPixelToCoords(now, view);
                        view.move(delta);
                        lastMouse = now;
                    }
                }

                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

                controlPanel.update(deltaTime);
                controlPanel.updateHover(mousePos, deltaTime);

                bookingPanel.update(deltaTime);
                bookingPanel.updateHover(mousePos, deltaTime);

                optimizationPanel.update(deltaTime);
                optimizationPanel.updateHover(mousePos, deltaTime);

                subgraphPanel.update(deltaTime);
                subgraphPanel.updateHover(mousePos, deltaTime);  
                shipAnimation.update(deltaTime);
                preferencesPanel.update(deltaTime);
                multiLegBuilderPanel.update(deltaTime);
                multiLegBuilderPanel.updateHover(mousePos, deltaTime);
                dockQueuePanel.update(deltaTime);
                algorithmVisualization.update(deltaTime);
                sf::Vector2f guiMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
                simBtn.update(deltaTime, guiMousePos);

                sf::Vector2i mousePix = sf::Mouse::getPosition(window);
                sf::Vector2f world = window.mapPixelToCoords(mousePix, view);

                hoveredPort = -1;
                hoveredRoute = -1;

                if (mousePix.x > panelWidth)
                {
                    for (int i = 0; i < portCount; i++)
                    {
                        float dx = ports[i].pixel.x - world.x;
                        float dy = ports[i].pixel.y - world.y;
                        if (sqrt(dx * dx + dy * dy) <= 10.f)
                        {
                            hoveredPort = i;
                            break;
                        }
                    }

                    for (int i = 0; i < routeCount; ++i)
                    {
                        if (!routes[i].valid)
                        {
                            continue;
                        }

                        sf::Vector2f a = ports[routes[i].originIndex].pixel;
                        sf::Vector2f b = ports[routes[i].destinationIndex].pixel;
                        sf::Vector2f ap = world - a;
                        sf::Vector2f ab = b - a;
                        float ab2 = ab.x * ab.x + ab.y * ab.y;
                        if (ab2 == 0)
                        {
                            continue;
                        }

                        float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
                        t = std::max(0.f, std::min(t, 1.f));
                        sf::Vector2f proj = a + ab * t;
                        float dist = sqrt((proj.x - world.x) * (proj.x - world.x) + (proj.y - world.y) * (proj.y - world.y));
                        if (dist <= 8.f)
                        {
                            hoveredRoute = i;
                            break;
                        }
                    }
                }

                window.clear(sf::Color(8, 15, 30));
                window.setView(view);
                window.draw(mapSprite);
                static float algoGlowTime = 0.f;
                algoGlowTime += deltaTime * 3.f;
                string preferredCompany = subgraphPanel.getCompany();
                string avoidPort = subgraphPanel.getAvoidPort();

                for (int i = 0; i < routeCount; i++)
                {
                    if (!routes[i].valid) 
                    {
                        continue;
                    }

                    sf::Vector2f a = ports[routes[i].originIndex].pixel;
                    sf::Vector2f b = ports[routes[i].destinationIndex].pixel;

                    bool isSel = (i == selectedRoute);
                    bool isHover = (i == hoveredRoute);
                    bool fromHover = (hoveredPort == routes[i].originIndex);

                    bool passesPreferences = routePassesPreferences(i);
                    bool hasActivePreferences = sfmlPreferences.usePreferredCompanies || sfmlPreferences.useAvoidedPorts || sfmlPreferences.useTotalTime;

                    bool isPreferredCompany = sfmlPreferences.usePreferredCompanies &&
                        sfmlPreferences.isCompanyPreferred(routes[i].company);

                    bool isSubgraphPreferred = false;
                    bool isAvoidedPort = false;

                    if (!preferredCompany.empty())
                    {
                        string companyLower = routes[i].company;
                        string prefLower = preferredCompany;

                        for (int j = 0; j < companyLower.length(); j++)
                        {
                            if (companyLower[j] >= 'A' && companyLower[j] <= 'Z')
                            {
                                companyLower[j] = companyLower[j] + 32;
                            }
                        }
                        for (int j = 0; j < prefLower.length(); j++)
                        {
                            if (prefLower[j] >= 'A' && prefLower[j] <= 'Z')
                            {
                                prefLower[j] = prefLower[j] + 32;
                            }
                        }

                        isSubgraphPreferred = (companyLower.find(prefLower) != string::npos);
                    }

                    if (!avoidPort.empty())
                    {
                        string avoidLower = avoidPort;
                        string originLower = ports[routes[i].originIndex].name;
                        string destLower = ports[routes[i].destinationIndex].name;

                        for (int j = 0; j < avoidLower.length(); j++)
                        {
                            if (avoidLower[j] >= 'A' && avoidLower[j] <= 'Z')
                            {
                                avoidLower[j] = avoidLower[j] + 32;
                            }
                        }

                        for (int j = 0; j < originLower.length(); j++)
                        {
                            if (originLower[j] >= 'A' && originLower[j] <= 'Z')
                            {
                                originLower[j] = originLower[j] + 32;
                            }
                        }

                        for (int j = 0; j < destLower.length(); j++)
                        {
                            if (destLower[j] >= 'A' && destLower[j] <= 'Z')
                            {
                                destLower[j] = destLower[j] + 32;
                            }
                        }

                        isAvoidedPort = (originLower.find(avoidLower) != string::npos ||
                            destLower.find(avoidLower) != string::npos);
                    }

                    sf::Color lineColor;
                    float thickness;

                    if (isAvoidedPort)
                    {
                        lineColor = sf::Color(40, 40, 50, 20);
                        thickness = 0.3f;
                    }

                    else if (isSel)
                    {
                        lineColor = sf::Color::Cyan;
                        thickness = 3.0f;
                    }

                    else if (isHover)
                    {
                        lineColor = sf::Color::Yellow;
                        thickness = 3.0f;
                    }

                    else if (fromHover)
                    {
                        lineColor = sf::Color(255, 140, 0);
                        thickness = 2.0f;
                    }

                    else if (isSubgraphPreferred)
                    {
                        float pulse = (sin(algoGlowTime * 2.f + i * 0.1f) + 1.f) / 2.f;
                        lineColor = sf::Color(
                            (sf::Uint8)(0 + pulse * 50),
                            (sf::Uint8)(200 + pulse * 55),
                            (sf::Uint8)(100 + pulse * 100),
                            250
                        );
                        thickness = 2.5f + pulse * 0.5f;
                    }

                    else if (isPreferredCompany)
                    {
                        float pulse = (sin(algoGlowTime * 2.f + i * 0.1f) + 1.f) / 2.f;
                        lineColor = sf::Color(
                            (sf::Uint8)(0 + pulse * 50),
                            (sf::Uint8)(200 + pulse * 55),
                            (sf::Uint8)(100 + pulse * 100),
                            250
                        );
                        thickness = 2.5f + pulse * 0.5f;
                    }

                    else if (hasActivePreferences && passesPreferences)
                    {
                        lineColor = sf::Color(0, 255, 200, 150);
                        thickness = 1.5f;
                    }
                    else if (hasActivePreferences && !passesPreferences)
                    {
                        lineColor = sf::Color(50, 50, 60, 25);
                        thickness = 0.4f;
                    }
                    else if (!preferredCompany.empty() && !isSubgraphPreferred)
                    {
                        lineColor = sf::Color(50, 50, 60, 40);
                        thickness = 0.5f;
                    }
                    else
                    {
                        lineColor = sf::Color(60, 80, 100, 60);
                        thickness = 0.7f;
                    }

                    sf::Vector2f pathPoints[totalWayPoints];
                    int pathLength = 0;
                    generateOceanPath(a, b, routes[i].originIndex, routes[i].destinationIndex, pathPoints, pathLength);

                    if ((isPreferredCompany || isSubgraphPreferred) && !isSel && !isHover && !isAvoidedPort)
                    {
                        float pulse = (sin(algoGlowTime * 2.f + i * 0.1f) + 1.f) / 2.f;
                        sf::Color glowColor(0, 255, 150, (sf::Uint8)(40 + pulse * 40));
                        drawOceanRoute(window, pathPoints, pathLength, glowColor, thickness + 4.f);
                    }
                    drawOceanRoute(window, pathPoints, pathLength, lineColor, thickness);
                }

                if (algorithmVisualization.exploredCount > 0)
                {
                    drawAlgorithmExploration(window, algoGlowTime);
                }

                if (visualLegCount > 0)
                {
                    for (int index = 0; index < visualLegCount; index++)
                    {
                        const Routes& r = routes[selectedItinerary[index].routeIndex];
                        sf::Vector2f a = ports[r.originIndex].pixel;
                        sf::Vector2f b = ports[r.destinationIndex].pixel;

                        sf::Vector2f pathPoints[totalWayPoints];
                        int pathLength = 0;
                        generateOceanPath(a, b, r.originIndex, r.destinationIndex, pathPoints, pathLength);
                        drawOceanRoute(window, pathPoints, pathLength, sf::Color(0, 255, 255), 4.f);
                    }

                    sf::CircleShape layoverCircle(9.f);
                    layoverCircle.setOrigin(9.f, 9.f);
                    layoverCircle.setOutlineThickness(2.f);

                    for (int index = 0; index + 1 < visualLegCount; index++)
                    {
                        const Routes& r = routes[selectedItinerary[index].routeIndex];
                        int layoverPortIdx = r.destinationIndex;
                        layoverCircle.setPosition(ports[layoverPortIdx].pixel);

                        if (selectedItinerary[index].layoverFeasible)
                        {
                            layoverCircle.setFillColor(sf::Color(0, 150, 0, 90));
                            layoverCircle.setOutlineColor(sf::Color(0, 255, 0));
                        }
                        else
                        {
                            layoverCircle.setFillColor(sf::Color(150, 0, 0, 90));
                            layoverCircle.setOutlineColor(sf::Color(255, 0, 0));
                        }
                        window.draw(layoverCircle);
                    }
                }

                static float glowTime = 0.f;
                glowTime += deltaTime * 3.f;

                if (algorithmLegCount > 0)
                {
                    for (int index = 0; index < algorithmLegCount; index++)
                    {
                        int fromIdx = algorithmPart[index].fromPortIndex;
                        int toIdx = algorithmPart[index].toPortIndex;

                        sf::Vector2f a = ports[fromIdx].pixel;
                        sf::Vector2f b = ports[toIdx].pixel;

                        sf::Vector2f pathPoints[totalWayPoints];
                        int pathLength = 0;
                        generateOceanPath(a, b, fromIdx, toIdx, pathPoints, pathLength);

                        float t = float(index) / std::max(1, algorithmLegCount - 1);
                        float glow = (sin(glowTime + index * 0.5f) + 1.f) / 2.f;

                        int rVal = std::min(255, std::max(0, (int)(255 - t * 155 + glow * 30)));
                        int gVal = std::min(255, std::max(0, (int)(t * 255 + glow * 30)));
                        int bVal = 255;

                        sf::Color gradColor(rVal, gVal, bVal);
                        drawOceanRoute(window, pathPoints, pathLength, gradColor, 5.f);
                    }

                    for (int index = 0; index < algorithmLegCount; index++)
                    {
                        int fromIdx = algorithmPart[index].fromPortIndex;
                        float pulse = (sin(glowTime * 2.f + index * 0.8f) + 1.f) / 2.f;
                        float radius = 10.f + pulse * 4.f;

                        sf::CircleShape nodeCircle(radius);
                        nodeCircle.setOrigin(radius, radius);
                        nodeCircle.setFillColor(sf::Color(255, 200, 0, (sf::Uint8)(180 + pulse * 75)));
                        nodeCircle.setOutlineThickness(2.f + pulse);
                        nodeCircle.setOutlineColor(sf::Color(255, 100, 0));
                        nodeCircle.setPosition(ports[fromIdx].pixel);
                        window.draw(nodeCircle);
                    }

                    int lastToIdx = algorithmPart[algorithmLegCount - 1].toPortIndex;
                    float pulse = (sin(glowTime * 2.5f) + 1.f) / 2.f;
                    float radius = 14.f + pulse * 5.f;
                    sf::CircleShape destCircle(radius);
                    destCircle.setOrigin(radius, radius);
                    destCircle.setFillColor(sf::Color(0, 255, 100, (sf::Uint8)(200 + pulse * 55)));
                    destCircle.setOutlineThickness(3.f);
                    destCircle.setOutlineColor(sf::Color(0, 200, 50));
                    destCircle.setPosition(ports[lastToIdx].pixel);
                    window.draw(destCircle);
                }

                if (multiLegRoute.getCount() >= 2)
                {
                    LegNode* current = multiLegRoute.getHead();

                    while (current != nullptr && current->next != nullptr)
                    {
                        sf::Vector2f startPos = ports[current->portIndex].pixel;
                        sf::Vector2f endPos = ports[current->next->portIndex].pixel;

                        sf::Vector2f pathPoints[totalWayPoints];
                        int pathLength = 0;
                        generateOceanPath(startPos, endPos, current->portIndex, current->next->portIndex, pathPoints, pathLength);

                        sf::Color routeColor = (current->routeToNextIndex != -1) ? sf::Color(0, 255, 255, 220) : sf::Color(255, 150, 0, 180);
                        float thickness = (current->routeToNextIndex != -1) ? 3.f : 2.f;

                        drawOceanRoute(window, pathPoints, pathLength, routeColor, thickness);
                        current = current->next;
                    }

                    current = multiLegRoute.getHead();
                    int nodeNum = 1;
                    while (current != nullptr)
                    {
                        sf::Vector2f pos = ports[current->portIndex].pixel;

                        sf::CircleShape glow(portRadius + 8.f);
                        glow.setOrigin(portRadius + 8.f, portRadius + 8.f);
                        glow.setPosition(pos);
                        glow.setFillColor(sf::Color(0, 255, 200, 60));
                        window.draw(glow);

                        sf::CircleShape marker(portRadius + 4.f);
                        marker.setOrigin(portRadius + 4.f, portRadius + 4.f);
                        marker.setPosition(pos);
                        marker.setFillColor(sf::Color(0, 200, 255));
                        marker.setOutlineThickness(2.f);
                        marker.setOutlineColor(sf::Color::White);
                        window.draw(marker);

                        sf::Text numText(intToString(nodeNum), font, 10);
                        numText.setFillColor(sf::Color::White);
                        numText.setStyle(sf::Text::Bold);
                        numText.setPosition(pos.x - 4.f, pos.y - 6.f);
                        window.draw(numText);

                        current = current->next;
                        nodeNum++;
                    }
                }
                    
                for (int i = 0; i < portCount; i++)
                {
                    if (sfmlPreferences.isPortAvoided(ports[i].name))
                    {
                        sf::CircleShape avoidedCircle(portRadius + 5.f);
                        avoidedCircle.setOrigin(portRadius + 5.f, portRadius + 5.f);
                        avoidedCircle.setPosition(ports[i].pixel);
                        avoidedCircle.setFillColor(sf::Color(255, 0, 0, 60));
                        avoidedCircle.setOutlineThickness(2.f);
                        avoidedCircle.setOutlineColor(sf::Color(255, 50, 50, 200));
                        window.draw(avoidedCircle);

                        float xSize = 6.f;
                        sf::RectangleShape line1(sf::Vector2f(xSize * 2, 2.f));
                        line1.setOrigin(xSize, 1.f);
                        line1.setPosition(ports[i].pixel);
                        line1.setRotation(45.f);
                        line1.setFillColor(sf::Color(255, 100, 100));
                        window.draw(line1);

                        sf::RectangleShape line2(sf::Vector2f(xSize * 2, 2.f));
                        line2.setOrigin(xSize, 1.f);
                        line2.setPosition(ports[i].pixel);
                        line2.setRotation(-45.f);
                        line2.setFillColor(sf::Color(255, 100, 100));
                        window.draw(line2);
                    }
                }

                if (!avoidPort.empty())
                {
                    for (int i = 0; i < portCount; i++)
                    {
                        string portNameLower = ports[i].name;
                        string avoidLower = avoidPort;

                        for (int j = 0; j < portNameLower.length(); j++)
                        {
                            if (portNameLower[j] >= 'A' && portNameLower[j] <= 'Z')
                            {
                                portNameLower[j] = portNameLower[j] + 32;
                            }
                        }
                        for (int j = 0; j < avoidLower.length(); j++)
                        {
                            if (avoidLower[j] >= 'A' && avoidLower[j] <= 'Z')
                            {
                                avoidLower[j] = avoidLower[j] + 32;
                            }
                        }

                        if (portNameLower.find(avoidLower) != string::npos)
                        {
                            sf::CircleShape avoidedCircle(portRadius + 5.f);
                            avoidedCircle.setOrigin(portRadius + 5.f, portRadius + 5.f);
                            avoidedCircle.setPosition(ports[i].pixel);
                            avoidedCircle.setFillColor(sf::Color(255, 0, 0, 60));
                            avoidedCircle.setOutlineThickness(2.f);
                            avoidedCircle.setOutlineColor(sf::Color(255, 50, 50, 200));
                            window.draw(avoidedCircle);

                            float xSize = 6.f;
                            sf::RectangleShape line1(sf::Vector2f(xSize * 2, 2.f));
                            line1.setOrigin(xSize, 1.f);
                            line1.setPosition(ports[i].pixel);
                            line1.setRotation(45.f);
                            line1.setFillColor(sf::Color(255, 100, 100));
                            window.draw(line1);

                            sf::RectangleShape line2(sf::Vector2f(xSize * 2, 2.f));
                            line2.setOrigin(xSize, 1.f);
                            line2.setPosition(ports[i].pixel);
                            line2.setRotation(-45.f);
                            line2.setFillColor(sf::Color(255, 100, 100));
                            window.draw(line2);
                        }
                    }
                }

                if (sfmlPreferences.usePreferredCompanies)
                {
                    for (int i = 0; i < portCount; ++i)
                    {
                        bool hasPreferredRoute = false;
                        int preferredRouteCount = 0;

                        for (int r = 0; r < routeCount; ++r)
                        {
                            if (routes[r].valid && routes[r].originIndex == i)
                            {
                                if (sfmlPreferences.isCompanyPreferred(routes[r].company))
                                {
                                    hasPreferredRoute = true;
                                    preferredRouteCount++;
                                }
                            }
                        }

                        if (hasPreferredRoute && !sfmlPreferences.isPortAvoided(ports[i].name))
                        {
                            float pulse = (sin(glowTime * 2.f + i * 0.3f) + 1.f) / 2.f;
                            float outerRadius = portRadius + 8.f + pulse * 4.f;
                            sf::CircleShape outerGlow(outerRadius);
                            outerGlow.setOrigin(outerRadius, outerRadius);
                            outerGlow.setPosition(ports[i].pixel);
                            outerGlow.setFillColor(sf::Color(0, 255, 150, (sf::Uint8)(20 + pulse * 20)));
                            window.draw(outerGlow);

                            float midRadius = portRadius + 5.f + pulse * 2.f;
                            sf::CircleShape midGlow(midRadius);
                            midGlow.setOrigin(midRadius, midRadius);
                            midGlow.setPosition(ports[i].pixel);
                            midGlow.setFillColor(sf::Color(0, 255, 150, (sf::Uint8)(40 + pulse * 30)));
                            midGlow.setOutlineThickness(1.f);
                            midGlow.setOutlineColor(sf::Color(0, 255, 100, (sf::Uint8)(80 + pulse * 50)));
                            window.draw(midGlow);

                            float innerRadius = portRadius + 2.f + pulse * 1.f;
                            sf::CircleShape innerGlow(innerRadius);
                            innerGlow.setOrigin(innerRadius, innerRadius);
                            innerGlow.setPosition(ports[i].pixel);
                            innerGlow.setFillColor(sf::Color::Transparent);
                            innerGlow.setOutlineThickness(2.f);
                            innerGlow.setOutlineColor(sf::Color(100, 255, 200, (sf::Uint8)(150 + pulse * 100)));
                            window.draw(innerGlow);
                        }
                    }
                }

                for (int i = 0; i < portCount; i++)
                {
                    portShape.setPosition(ports[i].pixel);
                    window.draw(portShape);
                    if (i == hoveredPort)
                    {
                        portHighlight.setPosition(ports[i].pixel);
                        window.draw(portHighlight);
                    }
                }

                shipAnimation.draw(window, view);
                window.setView(window.getDefaultView());
                controlPanel.draw(window);
                simBtn.draw(window);
                shipAnimation.drawUI(window, panelWidth);

                if (mousePix.x > panelWidth && !shipAnimation.isActive())
                {
                    if (hoveredPort != -1)
                    {
                        drawPortDropdown(window, font, hoveredPort, mousePix);
                    }
                    else if (hoveredRoute != -1)
                    {
                        Routes& r = routes[hoveredRoute];

                        float tooltipWidth = 380.f;
                        float tooltipHeight = 70.f;
                        float tooltipX = (float)mousePix.x + 15.f;
                        float tooltipY = (float)mousePix.y + 15.f;

                        if (tooltipX + tooltipWidth > windowWidth) tooltipX = windowWidth - tooltipWidth - 10.f;
                        if (tooltipY + tooltipHeight > windowHeight) tooltipY = windowHeight - tooltipHeight - 10.f;

                        sf::RectangleShape tooltip(sf::Vector2f(tooltipWidth, tooltipHeight));
                        tooltip.setPosition(tooltipX, tooltipY);
                        tooltip.setFillColor(sf::Color(15, 25, 45, 240));
                        tooltip.setOutlineThickness(2);
                        tooltip.setOutlineColor(sf::Color(0, 200, 255));
                        window.draw(tooltip);

                        sf::Text info1(ports[r.originIndex].name + " -> " + ports[r.destinationIndex].name, font, 13);
                        info1.setFillColor(sf::Color(0, 220, 255));
                        info1.setStyle(sf::Text::Bold);
                        info1.setPosition(tooltipX + 10.f, tooltipY + 8.f);
                        window.draw(info1);

                        sf::Text info2(r.date + " | " + r.depTime + " - " + r.arrTime, font, 11);
                        info2.setFillColor(sf::Color(200, 200, 200));
                        info2.setPosition(tooltipX + 10.f, tooltipY + 28.f);
                        window.draw(info2);

                        sf::Text info3("Cost: $" + intToString(r.vCost) + " | " + r.company, font, 11);
                        info3.setFillColor(sf::Color(150, 220, 255));
                        info3.setPosition(tooltipX + 10.f, tooltipY + 46.f);
                        window.draw(info3);
                    }
                }

                if (sfmlPreferences.usePreferredCompanies && sfmlPreferences.preferredCompanyCount > 0)
                {
                    float legendX = panelWidth + 15.f;
                    float legendY = 60.f;
                    float legendWidth = 200.f;
                    float legendHeight = 25.f + sfmlPreferences.preferredCompanyCount * 18.f;

                    sf::RectangleShape legendBg(sf::Vector2f(legendWidth, legendHeight));
                    legendBg.setPosition(legendX, legendY);
                    legendBg.setFillColor(sf::Color(10, 20, 35, 220));
                    legendBg.setOutlineThickness(1.f);
                    legendBg.setOutlineColor(sf::Color(0, 200, 150, 150));
                    window.draw(legendBg);

                    sf::Text legendTitle("PREFERRED COMPANIES", font, 10);
                    legendTitle.setFillColor(sf::Color(0, 255, 200));
                    legendTitle.setStyle(sf::Text::Bold);
                    legendTitle.setPosition(legendX + 10.f, legendY + 5.f);
                    window.draw(legendTitle);

                    for (int c = 0; c < sfmlPreferences.preferredCompanyCount && c < 5; c++)
                    {
                        float indicatorY = legendY + 25.f + c * 18.f;

                        sf::RectangleShape colorIndicator(sf::Vector2f(12.f, 12.f));
                        colorIndicator.setPosition(legendX + 10.f, indicatorY);
                        colorIndicator.setFillColor(sf::Color(0, 255, 150, 200));
                        colorIndicator.setOutlineThickness(1.f);
                        colorIndicator.setOutlineColor(sf::Color(100, 255, 200));
                        window.draw(colorIndicator);

                        sf::Text companyName(sfmlPreferences.preferredCompanies[c], font, 11);
                        companyName.setFillColor(sf::Color(200, 220, 240));
                        companyName.setPosition(legendX + 28.f, indicatorY - 2.f);
                        window.draw(companyName);
                    }

                    float pulse = (sin(glowTime * 2.f) + 1.f) / 2.f;
                    legendBg.setOutlineColor(sf::Color(0, (sf::Uint8)(180 + pulse * 75), (sf::Uint8)(150 + pulse * 50), (sf::Uint8)(150 + pulse * 50)));
                }

                if (!shipAnimation.isActive())
                {
                    sf::Text instructions("Mouse Wheel = Zoom | Drag = Pan | ESC = Menu | Hover ports for details", font, 11);
                    instructions.setFillColor(sf::Color(150, 150, 170));
                    instructions.setPosition(panelWidth + 15.f, (float)windowHeight - 22.f);
                    window.draw(instructions);
                }

                multiLegBuilderPanel.draw(window);
                preferencesPanel.draw(window);
                dockQueuePanel.draw(window);

                bookingPanel.draw(window);
                optimizationPanel.draw(window);
                subgraphPanel.draw(window);
                window.display();

                continue;
            }
        }
    }
}