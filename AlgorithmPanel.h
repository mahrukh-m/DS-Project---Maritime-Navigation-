#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "MenuButton.h"

using namespace std;
class BookingPanel
{
private:
    sf::RectangleShape overlay;
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::RectangleShape headerGlow;

    sf::Text titleText;

    sf::RectangleShape srcBox, dstBox, dateBox, companyBox, maxTimeBox;
    sf::RectangleShape srcAccent, dstAccent, dateAccent, companyAccent, maxTimeAccent;
    sf::Text srcLabel, dstLabel, dateLabel, companyLabel, maxTimeLabel;
    sf::Text srcText, dstText, dateText, companyText, maxTimeText;

    string srcInput, destInput, dateInput, companyInputStr, maxTimeInput;

    sf::RectangleShape btnFind;
    sf::RectangleShape btnFindGlow;
    sf::Text btnFindText;

    sf::RectangleShape btnClose;

    float panelWidth, panelHeight, panelX, panelY;
    float animPhase;
    int activeInput;
    float findHoverProgress;
    float closeHoverProgress;
    sf::Font* fontPtr;

    void setupInputField(sf::RectangleShape& box, sf::RectangleShape& accent,
        sf::Text& label, sf::Text& text,
        float x, float y, float w, float h,
        const string& labelStr, const string& placeholder,
        sf::Color accentColor, sf::Font& font)
    {
        box.setSize(sf::Vector2f(w, h));
        box.setPosition(x, y + 16.f);
        box.setFillColor(sf::Color(20, 35, 55));
        box.setOutlineThickness(1.f);
        box.setOutlineColor(sf::Color(40, 70, 100));

        accent.setSize(sf::Vector2f(3.f, h));
        accent.setPosition(x, y + 16.f);
        accent.setFillColor(accentColor);

        label.setFont(font);
        label.setString(labelStr);
        label.setCharacterSize(10);
        label.setFillColor(accentColor);
        label.setLetterSpacing(1.2f);
        label.setPosition(x + 5.f, y);

        text.setFont(font);
        text.setString(placeholder);
        text.setCharacterSize(13);
        text.setFillColor(sf::Color(80, 100, 120));
        text.setPosition(x + 12.f, y + 22.f);
    }

public:
    bool isVisible;

    BookingPanel() : isVisible(false), animPhase(0.f), activeInput(0),
        findHoverProgress(0.f), closeHoverProgress(0.f), fontPtr(nullptr)
    {
        panelWidth = 480.f;
        panelHeight = 430.f;
        srcInput = destInput = dateInput = companyInputStr = maxTimeInput = "";
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;
        isVisible = false;

        panelX = (windowWidth - panelWidth) / 2.f;
        panelY = (windowHeight - panelHeight) / 2.f;

        overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
        overlay.setPosition(0, 0);
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(12, 22, 38, 252));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color(30, 70, 110));

        headerBar.setSize(sf::Vector2f(panelWidth, 50.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(15, 30, 50));

        headerGlow.setSize(sf::Vector2f(panelWidth - 40.f, 2.f));
        headerGlow.setPosition(panelX + 20.f, panelY + 50.f);
        headerGlow.setFillColor(sf::Color(0, 200, 255, 150));

        titleText.setFont(font);
        titleText.setString("SHIP BOOKING");
        titleText.setCharacterSize(20);
        titleText.setFillColor(sf::Color(0, 220, 255));
        titleText.setStyle(sf::Text::Bold);
        titleText.setLetterSpacing(2.f);
        titleText.setPosition(panelX + 25.f, panelY + 12.f);

        btnClose.setSize(sf::Vector2f(30.f, 30.f));
        btnClose.setPosition(panelX + panelWidth - 42.f, panelY + 10.f);
        btnClose.setFillColor(sf::Color(40, 50, 65));
        btnClose.setOutlineThickness(1.f);
        btnClose.setOutlineColor(sf::Color(60, 80, 100));

        float inputX = panelX + 25.f;
        float inputW = panelWidth - 50.f;
        float inputH = 32.f;
        float startY = panelY + 65.f;
        float spacing = 52.f;

        setupInputField(srcBox, srcAccent, srcLabel, srcText,
            inputX, startY, inputW, inputH,
            "SOURCE PORT", "Enter source port...",
            sf::Color(0, 180, 255), font);

        setupInputField(dstBox, dstAccent, dstLabel, dstText,
            inputX, startY + spacing, inputW, inputH,
            "DESTINATION PORT", "Enter destination port...",
            sf::Color(0, 180, 255), font);

        setupInputField(dateBox, dateAccent, dateLabel, dateText,
            inputX, startY + spacing * 2, inputW, inputH,
            "DEPARTURE DATE", "DD/MM/YYYY",
            sf::Color(100, 200, 150), font);

        setupInputField(companyBox, companyAccent, companyLabel, companyText,
            inputX, startY + spacing * 3, inputW, inputH,
            "PREFERRED COMPANY (optional)", "e.g. MaerskLine, Evergreen",
            sf::Color(180, 150, 80), font);

        setupInputField(maxTimeBox, maxTimeAccent, maxTimeLabel, maxTimeText,
            inputX, startY + spacing * 4, inputW, inputH,
            "MAX VOYAGE TIME (optional)", "Time in minutes, e.g. 1440",
            sf::Color(180, 150, 80), font);

        btnFind.setSize(sf::Vector2f(panelWidth - 50.f, 50.f));
        btnFind.setPosition(panelX + 25.f, panelY + panelHeight - 70.f);
        btnFind.setFillColor(sf::Color(0, 120, 100));

        btnFindGlow.setSize(sf::Vector2f(panelWidth - 46.f, 54.f));
        btnFindGlow.setPosition(panelX + 23.f, panelY + panelHeight - 72.f);
        btnFindGlow.setFillColor(sf::Color::Transparent);
        btnFindGlow.setOutlineThickness(2.f);
        btnFindGlow.setOutlineColor(sf::Color(0, 255, 200, 0));

        btnFindText.setFont(font);
        btnFindText.setString("FIND ROUTE");
        btnFindText.setCharacterSize(18);
        btnFindText.setFillColor(sf::Color::White);
        btnFindText.setStyle(sf::Text::Bold);
        btnFindText.setLetterSpacing(3.f);
        sf::FloatRect textBounds = btnFindText.getLocalBounds();
        btnFindText.setPosition(
            panelX + (panelWidth - textBounds.width) / 2.f,
            panelY + panelHeight - 58.f
        );
    }

    void show() 
    { 
        isVisible = true; 
        activeInput = 0; 
    }
    void hide() 
    { 
        isVisible = false; 
        activeInput = 0; 
    }

    string getSource() 
    { 
        return srcInput; 
    }
    string getDestination() 
    { 
        return destInput; 
    }
    string getDate() 
    { 
        return dateInput; 
    }
    string getCompany() 
    { 
        return companyInputStr; 
    }
    string getMaxTime() 
    { 
        return maxTimeInput; 
    }

    void clear()
    {
        srcInput = destInput = dateInput = companyInputStr = maxTimeInput = "";
    }

    void handleTextInput(sf::Uint32 unicode)
    {
        if (!isVisible || activeInput == 0) 
        {
            return;
        }

        string* target = nullptr;
        switch (activeInput)
        {
        case 1: 
            target = &srcInput; 
            break;
        case 2: 
            target = &destInput; 
            break;
        case 3: 
            target = &dateInput; 
            break;
        case 4: 
            target = &companyInputStr; 
            break;
        case 5: 
            target = &maxTimeInput; 
            break;
        default: 
            return;
        }

        if (unicode == 8) 
        {
            if (!target->empty()) target->pop_back();
        }
        else if (unicode == 9) 
        {
            activeInput = (activeInput % 5) + 1;
        }
        else if (unicode >= 32 && unicode < 128 && target->length() < 35)
        {
            *target += static_cast<char>(unicode);
        }
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (!isVisible) 
        {
            return -1;
        }

        activeInput = 0;

        if (srcBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 1;
            return -1; 
        }
        if (dstBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 2; 
            return -1; 
        }
        if (dateBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 3; 
            return -1; 
        }
        if (companyBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 4; 
            return -1; 
        }
        if (maxTimeBox.getGlobalBounds().contains(mousePos)) 
        { 
            activeInput = 5; 
            return -1; 
        }

        if (btnFind.getGlobalBounds().contains(mousePos)) 
        {
            return 1;
        }
        if (btnClose.getGlobalBounds().contains(mousePos)) 
        {
            return 0;
        }

        return -1;
    }

    void update(float deltaTime)
    {
        if (!isVisible) 
        {
            return;
        }

        animPhase += deltaTime * 2.5f;
        if (animPhase > 6.28318f) 
        {
            animPhase -= 6.28318f;
        }

        srcText.setString(srcInput.empty() ? "Enter source port..." : srcInput);
        srcText.setFillColor(srcInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        srcBox.setOutlineColor(activeInput == 1 ? sf::Color(0, 180, 255) : sf::Color(40, 70, 100));
        srcBox.setOutlineThickness(activeInput == 1 ? 2.f : 1.f);
        srcAccent.setSize(sf::Vector2f(activeInput == 1 ? 5.f : 3.f, 32.f));

        dstText.setString(destInput.empty() ? "Enter destination port..." : destInput);
        dstText.setFillColor(destInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        dstBox.setOutlineColor(activeInput == 2 ? sf::Color(0, 180, 255) : sf::Color(40, 70, 100));
        dstBox.setOutlineThickness(activeInput == 2 ? 2.f : 1.f);
        dstAccent.setSize(sf::Vector2f(activeInput == 2 ? 5.f : 3.f, 32.f));

        dateText.setString(dateInput.empty() ? "DD/MM/YYYY" : dateInput);
        dateText.setFillColor(dateInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        dateBox.setOutlineColor(activeInput == 3 ? sf::Color(100, 200, 150) : sf::Color(40, 70, 100));
        dateBox.setOutlineThickness(activeInput == 3 ? 2.f : 1.f);
        dateAccent.setSize(sf::Vector2f(activeInput == 3 ? 5.f : 3.f, 32.f));

        companyText.setString(companyInputStr.empty() ? "e.g. MaerskLine, Evergreen" : companyInputStr);
        companyText.setFillColor(companyInputStr.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        companyBox.setOutlineColor(activeInput == 4 ? sf::Color(180, 150, 80) : sf::Color(40, 70, 100));
        companyBox.setOutlineThickness(activeInput == 4 ? 2.f : 1.f);
        companyAccent.setSize(sf::Vector2f(activeInput == 4 ? 5.f : 3.f, 32.f));

        maxTimeText.setString(maxTimeInput.empty() ? "Time in minutes, e.g. 1440" : maxTimeInput);
        maxTimeText.setFillColor(maxTimeInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        maxTimeBox.setOutlineColor(activeInput == 5 ? sf::Color(180, 150, 80) : sf::Color(40, 70, 100));
        maxTimeBox.setOutlineThickness(activeInput == 5 ? 2.f : 1.f);
        maxTimeAccent.setSize(sf::Vector2f(activeInput == 5 ? 5.f : 3.f, 32.f));

        float pulse = (sin(animPhase) + 1.f) / 2.f;
        headerGlow.setFillColor(sf::Color(0, (sf::Uint8)(180 + pulse * 50), 255, (sf::Uint8)(120 + pulse * 80)));
    }

    void updateHover(sf::Vector2f mousePos, float deltaTime)
    {
        if (!isVisible) 
        {
            return;
        }

        bool hover = btnFind.getGlobalBounds().contains(mousePos);
        findHoverProgress = hover ? min(1.f, findHoverProgress + deltaTime * 8.f)
            : max(0.f, findHoverProgress - deltaTime * 5.f);
        float t = findHoverProgress;
        btnFind.setFillColor(sf::Color(
            (sf::Uint8)(0 + t * 20),
            (sf::Uint8)(120 + t * 60),
            (sf::Uint8)(100 + t * 55)
        ));
        btnFindGlow.setOutlineColor(sf::Color(0, 255, 200, (sf::Uint8)(t * 150)));

        hover = btnClose.getGlobalBounds().contains(mousePos);
        closeHoverProgress = hover ? min(1.f, closeHoverProgress + deltaTime * 8.f)
            : max(0.f, closeHoverProgress - deltaTime * 5.f);
        t = closeHoverProgress;
        btnClose.setFillColor(sf::Color(
            (sf::Uint8)(40 + t * 80),
            (sf::Uint8)(50 - t * 20),
            (sf::Uint8)(65 - t * 30)
        ));
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible) 
        {
            return;
        }

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

        drawInputField(window, srcBox, srcAccent, srcLabel, srcText, activeInput == 1, sf::Color(0, 180, 255));
        drawInputField(window, dstBox, dstAccent, dstLabel, dstText, activeInput == 2, sf::Color(0, 180, 255));
        drawInputField(window, dateBox, dateAccent, dateLabel, dateText, activeInput == 3, sf::Color(100, 200, 150));
        drawInputField(window, companyBox, companyAccent, companyLabel, companyText, activeInput == 4, sf::Color(180, 150, 80));
        drawInputField(window, maxTimeBox, maxTimeAccent, maxTimeLabel, maxTimeText, activeInput == 5, sf::Color(180, 150, 80));

        window.draw(btnFindGlow);
        window.draw(btnFind);
        window.draw(btnFindText);

        drawCorners(window, sf::Color(0, 180, 255, 100));
    }

    void drawInputField(sf::RenderWindow& window, sf::RectangleShape& box, sf::RectangleShape& accent,
        sf::Text& label, sf::Text& text, bool isFocused, sf::Color accentColor)
    {
        window.draw(box);

        if (isFocused)
        {
            float pulse = (sin(animPhase * 3.f) + 1.f) / 2.f;
            sf::RectangleShape glow = accent;
            glow.setSize(sf::Vector2f(accent.getSize().x + 6.f, accent.getSize().y));
            glow.setPosition(accent.getPosition().x - 3.f, accent.getPosition().y);
            glow.setFillColor(sf::Color(accentColor.r, accentColor.g, accentColor.b, (sf::Uint8)(40 + pulse * 50)));
            window.draw(glow);

            // Cursor
            if (pulse > 0.5f)
            {
                sf::RectangleShape cursor(sf::Vector2f(2.f, 16.f));
                cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 2.f, text.getPosition().y + 2.f);
                cursor.setFillColor(sf::Color(200, 220, 255));
                window.draw(cursor);
            }
        }

        window.draw(accent);
        window.draw(label);
        window.draw(text);
    }

    void drawCorners(sf::RenderWindow& window, sf::Color color)
    {
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

class SimulateButton
{
private:
    sf::RectangleShape base;
    sf::RectangleShape glow;
    sf::RectangleShape highlight;
    sf::RectangleShape iconBg;
    sf::ConvexShape playIcon;
    sf::Text label;

    float hoverProgress;
    float pulsePhase;
    bool isHovered;

    float posX, posY;
    float width, height;

public:
    SimulateButton() : hoverProgress(0.f), pulsePhase(0.f), isHovered(false) {}

    void initialize(float x, float y, sf::Font& font)
    {
        posX = x;
        posY = y;
        width = 170.f;
        height = 45.f;

        base.setSize(sf::Vector2f(width, height));
        base.setPosition(posX, posY);
        base.setFillColor(sf::Color(15, 80, 120));
        base.setOutlineThickness(2.f);
        base.setOutlineColor(sf::Color(0, 150, 200));

        glow.setSize(sf::Vector2f(width + 8.f, height + 8.f));
        glow.setPosition(posX - 4.f, posY - 4.f);
        glow.setFillColor(sf::Color::Transparent);
        glow.setOutlineThickness(3.f);
        glow.setOutlineColor(sf::Color(0, 200, 255, 0));

        highlight.setSize(sf::Vector2f(width - 4.f, 2.f));
        highlight.setPosition(posX + 2.f, posY + 2.f);
        highlight.setFillColor(sf::Color(255, 255, 255, 50));

        iconBg.setSize(sf::Vector2f(30.f, 30.f));
        iconBg.setPosition(posX + 8.f, posY + 7.f);
        iconBg.setFillColor(sf::Color(0, 100, 150));
        iconBg.setOutlineThickness(1.f);
        iconBg.setOutlineColor(sf::Color(0, 180, 220));

        playIcon.setPointCount(3);
        playIcon.setPoint(0, sf::Vector2f(0.f, 0.f));
        playIcon.setPoint(1, sf::Vector2f(12.f, 7.f));
        playIcon.setPoint(2, sf::Vector2f(0.f, 14.f));
        playIcon.setPosition(posX + 18.f, posY + 15.f);
        playIcon.setFillColor(sf::Color(0, 255, 200));

        label.setFont(font);
        label.setString("SIMULATE");
        label.setCharacterSize(15);
        label.setFillColor(sf::Color::White);
        label.setStyle(sf::Text::Bold);
        label.setLetterSpacing(2.f);
        label.setPosition(posX + 45.f, posY + 12.f);
    }

    bool isMouseOver(sf::Vector2f mousePos)
    {
        return base.getGlobalBounds().contains(mousePos);
    }

    void update(float deltaTime, sf::Vector2f mousePos)
    {
        pulsePhase += deltaTime * 3.f;
        if (pulsePhase > 6.28318f) pulsePhase -= 6.28318f;

        isHovered = isMouseOver(mousePos);

        if (isHovered)
            hoverProgress = min(1.f, hoverProgress + deltaTime * 8.f);
        else
            hoverProgress = max(0.f, hoverProgress - deltaTime * 5.f);

        float t = hoverProgress;
        float pulse = (sin(pulsePhase) + 1.f) / 2.f;

        base.setFillColor(sf::Color(
            (sf::Uint8)(15 + t * 40),
            (sf::Uint8)(80 + t * 60),
            (sf::Uint8)(120 + t * 60)
        ));

        base.setOutlineColor(sf::Color(
            (sf::Uint8)(0 + t * 50),
            (sf::Uint8)(150 + t * 50),
            (sf::Uint8)(200 + t * 55)
        ));

        glow.setOutlineColor(sf::Color(0, 220, 255, (sf::Uint8)(t * 150)));

        iconBg.setFillColor(sf::Color(
            (sf::Uint8)(0 + t * 20),
            (sf::Uint8)(100 + t * 50 + pulse * 20),
            (sf::Uint8)(150 + t * 50)
        ));

        playIcon.setFillColor(sf::Color(
            (sf::Uint8)(0 + t * 50),
            (sf::Uint8)(255),
            (sf::Uint8)(200 + t * 55)
        ));

        if (isHovered)
        {
            float iconPulse = (sin(pulsePhase * 2.f) + 1.f) / 2.f;
            playIcon.setScale(1.f + iconPulse * 0.15f, 1.f + iconPulse * 0.15f);
        }
        else
        {
            playIcon.setScale(1.f, 1.f);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (hoverProgress > 0)
        {
            window.draw(glow);
        }

        sf::RectangleShape shadow = base;
        shadow.setPosition(posX + 3.f, posY + 3.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 60));
        shadow.setOutlineThickness(0);
        window.draw(shadow);

        window.draw(base);
        window.draw(highlight);
        window.draw(iconBg);
        window.draw(playIcon);
        window.draw(label);
    }
};

class OptimizationPanel
{
private:
    sf::RectangleShape overlay;
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::RectangleShape headerGlow;

    sf::Text titleText;

    sf::RectangleShape srcBox, dstBox;
    sf::RectangleShape srcAccent, dstAccent;
    sf::Text srcLabel, dstLabel;
    sf::Text srcText, dstText;

    string srcInput, dstInput;

    sf::RectangleShape btnDijkstraCost, btnDijkstraTime, btnAStarCost, btnAStarTime;
    sf::RectangleShape dijkstraCostAccent, dijkstraTimeAccent, astarCostAccent, astarTimeAccent;
    sf::Text txtDijkstraCost, txtDijkstraTime, txtAStarCost, txtAStarTime;
    sf::Text subDijkstraCost, subDijkstraTime, subAStarCost, subAStarTime;

    sf::RectangleShape btnClose;

    float panelWidth, panelHeight, panelX, panelY;
    float animPhase;
    int activeInput;
    float dijkstraCostHover, dijkstraTimeHover, astarCostHover, astarTimeHover;
    float closeHoverProgress;
    sf::Font* fontPtr;

public:
    bool isVisible;

    OptimizationPanel() : isVisible(false), animPhase(0.f), activeInput(0),
        dijkstraCostHover(0.f), dijkstraTimeHover(0.f), astarCostHover(0.f), astarTimeHover(0.f),
        closeHoverProgress(0.f), fontPtr(nullptr)
    {
        panelWidth = 500.f;
        panelHeight = 380.f;
        srcInput = dstInput = "";
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;
        isVisible = false;

        panelX = (windowWidth - panelWidth) / 2.f;
        panelY = (windowHeight - panelHeight) / 2.f;

        overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
        overlay.setPosition(0, 0);
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(12, 22, 38, 252));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color(60, 50, 120));

        headerBar.setSize(sf::Vector2f(panelWidth, 50.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(15, 30, 50));

        headerGlow.setSize(sf::Vector2f(panelWidth - 40.f, 2.f));
        headerGlow.setPosition(panelX + 20.f, panelY + 50.f);
        headerGlow.setFillColor(sf::Color(130, 100, 255, 150));

        titleText.setFont(font);
        titleText.setString("ROUTE OPTIMIZATION");
        titleText.setCharacterSize(18);
        titleText.setFillColor(sf::Color(150, 130, 255));
        titleText.setStyle(sf::Text::Bold);
        titleText.setLetterSpacing(2.f);
        titleText.setPosition(panelX + 25.f, panelY + 13.f);

        btnClose.setSize(sf::Vector2f(30.f, 30.f));
        btnClose.setPosition(panelX + panelWidth - 42.f, panelY + 10.f);
        btnClose.setFillColor(sf::Color(40, 50, 65));
        btnClose.setOutlineThickness(1.f);
        btnClose.setOutlineColor(sf::Color(60, 80, 100));

        float inputX = panelX + 25.f;
        float inputW = panelWidth - 50.f;
        float inputH = 32.f;

        srcBox.setSize(sf::Vector2f(inputW, inputH));
        srcBox.setPosition(inputX, panelY + 80.f);
        srcBox.setFillColor(sf::Color(20, 35, 55));
        srcBox.setOutlineThickness(1.f);
        srcBox.setOutlineColor(sf::Color(40, 70, 100));

        srcAccent.setSize(sf::Vector2f(3.f, inputH));
        srcAccent.setPosition(inputX, panelY + 80.f);
        srcAccent.setFillColor(sf::Color(130, 100, 255));

        srcLabel.setFont(font);
        srcLabel.setString("SOURCE PORT");
        srcLabel.setCharacterSize(10);
        srcLabel.setFillColor(sf::Color(130, 100, 255));
        srcLabel.setLetterSpacing(1.2f);
        srcLabel.setPosition(inputX + 5.f, panelY + 64.f);

        srcText.setFont(font);
        srcText.setString("Enter source port...");
        srcText.setCharacterSize(13);
        srcText.setFillColor(sf::Color(80, 100, 120));
        srcText.setPosition(inputX + 12.f, panelY + 86.f);

        dstBox.setSize(sf::Vector2f(inputW, inputH));
        dstBox.setPosition(inputX, panelY + 135.f);
        dstBox.setFillColor(sf::Color(20, 35, 55));
        dstBox.setOutlineThickness(1.f);
        dstBox.setOutlineColor(sf::Color(40, 70, 100));

        dstAccent.setSize(sf::Vector2f(3.f, inputH));
        dstAccent.setPosition(inputX, panelY + 135.f);
        dstAccent.setFillColor(sf::Color(130, 100, 255));

        dstLabel.setFont(font);
        dstLabel.setString("DESTINATION PORT");
        dstLabel.setCharacterSize(10);
        dstLabel.setFillColor(sf::Color(130, 100, 255));
        dstLabel.setLetterSpacing(1.2f);
        dstLabel.setPosition(inputX + 5.f, panelY + 119.f);

        dstText.setFont(font);
        dstText.setString("Enter destination port...");
        dstText.setCharacterSize(13);
        dstText.setFillColor(sf::Color(80, 100, 120));
        dstText.setPosition(inputX + 12.f, panelY + 141.f);

        float btnW = (panelWidth - 60.f) / 2.f;
        float btnH = 60.f;
        float btnY = panelY + 190.f;
        float gap = 10.f;

        btnDijkstraCost.setSize(sf::Vector2f(btnW, btnH));
        btnDijkstraCost.setPosition(panelX + 25.f, btnY);
        btnDijkstraCost.setFillColor(sf::Color(25, 70, 130));

        dijkstraCostAccent.setSize(sf::Vector2f(4.f, btnH));
        dijkstraCostAccent.setPosition(panelX + 25.f, btnY);
        dijkstraCostAccent.setFillColor(sf::Color(80, 160, 255));

        txtDijkstraCost.setFont(font);
        txtDijkstraCost.setString("Dijkstra Cost");
        txtDijkstraCost.setCharacterSize(16);
        txtDijkstraCost.setFillColor(sf::Color::White);
        txtDijkstraCost.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = txtDijkstraCost.getLocalBounds();
        txtDijkstraCost.setPosition(panelX + 25.f + (btnW - bounds.width) / 2.f, btnY + 12.f);

        subDijkstraCost.setFont(font);
        subDijkstraCost.setString("Minimum Cost");
        subDijkstraCost.setCharacterSize(10);
        subDijkstraCost.setFillColor(sf::Color(150, 180, 210));
        bounds = subDijkstraCost.getLocalBounds();
        subDijkstraCost.setPosition(panelX + 25.f + (btnW - bounds.width) / 2.f, btnY + 38.f);

        btnDijkstraTime.setSize(sf::Vector2f(btnW, btnH));
        btnDijkstraTime.setPosition(panelX + 25.f + btnW + gap, btnY);
        btnDijkstraTime.setFillColor(sf::Color(25, 70, 130));

        dijkstraTimeAccent.setSize(sf::Vector2f(4.f, btnH));
        dijkstraTimeAccent.setPosition(panelX + 25.f + btnW + gap, btnY);
        dijkstraTimeAccent.setFillColor(sf::Color(80, 160, 255));

        txtDijkstraTime.setFont(font);
        txtDijkstraTime.setString("Dijkstra Time");
        txtDijkstraTime.setCharacterSize(16);
        txtDijkstraTime.setFillColor(sf::Color::White);
        txtDijkstraTime.setStyle(sf::Text::Bold);
        bounds = txtDijkstraTime.getLocalBounds();
        txtDijkstraTime.setPosition(panelX + 25.f + btnW + gap + (btnW - bounds.width) / 2.f, btnY + 12.f);

        subDijkstraTime.setFont(font);
        subDijkstraTime.setString("Minimum Time");
        subDijkstraTime.setCharacterSize(10);
        subDijkstraTime.setFillColor(sf::Color(150, 180, 210));
        bounds = subDijkstraTime.getLocalBounds();
        subDijkstraTime.setPosition(panelX + 25.f + btnW + gap + (btnW - bounds.width) / 2.f, btnY + 38.f);

        btnAStarCost.setSize(sf::Vector2f(btnW, btnH));
        btnAStarCost.setPosition(panelX + 25.f, btnY + btnH + gap);
        btnAStarCost.setFillColor(sf::Color(130, 60, 25));

        astarCostAccent.setSize(sf::Vector2f(4.f, btnH));
        astarCostAccent.setPosition(panelX + 25.f, btnY + btnH + gap);
        astarCostAccent.setFillColor(sf::Color(255, 150, 80));

        txtAStarCost.setFont(font);
        txtAStarCost.setString("A* Cost");
        txtAStarCost.setCharacterSize(16);
        txtAStarCost.setFillColor(sf::Color::White);
        txtAStarCost.setStyle(sf::Text::Bold);
        bounds = txtAStarCost.getLocalBounds();
        txtAStarCost.setPosition(panelX + 25.f + (btnW - bounds.width) / 2.f, btnY + btnH + gap + 12.f);

        subAStarCost.setFont(font);
        subAStarCost.setString("Heuristic Cost");
        subAStarCost.setCharacterSize(10);
        subAStarCost.setFillColor(sf::Color(210, 180, 150));
        bounds = subAStarCost.getLocalBounds();
        subAStarCost.setPosition(panelX + 25.f + (btnW - bounds.width) / 2.f, btnY + btnH + gap + 38.f);

        btnAStarTime.setSize(sf::Vector2f(btnW, btnH));
        btnAStarTime.setPosition(panelX + 25.f + btnW + gap, btnY + btnH + gap);
        btnAStarTime.setFillColor(sf::Color(130, 60, 25));

        astarTimeAccent.setSize(sf::Vector2f(4.f, btnH));
        astarTimeAccent.setPosition(panelX + 25.f + btnW + gap, btnY + btnH + gap);
        astarTimeAccent.setFillColor(sf::Color(255, 150, 80));

        txtAStarTime.setFont(font);
        txtAStarTime.setString("A* Time");
        txtAStarTime.setCharacterSize(16);
        txtAStarTime.setFillColor(sf::Color::White);
        txtAStarTime.setStyle(sf::Text::Bold);
        bounds = txtAStarTime.getLocalBounds();
        txtAStarTime.setPosition(panelX + 25.f + btnW + gap + (btnW - bounds.width) / 2.f, btnY + btnH + gap + 12.f);

        subAStarTime.setFont(font);
        subAStarTime.setString("Heuristic Time");
        subAStarTime.setCharacterSize(10);
        subAStarTime.setFillColor(sf::Color(210, 180, 150));
        bounds = subAStarTime.getLocalBounds();
        subAStarTime.setPosition(panelX + 25.f + btnW + gap + (btnW - bounds.width) / 2.f, btnY + btnH + gap + 38.f);
    }

    void show() 
    { 
        isVisible = true; 
        activeInput = 0; 
    }
    void hide() 
    { 
        isVisible = false; 
        activeInput = 0; 
    }

    string getSource() 
    { 
        return srcInput; 
    }
    string getDestination() 
    {
        return dstInput; 
    }

    void clear() 
    { 
        srcInput = dstInput = ""; 
    }

    void handleTextInput(sf::Uint32 unicode)
    {
        if (!isVisible || activeInput == 0) 
        {
            return;
        }

        string* target = (activeInput == 1) ? &srcInput : &dstInput;

        if (unicode == 8)
        {
            if (!target->empty()) 
            {
                target->pop_back();
            }
        }
        else if (unicode == 9)
        {
            activeInput = (activeInput == 1) ? 2 : 1;
        }
        else if (unicode >= 32 && unicode < 128 && target->length() < 35)
        {
            *target += static_cast<char>(unicode);
        }
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (!isVisible) return -1;

        activeInput = 0;

        if (srcBox.getGlobalBounds().contains(mousePos)) { activeInput = 1; return -1; }
        if (dstBox.getGlobalBounds().contains(mousePos)) { activeInput = 2; return -1; }

        if (btnDijkstraCost.getGlobalBounds().contains(mousePos)) return 1;
        if (btnDijkstraTime.getGlobalBounds().contains(mousePos)) return 2;
        if (btnAStarCost.getGlobalBounds().contains(mousePos)) return 3;
        if (btnAStarTime.getGlobalBounds().contains(mousePos)) return 4;
        if (btnClose.getGlobalBounds().contains(mousePos)) return 0;

        return -1;
    }

    void update(float deltaTime)
    {
        if (!isVisible) return;

        animPhase += deltaTime * 2.5f;
        if (animPhase > 6.28318f) 
        {
            animPhase -= 6.28318f;
        }

        srcText.setString(srcInput.empty() ? "Enter source port..." : srcInput);
        srcText.setFillColor(srcInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        srcBox.setOutlineColor(activeInput == 1 ? sf::Color(130, 100, 255) : sf::Color(40, 70, 100));
        srcBox.setOutlineThickness(activeInput == 1 ? 2.f : 1.f);
        srcAccent.setSize(sf::Vector2f(activeInput == 1 ? 5.f : 3.f, 32.f));

        dstText.setString(dstInput.empty() ? "Enter destination port..." : dstInput);
        dstText.setFillColor(dstInput.empty() ? sf::Color(80, 100, 120) : sf::Color(220, 235, 255));
        dstBox.setOutlineColor(activeInput == 2 ? sf::Color(130, 100, 255) : sf::Color(40, 70, 100));
        dstBox.setOutlineThickness(activeInput == 2 ? 2.f : 1.f);
        dstAccent.setSize(sf::Vector2f(activeInput == 2 ? 5.f : 3.f, 32.f));

        float pulse = (sin(animPhase) + 1.f) / 2.f;
        headerGlow.setFillColor(sf::Color(130, 100, (sf::Uint8)(220 + pulse * 35), (sf::Uint8)(120 + pulse * 80)));
    }

    void updateHover(sf::Vector2f mousePos, float deltaTime)
    {
        if (!isVisible) 
        {
            return;
        }

        float btnH = 60.f;
        bool hover = btnDijkstraCost.getGlobalBounds().contains(mousePos);
        dijkstraCostHover = hover ? min(1.f, dijkstraCostHover + deltaTime * 8.f)
            : max(0.f, dijkstraCostHover - deltaTime * 5.f);
        float t = dijkstraCostHover;
        btnDijkstraCost.setFillColor(sf::Color((sf::Uint8)(25 + t * 30), (sf::Uint8)(70 + t * 40), (sf::Uint8)(130 + t * 40)));
        dijkstraCostAccent.setSize(sf::Vector2f(4.f + t * 4.f, btnH));

        hover = btnDijkstraTime.getGlobalBounds().contains(mousePos);
        dijkstraTimeHover = hover ? min(1.f, dijkstraTimeHover + deltaTime * 8.f)
            : max(0.f, dijkstraTimeHover - deltaTime * 5.f);
        t = dijkstraTimeHover;
        btnDijkstraTime.setFillColor(sf::Color((sf::Uint8)(25 + t * 30), (sf::Uint8)(70 + t * 40), (sf::Uint8)(130 + t * 40)));
        dijkstraTimeAccent.setSize(sf::Vector2f(4.f + t * 4.f, btnH));

        hover = btnAStarCost.getGlobalBounds().contains(mousePos);
        astarCostHover = hover ? min(1.f, astarCostHover + deltaTime * 8.f)
            : max(0.f, astarCostHover - deltaTime * 5.f);
        t = astarCostHover;
        btnAStarCost.setFillColor(sf::Color((sf::Uint8)(130 + t * 40), (sf::Uint8)(60 + t * 40), (sf::Uint8)(25 + t * 30)));
        astarCostAccent.setSize(sf::Vector2f(4.f + t * 4.f, btnH));

        hover = btnAStarTime.getGlobalBounds().contains(mousePos);
        astarTimeHover = hover ? min(1.f, astarTimeHover + deltaTime * 8.f)
            : max(0.f, astarTimeHover - deltaTime * 5.f);
        t = astarTimeHover;
        btnAStarTime.setFillColor(sf::Color((sf::Uint8)(130 + t * 40), (sf::Uint8)(60 + t * 40), (sf::Uint8)(25 + t * 30)));
        astarTimeAccent.setSize(sf::Vector2f(4.f + t * 4.f, btnH));

        hover = btnClose.getGlobalBounds().contains(mousePos);
        closeHoverProgress = hover ? min(1.f, closeHoverProgress + deltaTime * 8.f)
            : max(0.f, closeHoverProgress - deltaTime * 5.f);
        t = closeHoverProgress;
        btnClose.setFillColor(sf::Color((sf::Uint8)(40 + t * 80), (sf::Uint8)(50 - t * 20), (sf::Uint8)(65 - t * 30)));
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible) 
        {
            return;
        }

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

        drawInputField(window, srcBox, srcAccent, srcLabel, srcText, activeInput == 1, sf::Color(130, 100, 255));
        drawInputField(window, dstBox, dstAccent, dstLabel, dstText, activeInput == 2, sf::Color(130, 100, 255));

        drawAlgoButton(window, btnDijkstraCost, dijkstraCostAccent, txtDijkstraCost, subDijkstraCost, dijkstraCostHover);
        drawAlgoButton(window, btnDijkstraTime, dijkstraTimeAccent, txtDijkstraTime, subDijkstraTime, dijkstraTimeHover);
        drawAlgoButton(window, btnAStarCost, astarCostAccent, txtAStarCost, subAStarCost, astarCostHover);
        drawAlgoButton(window, btnAStarTime, astarTimeAccent, txtAStarTime, subAStarTime, astarTimeHover);

        drawCorners(window, sf::Color(130, 100, 255, 100));
    }

    void drawInputField(sf::RenderWindow& window, sf::RectangleShape& box, sf::RectangleShape& accent,
        sf::Text& label, sf::Text& text, bool isFocused, sf::Color accentColor)
    {
        window.draw(box);

        if (isFocused)
        {
            float pulse = (sin(animPhase * 3.f) + 1.f) / 2.f;
            sf::RectangleShape glow = accent;
            glow.setSize(sf::Vector2f(accent.getSize().x + 6.f, accent.getSize().y));
            glow.setPosition(accent.getPosition().x - 3.f, accent.getPosition().y);
            glow.setFillColor(sf::Color(accentColor.r, accentColor.g, accentColor.b, (sf::Uint8)(40 + pulse * 50)));
            window.draw(glow);

            if (pulse > 0.5f)
            {
                sf::RectangleShape cursor(sf::Vector2f(2.f, 16.f));
                cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 2.f, text.getPosition().y + 2.f);
                cursor.setFillColor(sf::Color(200, 180, 255));
                window.draw(cursor);
            }
        }

        window.draw(accent);
        window.draw(label);
        window.draw(text);
    }

    void drawAlgoButton(sf::RenderWindow& window, sf::RectangleShape& btn, sf::RectangleShape& accent,
        sf::Text& label, sf::Text& sublabel, float hoverProgress)
    {
        sf::RectangleShape shadow = btn;
        shadow.setPosition(btn.getPosition().x + 3.f, btn.getPosition().y + 3.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 60));
        window.draw(shadow);

        window.draw(btn);

        sf::RectangleShape highlight(sf::Vector2f(btn.getSize().x, 2.f));
        highlight.setPosition(btn.getPosition());
        highlight.setFillColor(sf::Color(255, 255, 255, 25));
        window.draw(highlight);

        if (hoverProgress > 0)
        {
            sf::RectangleShape glow = accent;
            glow.setSize(sf::Vector2f(accent.getSize().x + 6.f, accent.getSize().y));
            glow.setPosition(accent.getPosition().x - 3.f, accent.getPosition().y);
            glow.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(hoverProgress * 60)));
            window.draw(glow);
        }

        window.draw(accent);
        window.draw(label);
        window.draw(sublabel);
    }

    void drawCorners(sf::RenderWindow& window, sf::Color color)
    {
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

class SubgraphPanel
{
private:
    sf::RectangleShape overlay;
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::RectangleShape headerGlow;

    sf::Text titleText;
    sf::Text descriptionText;

    sf::RectangleShape companyBox;
    sf::RectangleShape companyAccent;
    sf::Text companyLabel;
    sf::Text companyText;

    sf::RectangleShape avoidPortBox;
    sf::RectangleShape avoidPortAccent;
    sf::Text avoidPortLabel;
    sf::Text avoidPortText;

    string companyInput;
    string avoidPortInput;
    bool inputFocused;
    bool avoidPortFocused;

    sf::RectangleShape btnGenerate;
    sf::RectangleShape btnGenerateGlow;
    sf::Text btnGenerateText;

    sf::RectangleShape btnClose;

    float panelWidth, panelHeight, panelX, panelY;
    float animPhase;
    float generateHoverProgress;
    float closeHoverProgress;
    sf::Font* fontPtr;

public:
    bool isVisible;

    SubgraphPanel() : isVisible(false), inputFocused(false), avoidPortFocused(false), animPhase(0.f),
        generateHoverProgress(0.f), closeHoverProgress(0.f), fontPtr(nullptr)
    {
        companyInput = "";
        avoidPortInput = "";
        panelWidth = 450.f;
        panelHeight = 360.f;
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;
        isVisible = false;

        panelX = (windowWidth - panelWidth) / 2.f;
        panelY = (windowHeight - panelHeight) / 2.f;

        overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
        overlay.setPosition(0, 0);
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(panelX, panelY);
        background.setFillColor(sf::Color(12, 22, 38, 252));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color(120, 90, 40));

        headerBar.setSize(sf::Vector2f(panelWidth, 50.f));
        headerBar.setPosition(panelX, panelY);
        headerBar.setFillColor(sf::Color(15, 30, 50));

        headerGlow.setSize(sf::Vector2f(panelWidth - 40.f, 2.f));
        headerGlow.setPosition(panelX + 20.f, panelY + 50.f);
        headerGlow.setFillColor(sf::Color(255, 180, 50, 150));

        titleText.setFont(font);
        titleText.setString("SUBGRAPH FILTER");
        titleText.setCharacterSize(18);
        titleText.setFillColor(sf::Color(255, 200, 100));
        titleText.setStyle(sf::Text::Bold);
        titleText.setLetterSpacing(2.f);
        titleText.setPosition(panelX + 25.f, panelY + 13.f);

        btnClose.setSize(sf::Vector2f(30.f, 30.f));
        btnClose.setPosition(panelX + panelWidth - 42.f, panelY + 10.f);
        btnClose.setFillColor(sf::Color(40, 50, 65));
        btnClose.setOutlineThickness(1.f);
        btnClose.setOutlineColor(sf::Color(60, 80, 100));

        descriptionText.setFont(font);
        descriptionText.setString("Filter routes by company and avoid specific ports.");
        descriptionText.setCharacterSize(13);
        descriptionText.setFillColor(sf::Color(140, 155, 175));
        descriptionText.setLineSpacing(1.4f);
        descriptionText.setPosition(panelX + 25.f, panelY + 65.f);

        float inputX = panelX + 25.f;
        float inputY = panelY + 100.f;
        float inputW = panelWidth - 50.f;
        float inputH = 35.f;

        companyBox.setSize(sf::Vector2f(inputW, inputH));
        companyBox.setPosition(inputX, inputY + 16.f);
        companyBox.setFillColor(sf::Color(20, 35, 55));
        companyBox.setOutlineThickness(1.f);
        companyBox.setOutlineColor(sf::Color(40, 70, 100));

        companyAccent.setSize(sf::Vector2f(3.f, inputH));
        companyAccent.setPosition(inputX, inputY + 16.f);
        companyAccent.setFillColor(sf::Color(255, 180, 50));

        companyLabel.setFont(font);
        companyLabel.setString("PREFERRED COMPANY");
        companyLabel.setCharacterSize(10);
        companyLabel.setFillColor(sf::Color(255, 180, 50));
        companyLabel.setLetterSpacing(1.2f);
        companyLabel.setPosition(inputX + 5.f, inputY);

        companyText.setFont(font);
        companyText.setString("e.g.  Evergreen, MaerskLine, CMA-CGM");
        companyText.setCharacterSize(13);
        companyText.setFillColor(sf::Color(80, 100, 120));
        companyText.setPosition(inputX + 12.f, inputY + 24.f);

        float avoidY = inputY + 75.f;

        avoidPortBox.setSize(sf::Vector2f(inputW, inputH));
        avoidPortBox.setPosition(inputX, avoidY + 16.f);
        avoidPortBox.setFillColor(sf::Color(20, 35, 55));
        avoidPortBox.setOutlineThickness(1.f);
        avoidPortBox.setOutlineColor(sf::Color(40, 70, 100));

        avoidPortAccent.setSize(sf::Vector2f(3.f, inputH));
        avoidPortAccent.setPosition(inputX, avoidY + 16.f);
        avoidPortAccent.setFillColor(sf::Color(255, 80, 80));

        avoidPortLabel.setFont(font);
        avoidPortLabel.setString("AVOID PORT");
        avoidPortLabel.setCharacterSize(10);
        avoidPortLabel.setFillColor(sf::Color(255, 80, 80));
        avoidPortLabel.setLetterSpacing(1.2f);
        avoidPortLabel.setPosition(inputX + 5.f, avoidY);

        avoidPortText.setFont(font);
        avoidPortText.setString("e.g. London, Singapore, Rotterdam");
        avoidPortText.setCharacterSize(13);
        avoidPortText.setFillColor(sf::Color(80, 100, 120));
        avoidPortText.setPosition(inputX + 12.f, avoidY + 24.f);

        btnGenerate.setSize(sf::Vector2f(panelWidth - 50.f, 50.f));
        btnGenerate.setPosition(panelX + 25.f, panelY + panelHeight - 70.f);
        btnGenerate.setFillColor(sf::Color(140, 95, 20));

        btnGenerateGlow.setSize(sf::Vector2f(panelWidth - 46.f, 54.f));
        btnGenerateGlow.setPosition(panelX + 23.f, panelY + panelHeight - 72.f);
        btnGenerateGlow.setFillColor(sf::Color::Transparent);
        btnGenerateGlow.setOutlineThickness(2.f);
        btnGenerateGlow.setOutlineColor(sf::Color(255, 200, 50, 0));

        btnGenerateText.setFont(font);
        btnGenerateText.setString("APPLY FILTERS");
        btnGenerateText.setCharacterSize(16);
        btnGenerateText.setFillColor(sf::Color::White);
        btnGenerateText.setStyle(sf::Text::Bold);
        btnGenerateText.setLetterSpacing(2.f);
        sf::FloatRect textBounds = btnGenerateText.getLocalBounds();
        btnGenerateText.setPosition(
            panelX + (panelWidth - textBounds.width) / 2.f,
            panelY + panelHeight - 58.f
        );
    }

    void show()
    {
        isVisible = true;
        inputFocused = false;
        avoidPortFocused = false;
    }

    void hide()
    {
        isVisible = false;
        inputFocused = false;
        avoidPortFocused = false;
    }

    string getCompany()
    {
        return companyInput;
    }

    string getAvoidPort()
    {
        return avoidPortInput;
    }

    void clear()
    {
        companyInput = "";
        avoidPortInput = "";
    }

    void handleTextInput(sf::Uint32 unicode)
    {
        if (!isVisible)
        {
            return;
        }

        if (inputFocused)
        {
            if (unicode == 8)
            {
                if (!companyInput.empty())
                {
                    companyInput.pop_back();
                }
            }
            else if (unicode >= 32 && unicode < 128 && companyInput.length() < 35)
            {
                companyInput += static_cast<char>(unicode);
            }
        }

        if (avoidPortFocused)
        {
            if (unicode == 8)
            {
                if (!avoidPortInput.empty())
                {
                    avoidPortInput.pop_back();
                }
            }
            else if (unicode >= 32 && unicode < 128 && avoidPortInput.length() < 35)
            {
                avoidPortInput += static_cast<char>(unicode);
            }
        }
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (!isVisible)
        {
            return -1;
        }

        inputFocused = false;
        avoidPortFocused = false;

        if (companyBox.getGlobalBounds().contains(mousePos))
        {
            inputFocused = true;
            return -1;
        }

        if (avoidPortBox.getGlobalBounds().contains(mousePos))
        {
            avoidPortFocused = true;
            return -1;
        }

        if (btnGenerate.getGlobalBounds().contains(mousePos))
        {
            return 1;
        }

        if (btnClose.getGlobalBounds().contains(mousePos))
        {
            return 0;
        }

        return -1;
    }

    void update(float deltaTime)
    {
        if (!isVisible)
        {
            return;
        }

        animPhase += deltaTime * 2.5f;
        if (animPhase > 6.28318f)
        {
            animPhase -= 6.28318f;
        }

        if (companyInput.empty())
        {
            companyText.setString("e.g.  Evergreen, MaerskLine, CMA-CGM");
            companyText.setFillColor(sf::Color(80, 100, 120));
        }
        else
        {
            companyText.setString(companyInput);
            companyText.setFillColor(sf::Color(220, 235, 255));
        }

        if (avoidPortInput.empty())
        {
            avoidPortText.setString("e.g. London, Singapore, Rotterdam");
            avoidPortText.setFillColor(sf::Color(80, 100, 120));
        }
        else
        {
            avoidPortText.setString(avoidPortInput);
            avoidPortText.setFillColor(sf::Color(255, 200, 200));
        }

        companyBox.setOutlineColor(inputFocused ? sf::Color(255, 180, 50) : sf::Color(40, 70, 100));
        companyBox.setOutlineThickness(inputFocused ? 2.f : 1.f);
        companyAccent.setSize(sf::Vector2f(inputFocused ? 5.f : 3.f, 35.f));

        avoidPortBox.setOutlineColor(avoidPortFocused ? sf::Color(255, 80, 80) : sf::Color(40, 70, 100));
        avoidPortBox.setOutlineThickness(avoidPortFocused ? 2.f : 1.f);
        avoidPortAccent.setSize(sf::Vector2f(avoidPortFocused ? 5.f : 3.f, 35.f));

        float pulse = (sin(animPhase) + 1.f) / 2.f;
        headerGlow.setFillColor(sf::Color(255, (sf::Uint8)(160 + pulse * 40), 50, (sf::Uint8)(120 + pulse * 80)));
    }

    void updateHover(sf::Vector2f mousePos, float deltaTime)
    {
        if (!isVisible)
        {
            return;
        }

        bool hover = btnGenerate.getGlobalBounds().contains(mousePos);
        generateHoverProgress = hover ? min(1.f, generateHoverProgress + deltaTime * 8.f)
            : max(0.f, generateHoverProgress - deltaTime * 5.f);
        float t = generateHoverProgress;
        btnGenerate.setFillColor(sf::Color(
            (sf::Uint8)(140 + t * 50),
            (sf::Uint8)(95 + t * 50),
            (sf::Uint8)(20 + t * 30)
        ));
        btnGenerateGlow.setOutlineColor(sf::Color(255, 200, 50, (sf::Uint8)(t * 150)));

        hover = btnClose.getGlobalBounds().contains(mousePos);
        closeHoverProgress = hover ? min(1.f, closeHoverProgress + deltaTime * 8.f)
            : max(0.f, closeHoverProgress - deltaTime * 5.f);
        t = closeHoverProgress;
        btnClose.setFillColor(sf::Color(
            (sf::Uint8)(40 + t * 80),
            (sf::Uint8)(50 - t * 20),
            (sf::Uint8)(65 - t * 30)
        ));
    }

    void draw(sf::RenderWindow& window)
    {
        if (!isVisible)
        {
            return;
        }

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

        window.draw(descriptionText);

        window.draw(companyBox);

        if (inputFocused)
        {
            float pulse = (sin(animPhase * 3.f) + 1.f) / 2.f;

            sf::RectangleShape glow = companyAccent;
            glow.setSize(sf::Vector2f(companyAccent.getSize().x + 6.f, companyAccent.getSize().y));
            glow.setPosition(companyAccent.getPosition().x - 3.f, companyAccent.getPosition().y);
            glow.setFillColor(sf::Color(255, 180, 50, (sf::Uint8)(40 + pulse * 50)));
            window.draw(glow);

            if (pulse > 0.5f)
            {
                sf::RectangleShape cursor(sf::Vector2f(2.f, 16.f));
                float textWidth = companyInput.empty() ? 0 : companyText.getLocalBounds().width;
                cursor.setPosition(companyText.getPosition().x + textWidth + 2.f, companyText.getPosition().y + 2.f);
                cursor.setFillColor(sf::Color(255, 200, 100));
                window.draw(cursor);
            }
        }

        window.draw(companyAccent);
        window.draw(companyLabel);
        window.draw(companyText);

        window.draw(avoidPortBox);

        if (avoidPortFocused)
        {
            float pulse = (sin(animPhase * 3.f) + 1.f) / 2.f;

            sf::RectangleShape glow = avoidPortAccent;
            glow.setSize(sf::Vector2f(avoidPortAccent.getSize().x + 6.f, avoidPortAccent.getSize().y));
            glow.setPosition(avoidPortAccent.getPosition().x - 3.f, avoidPortAccent.getPosition().y);
            glow.setFillColor(sf::Color(255, 80, 80, (sf::Uint8)(40 + pulse * 50)));
            window.draw(glow);

            if (pulse > 0.5f)
            {
                sf::RectangleShape cursor(sf::Vector2f(2.f, 16.f));
                float textWidth = avoidPortInput.empty() ? 0 : avoidPortText.getLocalBounds().width;
                cursor.setPosition(avoidPortText.getPosition().x + textWidth + 2.f, avoidPortText.getPosition().y + 2.f);
                cursor.setFillColor(sf::Color(255, 100, 100));
                window.draw(cursor);
            }
        }

        window.draw(avoidPortAccent);
        window.draw(avoidPortLabel);
        window.draw(avoidPortText);

        window.draw(btnGenerateGlow);
        window.draw(btnGenerate);

        sf::RectangleShape btnHighlight(sf::Vector2f(btnGenerate.getSize().x, 2.f));
        btnHighlight.setPosition(btnGenerate.getPosition());
        btnHighlight.setFillColor(sf::Color(255, 255, 255, 30));
        window.draw(btnHighlight);

        window.draw(btnGenerateText);

        drawCorners(window);
    }

    void drawCorners(sf::RenderWindow& window)
    {
        sf::Color color(255, 180, 50, 100);
        float len = 12.f, thick = 2.f;

        sf::RectangleShape h(sf::Vector2f(len, thick));
        sf::RectangleShape v(sf::Vector2f(thick, len));
        h.setFillColor(color);
        v.setFillColor(color);

        h.setPosition(panelX + 2.f, panelY + 2.f);
        v.setPosition(panelX + 2.f, panelY + 2.f);
        window.draw(h);
        window.draw(v);

        h.setPosition(panelX + panelWidth - len - 2.f, panelY + 2.f);
        v.setPosition(panelX + panelWidth - thick - 2.f, panelY + 2.f);
        window.draw(h);
        window.draw(v);

        h.setPosition(panelX + 2.f, panelY + panelHeight - thick - 2.f);
        v.setPosition(panelX + 2.f, panelY + panelHeight - len - 2.f);
        window.draw(h);
        window.draw(v);

        h.setPosition(panelX + panelWidth - len - 2.f, panelY + panelHeight - thick - 2.f);
        v.setPosition(panelX + panelWidth - thick - 2.f, panelY + panelHeight - len - 2.f);
        window.draw(h);
        window.draw(v);
    }
};


class SimpleControlPanel
{
private:
    sf::RectangleShape background;
    sf::RectangleShape headerBar;
    sf::RectangleShape headerGlow;
    sf::Text titleText;
    sf::Text subtitleText;

    struct ModernButton
    {
        sf::RectangleShape base;
        sf::RectangleShape highlight;
        sf::RectangleShape leftAccent;
        sf::Text label;
        sf::Color baseColor;
        sf::Color hoverColor;
        sf::Color accentColor;
        float hoverProgress;
        bool isHovered;

        ModernButton() : hoverProgress(0.f), isHovered(false) {}
    };

    ModernButton btnBooking;
    ModernButton btnOptimization;
    ModernButton btnMultiLeg;
    ModernButton btnSubgraph;
    ModernButton btnClear;

    sf::RectangleShape resultsPanel;
    sf::RectangleShape resultsBorder;
    sf::RectangleShape resultsHeaderBar;
    sf::Text resultsTitle;
    sf::Text resultsText;
    sf::Text statusText;
    sf::CircleShape statusDot;

    sf::RectangleShape separatorLine;

    float panelWidth;
    float panelHeight;
    float animPhase;
    sf::Font* fontPtr;

    string statusMessage;
    sf::Color statusColor;
    string resultsContent;

    void setupModernButton(ModernButton& btn, float x, float y, float w, float h,
        const string& label, sf::Color baseCol, sf::Color accentCol, sf::Font& font)
    {
        btn.baseColor = baseCol;
        btn.hoverColor = sf::Color(
            min(255, baseCol.r + 35),
            min(255, baseCol.g + 35),
            min(255, baseCol.b + 35)
        );
        btn.accentColor = accentCol;

        btn.base.setSize(sf::Vector2f(w, h));
        btn.base.setPosition(x, y);
        btn.base.setFillColor(baseCol);
        btn.base.setOutlineThickness(0.f);

        btn.highlight.setSize(sf::Vector2f(w, 2.f));
        btn.highlight.setPosition(x, y);
        btn.highlight.setFillColor(sf::Color(255, 255, 255, 25));

        btn.leftAccent.setSize(sf::Vector2f(4.f, h));
        btn.leftAccent.setPosition(x, y);
        btn.leftAccent.setFillColor(accentCol);

        btn.label.setFont(font);
        btn.label.setString(label);
        btn.label.setCharacterSize(15);
        btn.label.setFillColor(sf::Color::White);
        btn.label.setStyle(sf::Text::Bold);

        sf::FloatRect textBounds = btn.label.getLocalBounds();
        btn.label.setPosition(
            x + (w - textBounds.width) / 2.f,
            y + (h - textBounds.height) / 2.f - 4.f
        );

        btn.hoverProgress = 0.f;
        btn.isHovered = false;
    }

    void updateButtonHover(ModernButton& btn, sf::Vector2f mousePos, float deltaTime)
    {
        btn.isHovered = btn.base.getGlobalBounds().contains(mousePos);

        if (btn.isHovered)
        {
            btn.hoverProgress = min(1.f, btn.hoverProgress + deltaTime * 10.f);
        }
        else
        {
            btn.hoverProgress = max(0.f, btn.hoverProgress - deltaTime * 6.f);
        }

        float t = btn.hoverProgress;
        sf::Color currentColor(
            (sf::Uint8)(btn.baseColor.r + (btn.hoverColor.r - btn.baseColor.r) * t),
            (sf::Uint8)(btn.baseColor.g + (btn.hoverColor.g - btn.baseColor.g) * t),
            (sf::Uint8)(btn.baseColor.b + (btn.hoverColor.b - btn.baseColor.b) * t)
        );

        btn.base.setFillColor(currentColor);

        float accentWidth = 4.f + t * 4.f;
        btn.leftAccent.setSize(sf::Vector2f(accentWidth, btn.base.getSize().y));

        btn.leftAccent.setFillColor(sf::Color(
            (sf::Uint8)min(255, btn.accentColor.r + (int)(t * 50)),
            (sf::Uint8)min(255, btn.accentColor.g + (int)(t * 50)),
            (sf::Uint8)min(255, btn.accentColor.b + (int)(t * 50))
        ));
    }

    void drawModernButton(sf::RenderWindow& window, ModernButton& btn, float glowTime)
    {
        sf::RectangleShape shadow = btn.base;
        shadow.setPosition(btn.base.getPosition().x + 2.f, btn.base.getPosition().y + 2.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 50));
        window.draw(shadow);

        window.draw(btn.base);

        window.draw(btn.highlight);

        if (btn.hoverProgress > 0)
        {
            sf::RectangleShape glow = btn.leftAccent;
            glow.setSize(sf::Vector2f(btn.leftAccent.getSize().x + 6.f, btn.leftAccent.getSize().y));
            glow.setPosition(btn.leftAccent.getPosition().x - 3.f, btn.leftAccent.getPosition().y);
            glow.setFillColor(sf::Color(
                btn.accentColor.r,
                btn.accentColor.g,
                btn.accentColor.b,
                (sf::Uint8)(btn.hoverProgress * 100)
            ));
            window.draw(glow);
        }

        window.draw(btn.leftAccent);

        sf::Text labelCopy = btn.label;
        if (btn.hoverProgress > 0)
        {
            labelCopy.setPosition(
                btn.label.getPosition().x + btn.hoverProgress * 3.f,
                btn.label.getPosition().y
            );
        }
        window.draw(labelCopy);

        if (btn.hoverProgress > 0.3f)
        {
            float alpha = (btn.hoverProgress - 0.3f) / 0.7f;
            float arrowX = btn.base.getPosition().x + btn.base.getSize().x - 25.f + btn.hoverProgress * 5.f;
            float arrowY = btn.base.getPosition().y + btn.base.getSize().y / 2.f;

            sf::ConvexShape arrow;
            arrow.setPointCount(3);
            arrow.setPoint(0, sf::Vector2f(10.f, 0.f));
            arrow.setPoint(1, sf::Vector2f(0.f, -6.f));
            arrow.setPoint(2, sf::Vector2f(0.f, 6.f));
            arrow.setPosition(arrowX, arrowY);
            arrow.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha * 200)));
            window.draw(arrow);
        }
    }

public:
    SimpleControlPanel() :
        panelWidth(300.f), panelHeight(0.f), animPhase(0.f), fontPtr(nullptr)
    {
        statusMessage = "Ready";
        statusColor = sf::Color(100, 255, 150);
        resultsContent = "";
    }

    void initialize(float windowWidth, float windowHeight, sf::Font& font)
    {
        fontPtr = &font;
        panelHeight = windowHeight;

        background.setSize(sf::Vector2f(panelWidth, panelHeight));
        background.setPosition(0, 0);
        background.setFillColor(sf::Color(10, 18, 30, 252));

        headerBar.setSize(sf::Vector2f(panelWidth, 80.f));
        headerBar.setPosition(0, 0);
        headerBar.setFillColor(sf::Color(15, 25, 45));

        headerGlow.setSize(sf::Vector2f(panelWidth, 2.f));
        headerGlow.setPosition(0, 80.f);
        headerGlow.setFillColor(sf::Color(0, 180, 255, 200));

        titleText.setFont(font);
        titleText.setString("OCEAN ROUTE");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color(0, 210, 255));
        titleText.setStyle(sf::Text::Bold);
        titleText.setLetterSpacing(2.f);
        titleText.setPosition(18.f, 15.f);

        subtitleText.setFont(font);
        subtitleText.setString("Navigation System");
        subtitleText.setCharacterSize(12);
        subtitleText.setFillColor(sf::Color(100, 140, 170));
        subtitleText.setLetterSpacing(1.5f);
        subtitleText.setPosition(18.f, 52.f);

        float btnX = 15.f;
        float btnY = 100.f;
        float btnWidth = panelWidth - 30.f;
        float btnHeight = 52.f;
        float btnSpacing = 62.f;

        setupModernButton(btnBooking, btnX, btnY, btnWidth, btnHeight,
            "BOOKING",
            sf::Color(20, 70, 120), sf::Color(0, 200, 255), font);

        setupModernButton(btnOptimization, btnX, btnY + btnSpacing, btnWidth, btnHeight,
            "OPTIMIZATION",
            sf::Color(55, 40, 110), sf::Color(140, 100, 255), font);

        setupModernButton(btnMultiLeg, btnX, btnY + btnSpacing * 2, btnWidth, btnHeight,
            "MULTI-LEG ROUTE",
            sf::Color(15, 85, 85), sf::Color(0, 220, 180), font);

        setupModernButton(btnSubgraph, btnX, btnY + btnSpacing * 3, btnWidth, btnHeight,
            "SUBGRAPH",
            sf::Color(110, 70, 15), sf::Color(255, 180, 50), font);

        setupModernButton(btnClear, btnX, btnY + btnSpacing * 4, btnWidth, btnHeight,
            "CLEAR ALL",
            sf::Color(50, 50, 55), sf::Color(120, 120, 130), font);

        separatorLine.setSize(sf::Vector2f(panelWidth - 50.f, 1.f));
        separatorLine.setPosition(25.f, btnY + btnSpacing * 5 + 15.f);
        separatorLine.setFillColor(sf::Color(50, 70, 90, 150));

        float resultsPanelY = panelHeight - 210.f;

        resultsPanel.setSize(sf::Vector2f(panelWidth - 30.f, 170.f));
        resultsPanel.setPosition(15.f, resultsPanelY);
        resultsPanel.setFillColor(sf::Color(12, 22, 38, 230));
        resultsPanel.setOutlineThickness(1.f);
        resultsPanel.setOutlineColor(sf::Color(40, 70, 100, 180));

        resultsHeaderBar.setSize(sf::Vector2f(panelWidth - 30.f, 32.f));
        resultsHeaderBar.setPosition(15.f, resultsPanelY);
        resultsHeaderBar.setFillColor(sf::Color(25, 45, 70, 220));

        resultsBorder.setSize(sf::Vector2f(panelWidth - 28.f, 172.f));
        resultsBorder.setPosition(14.f, resultsPanelY - 1.f);
        resultsBorder.setFillColor(sf::Color::Transparent);
        resultsBorder.setOutlineThickness(1.f);
        resultsBorder.setOutlineColor(sf::Color(0, 150, 200, 80));

        resultsTitle.setFont(font);
        resultsTitle.setString("ROUTE RESULTS");
        resultsTitle.setCharacterSize(12);
        resultsTitle.setFillColor(sf::Color(0, 200, 255));
        resultsTitle.setStyle(sf::Text::Bold);
        resultsTitle.setLetterSpacing(1.5f);
        resultsTitle.setPosition(25.f, resultsPanelY + 8.f);

        resultsText.setFont(font);
        resultsText.setString("No route selected\n\nSelect an option above\nto find a route");
        resultsText.setCharacterSize(13);
        resultsText.setFillColor(sf::Color(130, 145, 165));
        resultsText.setLineSpacing(1.3f);
        resultsText.setPosition(25.f, resultsPanelY + 45.f);

        statusDot.setRadius(4.f);
        statusDot.setOrigin(4.f, 4.f);
        statusDot.setPosition(25.f, panelHeight - 25.f);
        statusDot.setFillColor(sf::Color(100, 255, 150));

        statusText.setFont(font);
        statusText.setString("Ready");
        statusText.setCharacterSize(13);
        statusText.setFillColor(sf::Color(100, 255, 150));
        statusText.setPosition(38.f, panelHeight - 33.f);
    }

    void setStatus(const string& msg, sf::Color color)
    {
        statusMessage = msg;
        statusColor = color;
        statusText.setString(msg);
        statusText.setFillColor(color);
        statusDot.setFillColor(color);
    }

    void setResults(const string& content)
    {
        resultsContent = content;
        resultsText.setString(content);
        resultsText.setFillColor(sf::Color(200, 215, 235));
    }

    float getPanelWidth() 
    { 
        return panelWidth; 
    }

    void update(float deltaTime)
    {
        animPhase += deltaTime * 2.5f;
        if (animPhase > 6.28318f) 
        {
            animPhase -= 6.28318f;
        }

        float glowPulse = (sin(animPhase) + 1.f) / 2.f;
        headerGlow.setFillColor(sf::Color(
            0,
            (sf::Uint8)(160 + glowPulse * 60),
            255,
            (sf::Uint8)(150 + glowPulse * 100)
        ));

        float statusPulse = (sin(animPhase * 2.5f) + 1.f) / 2.f;
        statusDot.setRadius(4.f + statusPulse * 2.f);
        statusDot.setOrigin(statusDot.getRadius(), statusDot.getRadius());

        resultsBorder.setOutlineColor(sf::Color(
            0,
            (sf::Uint8)(100 + glowPulse * 80),
            (sf::Uint8)(180 + glowPulse * 50),
            (sf::Uint8)(60 + glowPulse * 60)
        ));
    }

    void updateHover(sf::Vector2f mousePos, float deltaTime)
    {
        updateButtonHover(btnBooking, mousePos, deltaTime);
        updateButtonHover(btnOptimization, mousePos, deltaTime);
        updateButtonHover(btnMultiLeg, mousePos, deltaTime);
        updateButtonHover(btnSubgraph, mousePos, deltaTime);
        updateButtonHover(btnClear, mousePos, deltaTime);
    }

    int handleClick(sf::Vector2f mousePos)
    {
        if (btnBooking.base.getGlobalBounds().contains(mousePos)) 
        {
            return 1;
        }
        if (btnOptimization.base.getGlobalBounds().contains(mousePos))
        {
            return 2;
        }
        if (btnMultiLeg.base.getGlobalBounds().contains(mousePos)) 
        {
            return 3;
        }
        if (btnSubgraph.base.getGlobalBounds().contains(mousePos)) 
        {
            return 4;
        }
        if (btnClear.base.getGlobalBounds().contains(mousePos)) 
        {
            return 5;
        }

        return 0;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background);

        for (int i = 0; i < 3; i++)
        {
            sf::RectangleShape line(sf::Vector2f(1.f, panelHeight));
            line.setPosition(panelWidth - 5.f - i * 2.f, 0);
            line.setFillColor(sf::Color(30, 50, 70, 40 - i * 12));
            window.draw(line);
        }

        window.draw(headerBar);
        window.draw(headerGlow);
        window.draw(titleText);
        window.draw(subtitleText);

        drawModernButton(window, btnBooking, animPhase);
        drawModernButton(window, btnOptimization, animPhase);
        drawModernButton(window, btnMultiLeg, animPhase);
        drawModernButton(window, btnSubgraph, animPhase);
        drawModernButton(window, btnClear, animPhase);

        window.draw(separatorLine);

        window.draw(resultsBorder);
        window.draw(resultsPanel);
        window.draw(resultsHeaderBar);
        window.draw(resultsTitle);
        window.draw(resultsText);

        window.draw(statusDot);
        window.draw(statusText);

        drawCornerDecorations(window);
    }

    void drawCornerDecorations(sf::RenderWindow& window)
    {
        sf::Color cornerColor(0, 180, 255, 80);
        float bracketLen = 15.f;
        float bracketThick = 2.f;

        sf::RectangleShape cornerH(sf::Vector2f(bracketLen, bracketThick));
        sf::RectangleShape cornerV(sf::Vector2f(bracketThick, bracketLen));

        cornerH.setFillColor(cornerColor);
        cornerV.setFillColor(cornerColor);

        cornerH.setPosition(3.f, 3.f);
        cornerV.setPosition(3.f, 3.f);
        window.draw(cornerH);
        window.draw(cornerV);

        cornerH.setPosition(panelWidth - bracketLen - 3.f, 3.f);
        cornerV.setPosition(panelWidth - bracketThick - 3.f, 3.f);
        window.draw(cornerH);
        window.draw(cornerV);

        cornerH.setPosition(3.f, panelHeight - bracketThick - 3.f);
        cornerV.setPosition(3.f, panelHeight - bracketLen - 3.f);
        window.draw(cornerH);
        window.draw(cornerV);

        cornerH.setPosition(panelWidth - bracketLen - 3.f, panelHeight - bracketThick - 3.f);
        cornerV.setPosition(panelWidth - bracketThick - 3.f, panelHeight - bracketLen - 3.f);
        window.draw(cornerH);
        window.draw(cornerV);
    }
};