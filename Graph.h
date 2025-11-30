#pragma once    
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct HeapNode
{
    int index;
    int cost;
};

struct PriorityQueue
{
    HeapNode nodes[40];
    int size;

    PriorityQueue()
    {
        size = 0;
    }

    void swapNodes(int a, int b)
    {
        HeapNode temp = nodes[a];
        nodes[a] = nodes[b];
        nodes[b] = temp;
    }

    void enqueue(int index, int cost)
    {
        nodes[size].index=index;
        nodes[size].cost=cost;
        size++;
        int currentIndex = size - 1;
        while(currentIndex>0 && nodes[currentIndex].cost < nodes[(currentIndex - 1) / 2].cost)
        {
            swapNodes(currentIndex, (currentIndex - 1) / 2);
            currentIndex = (currentIndex - 1) / 2;
        }
    }

    HeapNode dequeue()
    {
        HeapNode minNode = nodes[0];
        nodes[0] = nodes[size - 1];
        size--;
        int currentIndex = 0;
        while (true)
        {
            int leftChildIndex = 2 * currentIndex + 1;
            int rightChildIndex = 2 * currentIndex + 2;
            int smallestIndex = currentIndex;

            if (leftChildIndex < size && nodes[leftChildIndex].cost < nodes[smallestIndex].cost)
            {
                smallestIndex = leftChildIndex;
            }

            if (rightChildIndex < size && nodes[rightChildIndex].cost < nodes[smallestIndex].cost)
            {
                smallestIndex = rightChildIndex;
            }

            if (smallestIndex != currentIndex)
            {
                swapNodes(currentIndex, smallestIndex);
                currentIndex = smallestIndex;
            }
            else
            {
                break;
            }
        }
        return minNode;
    }

        bool isEmpty()
        {
            if(size == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    
};

struct Route
{
    string destination;
    string date;
    string depTime;
    string arrTime;
    int vCost;
    string company;
    Route* next;

    Route(string dest, string d, string dTime, string aTime, int cost, string comp)
    {
        destination = dest;
        date = d;
        depTime = dTime;
        arrTime = aTime;
        vCost = cost;
        company = comp;
        next = NULL;
    }
};

struct Vertex
{
    string port;
    int portCharge;
};

struct Graph
{
    int vertices;
    Route** edges;
    Vertex ports[40];
    Route* head;

    static const int totalRoutes = 100;
    static const int totalDepth = 10;

    struct FoundPath
    {
        Route* routes[totalDepth];
        int length;
        int totalCost;

        FoundPath()
        {
            length = 0;
            totalCost = 0;
            for (int i = 0; i < totalDepth; i++)
            {
                routes[i] = NULL;
            }
        }
    };

    FoundPath foundPaths[totalRoutes];
    int pathCount;

    Graph()
    {
        vertices = 40;
        edges = new Route * [vertices];
        for (int i = 0; i < vertices; i++)
        {
            edges[i] = NULL;
        }
        readPortCharges("PortCharges.txt");
        head = NULL;
        pathCount = 0;
    }

    void readPortCharges(string fname)
    {
        ifstream fin(fname);
        if (!fin)
        {
            cout << "Error loading port charges file" << endl;
            return;
        }
        string portName;
        int charge;
        int index = 0;


        while (fin >> portName >> charge && index < vertices) {
            ports[index].port = portName;
            ports[index].portCharge = charge;
            index++;
        }
        fin.close();
    }

    void addToGraph(Route* route, string origin, int index)
    {
        if (index < 0 || index >= vertices)
        {
            return;
        }
        if (edges[index] == NULL)
        {
            edges[index] = route;
            return;
        }
        head = edges[index];
        Route* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = route;
    }

    int getPortIndex(string name)
    {
        for (int i = 0; i < vertices; i++)
        {
            if (name == ports[i].port)
            {
                return i;
            }
        }
        return -1;
    }

    void loadFile(string fname)
    {
        ifstream fin(fname);
        if (!fin)
        {
            cout << "Sorry. There was an error while loading the file" << endl;
            return;
        }
        string origin, destination, date, depTime, arrTime, company;
        int vCost;
        int count = 0;
        while (fin >> origin >> destination >> date >> depTime >> arrTime >> vCost >> company)
        {
            Route* route = new Route(destination, date, depTime, arrTime, vCost, company);
            int index = getPortIndex(origin);
            addToGraph(route, origin, index);
            count++;
        }
        cout << "Successfully loaded file" << endl << endl;
        fin.close();
    }

    void printGraph()
    {
        for (int i = 0; i < vertices; i++)
        {
            Route* temp = edges[i];
            cout << ports[i].port << ": ";
            while (temp != NULL)
            {
                cout << " --> " << temp->destination;
                temp = temp->next;
            }
            cout << endl;
        }
    }

    void displayPortRoutes(string portName)
    {
        int portIndex = getPortIndex(portName);
        if (portIndex == -1)
        {
            cout << "The port that you entered is invalid" << endl;
            return;
        }

        cout << "\nRoutes from " << portName << ": " << endl;
        Route* temp = edges[portIndex];
        int count = 0;
        while (temp != NULL)
        {
            count++;
            cout << count << "." << endl;
            cout << "Destination: " << temp->destination << endl;
            cout << "Date: " << temp->date << endl;
            cout << "Departure Time: " << temp->depTime << endl;
            cout << "Arrival Time: " << temp->arrTime << endl;
            cout << "Cost: " << temp->vCost << "$" << endl;
            cout << "Company: " << temp->company << endl;
            temp = temp->next;
        }

        if (count == 0)
        {
            cout << "No routes have been found from this port." << endl;
        }
        else
        {
            cout << "Total: " << count << " routes" << endl;
        }
    }

    int timeToMinutes(string time)
    {
        int hrs = 0;
        int mins = 0;

        int colonPosition = -1;
        for (int i = 0; i < time.length(); i++)
        {
            if (time[i] == ':')
            {
                colonPosition = i;
                break;
            }
        }

        if (colonPosition == -1)
        {
            return -1;
        }

        for (int i = 0; i < colonPosition; i++)
        {
            hrs = hrs * 10 + (time[i] - '0');
        }

        for (int i = colonPosition + 1; i < time.length(); i++)
        {
            mins = mins * 10 + (time[i] - '0');
        }
        return (hrs * 60 + mins);
    }

    int dateConversion(string date)
    {
        int day = 0;
        int month = 0;
        int year = 0;
        int position = 0;

        while (position < date.length() && date[position] != '/')
        {
            day = day * 10 + (date[position] - '0');
            position++;
        }
        position++;

        while (position < date.length() && date[position] != '/')
        {
            month = month * 10 + (date[position] - '0');
            position++;
        }
        position++;

        while (position < date.length())
        {
            year = year * 10 + (date[position] - '0');
            position++;
        }

        return year * 10000 + month * 100 + day;
    }

    string getNextDate(string currentDate)
    {
        int day = 0;
        int month = 0;
        int year = 0;
        int position = 0;
        int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        while (position < currentDate.length() && currentDate[position] != '/')
        {
            day = day * 10 + (currentDate[position] - '0');
            position++;
        }
        position++;

        while (position < currentDate.length() && currentDate[position] != '/')
        {
            month = month * 10 + (currentDate[position] - '0');
            position++;
        }
        position++;

        while (position < currentDate.length())
        {
            year = year * 10 + (currentDate[position] - '0');
            position++;
        }

        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            daysInMonth[2] = 29;
        }

        day++;
        if (day > daysInMonth[month])
        {
            day = 1;
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
            }
        }

        string result = "";
        if (day < 10) 
        {
            result += "0";
        }
        result = result + to_string(day) + "/";
        if (month < 10) 
        {
            result += "0";
        }
        result = result + to_string(month) + "/";
        result = result + to_string(year);

        return result;
    }

    bool isTimeValid(Route* prevRoute, Route* nextRoute)
    {
        int arrivalMinutes = timeToMinutes(prevRoute->arrTime);
        int departureMinutes = timeToMinutes(nextRoute->depTime);
        int minLayover = 15;

        if (arrivalMinutes == -1 || departureMinutes == -1)
        {
            return false;
        }

        int prevDate = dateConversion(prevRoute->date);
        int nextDate = dateConversion(nextRoute->date);

        if (prevDate == nextDate)
        {
            if (departureMinutes >= arrivalMinutes + minLayover)
            {
                return true;
            }
            return false;
        }

        string expectedNextDate = getNextDate(prevRoute->date);
        int expectedNextDateInt = dateConversion(expectedNextDate);

        if (nextDate >= expectedNextDateInt)
        {
            int minutesUntilMidnight = (24 * 60) - arrivalMinutes;
            int totalLayover = minutesUntilMidnight + departureMinutes;

            if (totalLayover >= minLayover)
            {
                return true; 
            }
        }
        return false;
    }

    bool isInPath(string port, string path[], int pathLength)
    {
        for (int i = 0; i < pathLength; i++)
        {
            if (path[i] == port)
            {
                return true;
            }
        }
        return false;
    }

    bool isDuplicatePath(Route* newPath[], int newLength)
    {
        for (int i = 0; i < pathCount; i++)
        {
            if (foundPaths[i].length != newLength)
            {
                continue;
            }

            bool isSame = true;
            for (int j = 0; j < newLength; j++)
            {
                if (foundPaths[i].routes[j] != newPath[j])
                {
                    isSame = false;
                    break;
                }
            }

            if (isSame != false)
            {
                return true;
            }
        }
        return false;
    }

    bool iterativeDeepeningSearch(string currPort, string destination, string searchDate,int depthLimit, Route* currentPath[], string visitedPorts[], int currentDepth, bool isFirstLeg)
    {
        if (currPort == destination && currentDepth > 0)
        {
            if (!isDuplicatePath(currentPath, currentDepth) && pathCount < totalRoutes)
            {
                foundPaths[pathCount].totalCost = 0;
                for (int i = 0; i < currentDepth; i++)
                {
                    foundPaths[pathCount].routes[i] = currentPath[i];
                    foundPaths[pathCount].totalCost = foundPaths[pathCount].totalCost + currentPath[i]->vCost;
                }
                foundPaths[pathCount].length = currentDepth;
                pathCount++;
            }
            return true;
        }

        if (currentDepth >= depthLimit)
        {
            return false;
        }

        int currentIndex = getPortIndex(currPort);
        if (currentIndex == -1)
        {
            return false;
        }

        visitedPorts[currentDepth] = currPort;

        bool isFound = false;

        Route* temp = edges[currentIndex];
        while (temp != NULL)
        {
            bool isDateValid = false;

            if (isFirstLeg)
            {
                isDateValid = (temp->date == searchDate);
            }
            else
            {
                isDateValid = true;
            }

            if (isDateValid && !isInPath(temp->destination, visitedPorts, currentDepth + 1))
            {
                bool isValidTime = true;

                if (!isFirstLeg && currentDepth > 0)
                {
                    Route* previousRoute = currentPath[currentDepth - 1];
                    isValidTime = isTimeValid(previousRoute, temp);
                }

                if (isValidTime != false)
                {
                    currentPath[currentDepth] = temp;

                    if (iterativeDeepeningSearch(temp->destination, destination, searchDate, depthLimit, currentPath, visitedPorts, currentDepth + 1, false))
                    {
                        isFound = true;
                    }
                }
            }
            temp = temp->next;
        }
        return isFound;
    }

    void searchRoutes(string source, string destination, string searchDate)
    {
        int sourceIndex = getPortIndex(source);
        int destIndex = getPortIndex(destination);

        if (sourceIndex == -1)
        {
            cout << "You have entered an invalid source port" << endl;
            return;
        }

        if (destIndex == -1)
        {
            cout << "You have entered an invalid destination port" << endl;
            return;
        }

        if (source == destination)
        {
            cout << "The source and destination cannot be the same" << endl;
            return;
        }
        pathCount = 0;

        for (int i = 0; i < totalRoutes; i++)
        {
            foundPaths[i].length = 0;
            foundPaths[i].totalCost = 0;
            for (int j = 0; j < totalDepth; j++)
            {
                foundPaths[i].routes[j] = NULL;
            }
        }

        for (int depth = 1; depth <= totalDepth; depth++)
        {
            Route* currentPath[totalDepth];
            string visitedCities[totalDepth];

            iterativeDeepeningSearch(source, destination, searchDate, depth, currentPath, visitedCities, 0, true);
        }

        if (pathCount == 0)
        {
            cout << "Sorry, no routes have been found" << endl;
            return;
        }
        sortPathsByLength();
        displayFoundRoutes(source, destination);
    }

    void sortPathsByLength()
    {
        for (int i = 0; i < pathCount - 1; i++)
        {
            for (int j = 0; j < pathCount - i - 1; j++)
            {
                if (foundPaths[j].length > foundPaths[j + 1].length)
                {
                    FoundPath temp = foundPaths[j];
                    foundPaths[j] = foundPaths[j + 1];
                    foundPaths[j + 1] = temp;
                }
            }
        }
    }

    void displayFoundRoutes(string source, string destination)
    {
        int currentStops = -1;
        for (int i = 0; i < pathCount; i++)
        {
            int stops = foundPaths[i].length - 1;
            if (stops != currentStops)
            {
                currentStops = stops;
                cout << endl;
                if (stops == 0)
                {
                    cout << "Direct Route: " << endl;
                }
                else if (stops == 1)
                {
                    cout << "Route having 1 stop: " << endl;
                }
                else
                {
                    cout << "Route having " << stops << " stops: " << endl;
                }
                cout << endl;
            }

            if (stops == 0)
            {
                cout << source << " --> " << destination << endl;
                Route* r = foundPaths[i].routes[0];
                cout << "  Date: " << r->date << endl;
                cout << "  Departure Time: " << r->depTime << endl;
                cout << "  Arrival Time: " << r->arrTime << endl;
                cout << "  Cost: " << r->vCost <<"$" << endl;
                cout << "  Company: " << r->company << endl;
            }
            else
            {
                cout << "Route " << (i + 1) << ":" << endl;
                cout << "Path: " << source;
                for (int j = 0; j < foundPaths[i].length; j++)
                {
                    cout << " --> " << foundPaths[i].routes[j]->destination;
                }
                cout << endl << endl;
                string currentPort = source;
                for (int j = 0; j < foundPaths[i].length; j++)
                {
                    Route* r = foundPaths[i].routes[j];
                    cout << "Leg " << (j + 1) << ": " << "\n" << currentPort << " to " << r->destination << endl;
                    cout << "Date: " << r->date << endl;
                    cout << "Departure Time: " << r->depTime << endl;
                    cout << "Arrival Time: " << r->arrTime << endl;

                    // Check if this leg arrives next day
                    int legDepMin = timeToMinutes(r->depTime);
                    int legArrMin = timeToMinutes(r->arrTime);
                    if (legArrMin < legDepMin)
                    {
                        cout << "(Arrival on next day)" << endl;
                    }
                    
                    cout << "Cost: " << r->vCost << "$" << endl;
                    cout << "Company: " << r->company << endl;

                    if (j < foundPaths[i].length - 1)
                    {
                        Route* nextRoute = foundPaths[i].routes[j + 1];
                    int arrivalMinutes = timeToMinutes(r->arrTime);
                    int departureMinutes = timeToMinutes(nextRoute->depTime);

                    int arrivalDateInt = dateConversion(r->date);
                    int departureDateInt = dateConversion(nextRoute->date);
                    
                    int actualArrivalDate = arrivalDateInt;
                    if (legArrMin < legDepMin)
                    {
                        string nextDay = getNextDate(r->date);
                        actualArrivalDate = dateConversion(nextDay);
                    }
                    
                    int layoverTime = 0;

                    if (actualArrivalDate == departureDateInt)
                    {
                        layoverTime = departureMinutes - arrivalMinutes;
                    }
                    else if (departureDateInt > actualArrivalDate)
                    {
                        int minutesUntilMidnight = (24 * 60) - arrivalMinutes;
                        
                        int daysDiff = departureDateInt - actualArrivalDate;
                        
                        if (daysDiff == 1)
                        {
                            layoverTime = minutesUntilMidnight + departureMinutes;
                        }
                        else
                        {
                            layoverTime = minutesUntilMidnight + ((daysDiff - 1) * 24 * 60) + departureMinutes;
                        }
                    }


                        int layoverHours = layoverTime / 60;
                        int layoverMinutes = layoverTime % 60;
                        cout << "Layover at " << r->destination << ": " << layoverHours << "hrs " << layoverMinutes << "mins";

                        if (arrivalDateInt != departureDateInt)
                        {
                            cout << " (departure on next day)";
                        }
                        cout << endl;
                    }
                    cout << endl;

                    currentPort = r->destination;
                }

                cout << "  Total Cost: " << foundPaths[i].totalCost << "$" << endl;
            }
            cout << endl;
        }

        cout << "Total routes found: " << pathCount << endl;
    }

    //================== DIJKSTRA ALGORITHM =====================

    void dijkstraMinCost(string src, string dest)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(src);
        int destIndex = getPortIndex(dest);

        int costs[40];
        bool visited[40];
        int parents[40];

        Route* bestRoute[40];

        for(int i=0;i<numPorts;i++)
        {
            costs[i] = 1e9;
            visited[i] = false;
            parents[i] = -1;

            bestRoute[i] = NULL;
        }

        costs[srcIndex] = 0;
        PriorityQueue pq;
        pq.enqueue(srcIndex, 0);

        bestRoute[srcIndex] = NULL;

        while(!pq.isEmpty())
        {
            HeapNode currentNode =pq.dequeue();
            int currentNodeIndex =currentNode.index;
            
            if(visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;
            }

            Route* currentRoute = bestRoute[currentNodeIndex]; 
            Route* temp = edges[currentNodeIndex];

            while(temp!=NULL)
            {
                int neighbor = getPortIndex(temp->destination);
                int cost = temp->vCost;
                int portCharge = ports[neighbor].portCharge;
                cost += portCharge;

                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL) {
                    canTravel = true;
                } else {
                    canTravel = isTimeValid(bestRoute[currentNodeIndex], temp);
                }
                
                if(canTravel && costs[currentNodeIndex] + cost < costs[neighbor])
                {
                    costs[neighbor]= costs[currentNodeIndex]+cost;
                    parents[neighbor]=currentNodeIndex;
                    bestRoute[neighbor] = temp;
                    pq.enqueue(neighbor,costs[neighbor]);
                }
                temp=temp->next;
            }
        }

        if(costs[destIndex]==1e9)
        {
            cout<<"No path exists from "<<src<<" to "<<dest<<endl;
            return;
        }

        cout<<"Minimum cost from "<<src<<" to "<<dest<<" is: "<<costs[destIndex]<<"$"<<endl;
        cout<<"Path: ";

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = parents[v])
            path[count++] = v;

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl<<endl;

        cout << "Detailed Route Information"<<endl;
        for (int i = count - 1; i > 0; i--) {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = edges[from];
            while (temp != NULL) {
                if (temp->destination == ports[to].port) {
                    cout << "\nLeg " << (count - i)
                        << ": " << ports[from].port << " -> " << ports[to].port << endl;
                    cout << "Date: " << temp->date << endl;
                    cout << "Departure Time: " << temp->depTime << endl;
                    cout << "Arrival Time: " << temp->arrTime << endl;
                    cout << "Cost: " << temp->vCost << "$" << endl;
                    cout << "Company: " << temp->company << endl;

                    if (i > 1) {
                        Route* nextR = edges[to];
                        while (nextR != NULL) {
                            if (nextR->destination == ports[path[i - 2]].port) {
                                int arrMins = timeToMinutes(temp->arrTime);
                                int depMins = timeToMinutes(nextR->depTime);
                                int diff = depMins - arrMins;
                                bool nextDay = false;
                                if (diff < 0) {
                                    diff += 24 * 60;
                                    nextDay = true;
                                }
                                int hrs = diff / 60;
                                int mins = diff % 60;
                                cout << "Layover at " << ports[to].port << ": "
                                    << hrs << "hrs " << mins << "mins";
                                if (nextDay)
                                    cout << " (departure on next day)";
                                cout << endl;
                                break;
                            }
                            nextR = nextR->next;
                        }
                    }
                    break;
                }
                temp = temp->next;
            }
        }
    }

    void dijkstraMinTime(string src, string dest)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(src);
        int destIndex = getPortIndex(dest);

        int times[40];
        bool visited[40];
        int parents[40];

        Route* bestRoute[40];

        for(int i=0;i<numPorts;i++)
        {
            times[i] = 1e9;
            visited[i] = false;
            parents[i] = -1;

            bestRoute[i] = NULL;
        }

        times[srcIndex] = 0;
        PriorityQueue pq;
        pq.enqueue(srcIndex, 0);

        bestRoute[srcIndex] = NULL;

        while(!pq.isEmpty())
        {
            HeapNode currentNode =pq.dequeue();
            int currentNodeIndex =currentNode.index;
            
            if(visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;
            }

            Route* currentRoute = bestRoute[currentNodeIndex]; 
            Route* temp = edges[currentNodeIndex];

            while(temp!=NULL)
            {
                int neighbor = getPortIndex(temp->destination);
                int time = timeToMinutes(temp->arrTime)- timeToMinutes(temp->depTime);
                
                if (time < 0) //next day arrival
                {
                    time += 24 * 60; 
                }
    

                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL) {
                    canTravel = true;
                } else {
                    canTravel = isTimeValid(bestRoute[currentNodeIndex], temp);
                }
                
                if(canTravel && times[currentNodeIndex]< times[neighbor])
                {
                    times[neighbor]= times[currentNodeIndex]+time;
                    parents[neighbor]=currentNodeIndex;
                    bestRoute[neighbor] = temp;
                    pq.enqueue(neighbor,times[neighbor]);
                }
                temp=temp->next;
            }
        }

        if(times[destIndex]==1e9)
        {
            cout<<"No path exists from "<<src<<" to "<<dest<<endl;
            return;
        }

        cout<<"Minimum time from "<<src<<" to "<<dest<<" is: "<<times[destIndex]<<" mins "<<endl;
        cout<<"Path: ";

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = parents[v])
            path[count++] = v;

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl<<endl;

        cout << "Detailed Route Information"<<endl;
        for (int i = count - 1; i > 0; i--) {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = edges[from];
            while (temp != NULL) {
                if (temp->destination == ports[to].port) {
                    cout << "\nLeg " << (count - i)
                        << ": " << ports[from].port << " -> " << ports[to].port << endl;
                    cout << "Date: " << temp->date << endl;
                    cout << "Departure Time: " << temp->depTime << endl;
                    cout << "Arrival Time: " << temp->arrTime << endl;
                    cout << "Cost: " << temp->vCost << "$" << endl;
                    cout << "Company: " << temp->company << endl;

                    if (i > 1) {
                        Route* nextR = edges[to];
                        while (nextR != NULL) {
                            if (nextR->destination == ports[path[i - 2]].port) {
                                int arrMins = timeToMinutes(temp->arrTime);
                                int depMins = timeToMinutes(nextR->depTime);
                                int diff = depMins - arrMins;
                                bool nextDay = false;
                                if (diff < 0) {
                                    diff += 24 * 60;
                                    nextDay = true;
                                }
                                int hrs = diff / 60;
                                int mins = diff % 60;
                                cout << "Layover at " << ports[to].port << ": "
                                    << hrs << "hrs " << mins << "mins";
                                if (nextDay)
                                    cout << " (departure on next day)";
                                cout << endl;
                                break;
                            }
                            nextR = nextR->next;
                        }
                    }
                    break;
                }
                temp = temp->next;
            }
        }
    }

    

};

