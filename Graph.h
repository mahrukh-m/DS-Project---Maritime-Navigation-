#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "SFMLrun.h"
using namespace std;

struct HeapNode
{
    int index;
    int cost;
    int heuristic;

    HeapNode()
    {
        index = 0;
        cost = 0;
        heuristic = 0;
    }
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

    void enqueue(int index, int cost, int heuristic = 0)
    {
        nodes[size].index = index;
        nodes[size].cost = cost;
        nodes[size].heuristic = heuristic;
        size++;
        int currentIndex = size - 1;
        int fValue = cost + heuristic;
        while (currentIndex > 0)
        {
            int parentIdx = (currentIndex - 1) / 2;
            int parentF = nodes[parentIdx].cost + nodes[parentIdx].heuristic;
            if (fValue < parentF)
            {
                swapNodes(currentIndex, parentIdx);
                currentIndex = parentIdx;
            }
            else
            {
                break;
            }
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
            int currentF = nodes[currentIndex].cost + nodes[currentIndex].heuristic;
            if (leftChildIndex < size)
            {
                int leftF = nodes[leftChildIndex].cost + nodes[leftChildIndex].heuristic;
                if (leftF < currentF)
                {
                    smallestIndex = leftChildIndex;
                    currentF = leftF;
                }
            }

            if (rightChildIndex < size)
            {
                int rightF = nodes[rightChildIndex].cost + nodes[rightChildIndex].heuristic;
                if (rightF < currentF)
                {
                    smallestIndex = rightChildIndex;
                }
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
        if (size == 0)
        {
            return true;
        }
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

struct DockingShip
{
    string shipName;
    string arrivalTime;
    string arrivalDate;
    string departureTime;
    string departureDate;
    int waitTimeMinutes;
    bool isProcessing; 

    DockingShip* next;

    DockingShip()
    {
        shipName = "";
        arrivalTime = "";
        arrivalDate = "";
        departureTime = "";
        departureDate = "";
        waitTimeMinutes = 0;
        isProcessing = false;
        next = NULL;
    }

    DockingShip(string name, string arrTime, string arrDate)
    {
        shipName = name;
        arrivalTime = arrTime;
        arrivalDate = arrDate;
        departureTime = "";
        departureDate = "";
        waitTimeMinutes = 0;
        isProcessing = false;
        next = NULL;
    }
};

struct DockQueue
{
    DockingShip* front;
    DockingShip* rear;
    int count;
    int totalDockingSlots;     
    int currentlyDocked;      
    int avgServiceTimeMinutes; 

    DockQueue()
    {
        front = NULL;
        rear = NULL;
        count = 0;
        totalDockingSlots = 2;    
        currentlyDocked = 0;
        avgServiceTimeMinutes = 120; 
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    int getCount()
    {
        return count;
    }

    int getWaitingCount()
    {
        return count - currentlyDocked;
    }

    void enqueue(string shipName, string arrivalTime, string arrivalDate)
    {
        DockingShip* newShip = new DockingShip(shipName, arrivalTime, arrivalDate);

        int queuePosition = count - currentlyDocked;
        if (queuePosition < 0) 
        {
            queuePosition = 0;
        }

        if (totalDockingSlots > 0)
        {
            newShip->waitTimeMinutes = (queuePosition * avgServiceTimeMinutes) / totalDockingSlots;
        }

        if (rear == NULL)
        {
            front = newShip;
            rear = newShip;
        }
        else
        {
            rear->next = newShip;
            rear = newShip;
        }

        count++;

        if (currentlyDocked < totalDockingSlots)
        {
            currentlyDocked++;
            newShip->isProcessing = true;
            newShip->waitTimeMinutes = 0;
        }

        cout << "Ship '" << shipName << "' enqueued at dock.  ";
        if (newShip->isProcessing)
        {
            cout << "Now docking." << endl;
        }
        else
        {
            cout << "Wait time: " << newShip->waitTimeMinutes << " minutes.  " << "Position in queue: " << (count - currentlyDocked) << endl;
        }
    }

    DockingShip* dequeue()
    {
        if (front == NULL)
        {
            return NULL;
        }

        DockingShip* temp = front;
        front = front->next;

        if (front == NULL)
        {
            rear = NULL;
        }

        count--;

        if (temp->isProcessing)
        {
            currentlyDocked--;
        }

        if (currentlyDocked < totalDockingSlots && front != NULL)
        {
            DockingShip* nextShip = front;
            while (nextShip != NULL)
            {
                if (!nextShip->isProcessing)
                {
                    nextShip->isProcessing = true;
                    currentlyDocked++;
                    nextShip->waitTimeMinutes = 0;
                    cout << "Ship '" << nextShip->shipName << "' now docking." << endl;
                    break;
                }
                nextShip = nextShip->next;
            }
        }

        cout << "Ship '" << temp->shipName << "' departed from dock." << endl;

        return temp;
    }

    DockingShip* peek()
    {
        return front;
    }

    int getEstimatedWaitTime()
    {
        int waitingShips = count - currentlyDocked;
        if (waitingShips < 0) waitingShips = 0;

        if (totalDockingSlots > 0)
        {
            return (waitingShips * avgServiceTimeMinutes) / totalDockingSlots;
        }
        return waitingShips * avgServiceTimeMinutes;
    }

    void displayQueue()
    {
        cout << "Dock Slots: " << currentlyDocked << "/" << totalDockingSlots << " occupied" << endl;
        cout << "Ships in queue: " << count << endl;

        if (front == NULL)
        {
            cout << "Queue is empty." << endl;
            return;
        }

        cout << "Queue(front to rear) :" << endl;
        DockingShip* current = front;
        int pos = 1;
        while (current != NULL)
        {
            cout << "[" << pos << "] " << current->shipName
                << " | Arrived: " << current->arrivalTime;
            if (current->isProcessing)
            {
                cout << " | STATUS: DOCKING";
            }
            else
            {
                cout << " | WAITING (" << current->waitTimeMinutes << " min)";
            }
            cout << endl;
            current = current->next;
            pos++;
        }
    }

    void clear()
    {
        while (front != NULL)
        {
            DockingShip* temp = front;
            front = front->next;
            delete temp;
        }
        rear = NULL;
        count = 0;
        currentlyDocked = 0;
    }
};


int calculateVoyageTime(string depTime, string arrTime)
{
    int depMins = timeToMinutes(depTime);
    int arrMins = timeToMinutes(arrTime);

    int voyageTime = arrMins - depMins;

    if (voyageTime < 0)
    {
        voyageTime += 24 * 60;
    }

    return voyageTime;
}

struct Vertex
{
    string port;
    int portCharge;
    double latitude;
    double longitude;
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
        loadPortCoordinates();
        head = NULL;
        pathCount = 0;
    }

    void loadPortCoordinates()
    {
        string names[40] = 
        {
            "AbuDhabi","Alexandria","Antwerp","Athens","Busan","CapeTown","Chittagong","Colombo",
            "Copenhagen","Doha","Dubai","Dublin","Durban","Genoa","Hamburg","Helsinki","HongKong",
            "Istanbul","Jakarta","Jeddah","Karachi","Lisbon","London","LosAngeles","Manila",
            "Marseille","Melbourne","Montreal","Mumbai","NewYork","Osaka","Oslo","PortLouis",
            "Rotterdam","Shanghai","Singapore","Stockholm","Sydney","Tokyo","Vancouver"
        };

        double latitudes[40] =
        {
            24.4539,31.2001,51.2194,37.9838,35.1796,-33.9249,22.3569,6.9271,
            55.6761,25.2854,25.2048,53.3498,-29.8587,44.4056,53.5511,60.1699,22.3193,
            41.0082,-6.2088,21.4858,24.8607,38.7223,51.5074,34.0522,14.5995,
            43.2965,-37.8136,45.5017,19.0760,40.7128,34.6937,59.9139,-20.1609,
            51.9244,31.2304,1.3521,59.3293,-33.8688,35.6762,49.2827
        };

        double longitudes[40] =
        {
            54.3773,29.9187,4.4025,23.7275,129.0756,18.4241,91.7832,79.8612,
            12.5683,51.5310,55.2708,-6.2603,31.0218,8.9463,9.9937,24.9384,114.1694,
            28.9784,106.8456,39.1925,67.0011,-9.1393,-0.1278,-118.2437,120.9842,
            5.3698,144.9631,-73.5673,72.8777,-74.0060,135.5023,10.7522,57.5012,
            4.4777,121.4737,103.8198,18.0686,151.2093,139.6503,-123.1207
        };

        for (int i = 0; i < 40; i++)
        {
            ports[i].latitude = latitudes[i];
            ports[i].longitude = longitudes[i];
        }
    }

    DockQueue dockQueues[100];  

    void initializeDockQueues()
    {
        for (int i = 0; i < vertices; i++)
        {
            dockQueues[i].totalDockingSlots = 2;  
            dockQueues[i].avgServiceTimeMinutes = 120; 

            string portName = ports[i].port; 
            if (portName == "Singapore" || portName == "Shanghai" || portName == "Rotterdam" || portName == "Dubai")
            {
                dockQueues[i].totalDockingSlots = 4;
                dockQueues[i].avgServiceTimeMinutes = 90;
            }
            else if (portName == "HongKong" || portName == "Busan" || portName == "Hamburg" || portName == "LosAngeles")
            {
                dockQueues[i].totalDockingSlots = 3;
                dockQueues[i].avgServiceTimeMinutes = 100;
            }
        }
    }

    void enqueueAtPort(int portIndex, string shipName, string arrivalTime, string arrivalDate)
    {
        if (portIndex >= 0 && portIndex < vertices)
        {
            cout << "\nArriving at " << ports[portIndex].port << endl; 
            dockQueues[portIndex].enqueue(shipName, arrivalTime, arrivalDate);
        }
    }

    DockingShip* dequeueFromPort(int portIndex)
    {
        if (portIndex >= 0 && portIndex < vertices)
        {
            cout << "\nDeparting from " << ports[portIndex].port << endl; 
            return dockQueues[portIndex].dequeue();
        }
        return NULL;
    }

    void displayPortDockStatus(int portIndex)
    {
        if (portIndex >= 0 && portIndex < vertices)
        {
            cout << "\nDock Status: " << ports[portIndex].port << endl;  
            dockQueues[portIndex].displayQueue();
        }
    }

    void displayAllDockStatus()
    {
        cout << "\nALL PORT DOCK STATUS" << endl;
        for (int i = 0; i < vertices; i++)
        {
            if (dockQueues[i].count > 0)
            {
                cout << "\n[" << ports[i].port << "]" << endl;  
                dockQueues[i].displayQueue();
            }
        }
        cout << endl;
    }

    double haversineDistance(int source, int destination)
    {
        const double R = 6371.0;
        double lat1 = ports[source].latitude * 3.14159265 / 180.0;
        double lon1 = ports[source].longitude * 3.14159265 / 180.0;
        double lat2 = ports[destination].latitude * 3.14159265 / 180.0;
        double lon2 = ports[destination].longitude * 3.14159265 / 180.0;

        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;

        double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return R * c;
    }

    int heuristicCost(int source, int destination)
    {
        double dist = haversineDistance(source, destination);
        return (int)(dist * 0.5);
    }

    int heuristicTime(int source, int destination)
    {
        double dist = haversineDistance(source, destination);
        return (int)(dist / 50.0 * 60);
    }

    void readPortCharges(string fname)
    {
        ifstream fin(fname);
        if (!fin)
        {
            cout << "Sorry, there was an error in loading the port charges file." << endl;
            return;
        }
        string portName;
        int charge;
        int index = 0;

        while (fin >> portName >> charge && index < vertices)
        {
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
            cout << "Sorry.  There was an error while loading the file" << endl;
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
        for (int i = 0; i < (int)time.length(); i++)
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

        for (int i = colonPosition + 1; i < (int)time.length(); i++)
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

        while (position < (int)date.length() && date[position] != '/')
        {
            day = day * 10 + (date[position] - '0');
            position++;
        }
        position++;

        while (position < (int)date.length() && date[position] != '/')
        {
            month = month * 10 + (date[position] - '0');
            position++;
        }
        position++;

        while (position < (int)date.length())
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

        while (position < (int)currentDate.length() && currentDate[position] != '/')
        {
            day = day * 10 + (currentDate[position] - '0');
            position++;
        }
        position++;

        while (position < (int)currentDate.length() && currentDate[position] != '/')
        {
            month = month * 10 + (currentDate[position] - '0');
            position++;
        }
        position++;

        while (position < (int)currentDate.length())
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

    //time validation between two routes
    bool isTimeValid(Route* prevRoute, Route* nextRoute)
    {
        int prevDepMinutes = timeToMinutes(prevRoute->depTime);
        int arrivalMinutes = timeToMinutes(prevRoute->arrTime);
        int departureMinutes = timeToMinutes(nextRoute->depTime);
        
        if (prevDepMinutes == -1 || arrivalMinutes == -1 || departureMinutes == -1)
            return false;

        int prevDate = dateConversion(prevRoute->date);
        int nextDate = dateConversion(nextRoute->date);

        //Calculate actual arrival date 
        int actualArrivalDate = prevDate;
        if (arrivalMinutes < prevDepMinutes) 
        {
            // Overnight voyage 
            string nextDay = getNextDate(prevRoute->date);
            actualArrivalDate = dateConversion(nextDay);
        }

        const int minLayover = 15;       // 15 minutes minimum
        const int maxLayoverDays = 15;    // 7 days maximum

        // Case 1: Same day arrival and departure
        if (nextDate == actualArrivalDate) 
        {
            return (departureMinutes >= arrivalMinutes + minLayover);
        }

        // Case 2: Departure is after arrival 
        if (nextDate > actualArrivalDate) {
            int daysDifference = nextDate - actualArrivalDate;
            return (daysDifference <= maxLayoverDays);
        }

        // Case 3: Departure before arrival 
        return false;
    }

    bool isTimeValidForOptimization(Route* prevRoute, Route* nextRoute)
    {
        int prevDepMinutes = timeToMinutes(prevRoute->depTime);
        int arrivalMinutes = timeToMinutes(prevRoute->arrTime);
        int departureMinutes = timeToMinutes(nextRoute->depTime);

        if (prevDepMinutes == -1 || arrivalMinutes == -1 || departureMinutes == -1)
        {
            return false;
        }

        int prevDate = dateConversion(prevRoute->date);
        int nextDate = dateConversion(nextRoute->date);

        int actualArrivalDate = prevDate;
        if (arrivalMinutes < prevDepMinutes)
        {
            string nextDay = getNextDate(prevRoute->date);
            actualArrivalDate = dateConversion(nextDay);
        }

        const int minLayover = 15;

        if (nextDate == actualArrivalDate)
        {
            return (departureMinutes >= arrivalMinutes + minLayover);
        }

        if (nextDate > actualArrivalDate)
        {
            return true;
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

    bool iterativeDeepeningSearch(string currPort, string destination, string searchDate, int depthLimit, Route* currentPath[], string visitedPorts[], int currentDepth, bool isFirstLeg)
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
                cout << "  Cost: " << r->vCost << "$" << endl;
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

    void dijkstraMinCost(string source, string destination)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(source);
        int destIndex = getPortIndex(destination);

        if (srcIndex == -1 || destIndex == -1)
        {
            cout << "Invalid source or destination port" << endl;
            return;
        }

        algorithmVisualization.clear();

        int costs[40];
        bool visited[40];
        int predecessors[40];
        Route* bestRoute[40];

        for (int i = 0; i < numPorts; i++)
        {
            costs[i] = 1e9;
            visited[i] = false;
            predecessors[i] = -1;
            bestRoute[i] = NULL;
        }

        costs[srcIndex] = 0;
        PriorityQueue pq;
        pq.enqueue(srcIndex, 0);
        bestRoute[srcIndex] = NULL;

        while (!pq.isEmpty())
        {
            HeapNode currentNode = pq.dequeue();
            int currentNodeIndex = currentNode.index;

            if (visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;
                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
            }

            Route* temp = edges[currentNodeIndex];

            while (temp != NULL)
            {
                int neighbour = getPortIndex(temp->destination);
                if (neighbour == -1)
                {
                    temp = temp->next;
                    continue;
                }
                int sfmlFrom = findPortIndexByName(ports[currentNodeIndex].port);
                int sfmlTo = findPortIndexByName(temp->destination);
                if (sfmlFrom != -1 && sfmlTo != -1)
                {
                    algorithmVisualization.addExploredEdge(sfmlFrom, sfmlTo, false);
                }

                int cost = temp->vCost;
                int portCharge = ports[neighbour].portCharge;
                cost = cost + portCharge;

                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL)
                {
                    canTravel = true;
                }
                else
                {
                    canTravel = isTimeValidForOptimization(bestRoute[currentNodeIndex], temp);

                }

                if (canTravel && costs[currentNodeIndex] + cost < costs[neighbour])
                {
                    costs[neighbour] = costs[currentNodeIndex] + cost;
                    predecessors[neighbour] = currentNodeIndex;
                    bestRoute[neighbour] = temp;
                    pq.enqueue(neighbour, costs[neighbour]);
                }
                temp = temp->next;
            }
        }

        if (costs[destIndex] == 1e9)
        {
            cout << "No path exists from " << source << " to " << destination << endl;
            return;
        }

        cout << "Minimum cost from " << source << " to " << destination << " is: " << costs[destIndex] << "$" << endl;

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = predecessors[v])
        {
            path[count++] = v;
        }

        int sfmlPath[50];
        int sfmlPathCount = 0;
        for (int i = count - 1; i >= 0; i--)
        {
            int sfmlIdx = findPortIndexByName(ports[path[i]].port);
            if (sfmlIdx != -1)
            {
                sfmlPath[sfmlPathCount++] = sfmlIdx;
            }
        }
        algorithmVisualization.setFinalPath(sfmlPath, sfmlPathCount);

        for (int i = 0; i < sfmlPathCount - 1; i++)
        {
            for (int j = 0; j < algorithmVisualization.exploredCount; j++)
            {
                if (algorithmVisualization.exploredEdges[j].fromPortIndex == sfmlPath[i] && algorithmVisualization.exploredEdges[j].toPortIndex == sfmlPath[i + 1])
                {
                    algorithmVisualization.exploredEdges[j].isOptimal = true;
                }
            }
        }
        algorithmVisualization.startAnimation();

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl << endl;

        cout << "\nDOCKING SIMULATION" << endl;

        string shipName = "Ship_" + source + "_" + destination;

        for (int i = count - 1; i >= 0; i--)
        {
            int currentPortIdx = path[i];
            string portName = ports[currentPortIdx].port;

            string arrivalTime = "08:00";
            string arrivalDate = "01/01/2025";

            if (i < count - 1)
            {
                Route* routeUsed = bestRoute[currentPortIdx];
                if (routeUsed != NULL)
                {
                    arrivalTime = routeUsed->arrTime;
                    arrivalDate = routeUsed->date;
                }
            }

            cout << "\nArriving at " << portName << endl;
            dockQueues[currentPortIdx].enqueue(shipName, arrivalTime, arrivalDate);

            cout << "Dock Status at " << portName << ":" << endl;
            dockQueues[currentPortIdx].displayQueue();

            int waitTime = dockQueues[currentPortIdx].getEstimatedWaitTime();
            if (waitTime > 0)
            {
                cout << "Estimated wait time: " << waitTime << " minutes" << endl;
            }

            if (i > 0)
            {
                cout << "\nDeparting from " << portName << endl;
                DockingShip* departedShip = dockQueues[currentPortIdx].dequeue();
                if (departedShip != NULL)
                {
                    delete departedShip;
                }
            }
            else
            {
                cout << "\nFinal destination reached: " << portName << " ---" << endl;
                cout << "Ship remains docked." << endl;
            }
        }

        cout << endl;

        cout << "\nDetailed Route Information" << endl;
        for (int i = count - 1; i > 0; i--)
        {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = edges[from];
            while (temp != NULL)
            {
                if (temp->destination == ports[to].port)
                {
                    cout << "\nLeg " << (count - i) << ": " << ports[from].port << " -> " << ports[to].port << endl;
                    cout << "Date: " << temp->date << endl;
                    cout << "Departure Time: " << temp->depTime << endl;
                    cout << "Arrival Time: " << temp->arrTime << endl;
                    cout << "Cost: " << temp->vCost << "$" << endl;
                    cout << "Company: " << temp->company << endl;

                    if (i > 1)
                    {
                        Route* nextR = edges[to];
                        while (nextR != NULL)
                        {
                            if (nextR->destination == ports[path[i - 2]].port)
                            {
                                int arrMins = timeToMinutes(temp->arrTime);
                                int depMins = timeToMinutes(nextR->depTime);
                                int diff = depMins - arrMins;
                                bool nextDay = false;
                                if (diff < 0)
                                {
                                    diff = diff + 24 * 60;
                                    nextDay = true;
                                }
                                int hrs = diff / 60;
                                int mins = diff % 60;
                                cout << "Layover at " << ports[to].port << ": " << hrs << "hrs " << mins << "mins";
                                if (nextDay)
                                {
                                    cout << " (departure on next day)";
                                }
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

        algorithmLegCount = 0;
        algorithmName = "Dijkstra";
        algorithmMetric = "Cost";
        algorithmTotalValue = costs[destIndex];

        for (int i = count - 1; i > 0 && algorithmLegCount < totalAlgorithmLegs; --i)
        {
            int fromGraphIdx = path[i];
            int toGraphIdx = path[i - 1];

            int sfmlFromIdx = findPortIndexByName(ports[fromGraphIdx].port);
            int sfmlToIdx = findPortIndexByName(ports[toGraphIdx].port);

            if (sfmlFromIdx != -1 && sfmlToIdx != -1)
            {
                algorithmPart[algorithmLegCount].fromPortIndex = sfmlFromIdx;
                algorithmPart[algorithmLegCount].toPortIndex = sfmlToIdx;

                if (bestRoute[toGraphIdx] != NULL)
                {
                    Route* r = bestRoute[toGraphIdx];
                    algorithmPart[algorithmLegCount].date = r->date;
                    algorithmPart[algorithmLegCount].depTime = r->depTime;
                    algorithmPart[algorithmLegCount].arrTime = r->arrTime;
                    algorithmPart[algorithmLegCount].cost = r->vCost;
                    algorithmPart[algorithmLegCount].company = r->company;
                }
                algorithmLegCount++;
            }
        }

        for (int i = count - 1; i >= 0; i--)
        {
            int graphPortIdx = path[i];
            int sfmlPortIdx = findPortIndexByName(ports[graphPortIdx].port);

            if (sfmlPortIdx != -1)
            {
                portDockStatus[sfmlPortIdx].shipsInQueue = dockQueues[graphPortIdx].getCount();
                portDockStatus[sfmlPortIdx].shipsDocking = dockQueues[graphPortIdx].currentlyDocked;
                portDockStatus[sfmlPortIdx].maxSlots = dockQueues[graphPortIdx].totalDockingSlots;
            }
        }
    }

    void dijkstraMinTime(string source, string destination)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(source);
        int destIndex = getPortIndex(destination);

        if (srcIndex == -1 || destIndex == -1)
        {
            cout << "Invalid source or destination port" << endl;
            return;
        }

        algorithmVisualization.clear();

        int times[40];
        bool visited[40];
        int predecessors[40];
        Route* bestRoute[40];

        for (int i = 0; i < numPorts; i++)
        {
            times[i] = 1e9;
            visited[i] = false;
            predecessors[i] = -1;
            bestRoute[i] = NULL;
        }

        times[srcIndex] = 0;
        PriorityQueue pq;
        pq.enqueue(srcIndex, 0);
        bestRoute[srcIndex] = NULL;

        while (!pq.isEmpty())
        {
            HeapNode currentNode = pq.dequeue();
            int currentNodeIndex = currentNode.index;

            if (visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;

                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
            }

            Route* temp = edges[currentNodeIndex];
            while (temp != NULL)
            {
                int neighbour = getPortIndex(temp->destination);
                if (neighbour == -1)
                {
                    temp = temp->next;
                    continue;
                }

                int sfmlFrom = findPortIndexByName(ports[currentNodeIndex].port);
                int sfmlTo = findPortIndexByName(temp->destination);
                if (sfmlFrom != -1 && sfmlTo != -1)
                {
                    algorithmVisualization.addExploredEdge(sfmlFrom, sfmlTo, false);
                }

                int travelTime = calculateVoyageTime(temp->depTime, temp->arrTime);

                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL)
                {
                    canTravel = true;
                }
                else
                {
                    canTravel = isTimeValidForOptimization(bestRoute[currentNodeIndex], temp);
                }

                if (canTravel && times[currentNodeIndex] + travelTime < times[neighbour])
                {
                    times[neighbour] = times[currentNodeIndex] + travelTime;
                    predecessors[neighbour] = currentNodeIndex;
                    bestRoute[neighbour] = temp;
                    pq.enqueue(neighbour, times[neighbour]);
                }
                temp = temp->next;
            }
        }

        if (times[destIndex] == 1e9)
        {
            cout << "No path exists from " << source << " to " << destination << endl;
            return;
        }

        cout << "Minimum time from " << source << " to " << destination << " is: " << times[destIndex] << " minutes" << endl;

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = predecessors[v])
        {
            path[count++] = v;
        }

        int sfmlPath[50];
        int sfmlPathCount = 0;
        for (int i = count - 1; i >= 0; i--)
        {
            int sfmlIdx = findPortIndexByName(ports[path[i]].port);
            if (sfmlIdx != -1)
            {
                sfmlPath[sfmlPathCount++] = sfmlIdx;
            }
        }
        algorithmVisualization.setFinalPath(sfmlPath, sfmlPathCount);

        for (int i = 0; i < sfmlPathCount - 1; i++)
        {
            for (int j = 0; j < algorithmVisualization.exploredCount; j++)
            {
                if (algorithmVisualization.exploredEdges[j].fromPortIndex == sfmlPath[i] &&
                    algorithmVisualization.exploredEdges[j].toPortIndex == sfmlPath[i + 1])
                {
                    algorithmVisualization.exploredEdges[j].isOptimal = true;
                }
            }
        }

        algorithmVisualization.startAnimation();

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl << endl;

        cout << "\nDOCKING SIMULATION" << endl;

        string shipName = "Ship_" + source + "_" + destination;

        for (int i = count - 1; i >= 0; i--)
        {
            int currentPortIdx = path[i];
            string portName = ports[currentPortIdx].port;

            string arrivalTime = "08:00";
            string arrivalDate = "01/01/2025";

            if (i < count - 1)
            {
                Route* routeUsed = bestRoute[currentPortIdx];
                if (routeUsed != NULL)
                {
                    arrivalTime = routeUsed->arrTime;
                    arrivalDate = routeUsed->date;
                }
            }

            cout << "\nArriving at " << portName << endl;
            dockQueues[currentPortIdx].enqueue(shipName, arrivalTime, arrivalDate);

            cout << "Dock Status at " << portName << ":" << endl;
            dockQueues[currentPortIdx].displayQueue();

            int waitTime = dockQueues[currentPortIdx].getEstimatedWaitTime();
            if (waitTime > 0)
            {
                cout << "Estimated wait time: " << waitTime << " minutes" << endl;
            }

            if (i > 0)
            {
                cout << "\nDeparting from " << portName << endl;
                DockingShip* departedShip = dockQueues[currentPortIdx].dequeue();
                if (departedShip != NULL)
                {
                    delete departedShip;
                }
            }
            else
            {
                cout << "\nFinal destination reached: " << portName << endl;
                cout << "Ship remains docked." << endl;
            }
        }

        cout << endl;

        cout << "\nDetailed Route Information" << endl;
        for (int i = count - 1; i > 0; i--)
        {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = edges[from];
            while (temp != NULL)
            {
                if (temp->destination == ports[to].port)
                {
                    int voyageTime = calculateVoyageTime(temp->depTime, temp->arrTime);

                    cout << "\nLeg " << (count - i) << ": " << ports[from].port << " -> " << ports[to].port << endl;
                    cout << "Date: " << temp->date << endl;
                    cout << "Departure Time: " << temp->depTime << endl;
                    cout << "Arrival Time: " << temp->arrTime << endl;
                    cout << "Travel Time: " << voyageTime << " minutes" << endl;
                    cout << "Company: " << temp->company << endl;

                    if (i > 1)
                    {
                        Route* nextR = edges[to];
                        while (nextR != NULL)
                        {
                            if (nextR->destination == ports[path[i - 2]].port)
                            {
                                int arrMins = timeToMinutes(temp->arrTime);
                                int depMins = timeToMinutes(nextR->depTime);
                                int diff = depMins - arrMins;
                                bool nextDay = false;
                                if (diff < 0)
                                {
                                    diff = diff + 24 * 60;
                                    nextDay = true;
                                }
                                int hrs = diff / 60;
                                int mins = diff % 60;
                                cout << "Layover at " << ports[to].port << ": " << hrs << "hrs " << mins << "mins";
                                if (nextDay)
                                {
                                    cout << " (departure on next day)";
                                }
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

        algorithmLegCount = 0;
        algorithmName = "Dijkstra";
        algorithmMetric = "Time";
        algorithmTotalValue = times[destIndex];

        for (int i = count - 1; i > 0 && algorithmLegCount < totalAlgorithmLegs; --i)
        {
            int fromGraphIdx = path[i];
            int toGraphIdx = path[i - 1];

            int sfmlFromIdx = findPortIndexByName(ports[fromGraphIdx].port);
            int sfmlToIdx = findPortIndexByName(ports[toGraphIdx].port);

            if (sfmlFromIdx != -1 && sfmlToIdx != -1)
            {
                algorithmPart[algorithmLegCount].fromPortIndex = sfmlFromIdx;
                algorithmPart[algorithmLegCount].toPortIndex = sfmlToIdx;

                if (bestRoute[toGraphIdx] != NULL)
                {
                    Route* r = bestRoute[toGraphIdx];
                    algorithmPart[algorithmLegCount].date = r->date;
                    algorithmPart[algorithmLegCount].depTime = r->depTime;
                    algorithmPart[algorithmLegCount].arrTime = r->arrTime;
                    algorithmPart[algorithmLegCount].cost = r->vCost;
                    algorithmPart[algorithmLegCount].company = r->company;
                }
                algorithmLegCount++;
            }
        }

        for (int i = count - 1; i >= 0; i--)
        {
            int graphPortIdx = path[i];
            int sfmlPortIdx = findPortIndexByName(ports[graphPortIdx].port);

            if (sfmlPortIdx != -1)
            {
                portDockStatus[sfmlPortIdx].shipsInQueue = dockQueues[graphPortIdx].getCount();
                portDockStatus[sfmlPortIdx].shipsDocking = dockQueues[graphPortIdx].currentlyDocked;
                portDockStatus[sfmlPortIdx].maxSlots = dockQueues[graphPortIdx].totalDockingSlots;
            }
        }
    }

    void aStarMinCost(string source, string destination)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(source);
        int destIndex = getPortIndex(destination);

        if (srcIndex == -1 || destIndex == -1)
        {
            cout << "Invalid source or destination port" << endl;
            return;
        }
        algorithmVisualization.clear();

        int gCost[40];
        int fCost[40];
        bool visited[40];
        int predecessors[40];
        Route* bestRoute[40];

        for (int i = 0; i < numPorts; i++)
        {
            gCost[i] = 1e9;
            fCost[i] = 1e9;
            visited[i] = false;
            predecessors[i] = -1;
            bestRoute[i] = NULL;
        }

        gCost[srcIndex] = 0;
        fCost[srcIndex] = heuristicCost(srcIndex, destIndex);

        PriorityQueue pq;
        pq.enqueue(srcIndex, fCost[srcIndex]);
        bestRoute[srcIndex] = NULL;

        while (!pq.isEmpty())
        {
            HeapNode currentNode = pq.dequeue();
            int currentNodeIndex = currentNode.index;

            if (currentNodeIndex == destIndex)
            {
                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
                break;
            }

            if (visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;
                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
            }

            Route* temp = edges[currentNodeIndex];

            while (temp != NULL)
            {
                int neighbour = getPortIndex(temp->destination);
                if (neighbour == -1)
                {
                    temp = temp->next;
                    continue;
                }
                int sfmlFrom = findPortIndexByName(ports[currentNodeIndex].port);
                int sfmlTo = findPortIndexByName(temp->destination);
                if (sfmlFrom != -1 && sfmlTo != -1)
                {
                    algorithmVisualization.addExploredEdge(sfmlFrom, sfmlTo, false);
                }

                int cost = temp->vCost;
                int portCharge = ports[neighbour].portCharge;
                cost = cost + portCharge;

                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL)
                {
                    canTravel = true;
                }
                else
                {
                    canTravel = isTimeValidForOptimization(bestRoute[currentNodeIndex], temp);
                }

                int tentativeGCost = gCost[currentNodeIndex] + cost;

                if (canTravel && tentativeGCost < gCost[neighbour])
                {
                    gCost[neighbour] = tentativeGCost;
                    fCost[neighbour] = gCost[neighbour] + heuristicCost(neighbour, destIndex);
                    predecessors[neighbour] = currentNodeIndex;
                    bestRoute[neighbour] = temp;
                    pq.enqueue(neighbour, fCost[neighbour]);
                }
                temp = temp->next;
            }
        }

        if (gCost[destIndex] == 1e9)
        {
            cout << "No path exists from " << source << " to " << destination << endl;
            return;
        }

        cout << "A* Minimum cost from " << source << " to " << destination << " is: " << gCost[destIndex] << "$" << endl;

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = predecessors[v])
        {
            path[count++] = v;
        }
        int sfmlPath[50];
        int sfmlPathCount = 0;
        for (int i = count - 1; i >= 0; i--)
        {
            int sfmlIdx = findPortIndexByName(ports[path[i]].port);
            if (sfmlIdx != -1)
            {
                sfmlPath[sfmlPathCount++] = sfmlIdx;
            }
        }
        algorithmVisualization.setFinalPath(sfmlPath, sfmlPathCount);

        for (int i = 0; i < sfmlPathCount - 1; i++)
        {
            for (int j = 0; j < algorithmVisualization.exploredCount; j++)
            {
                if (algorithmVisualization.exploredEdges[j].fromPortIndex == sfmlPath[i] &&
                    algorithmVisualization.exploredEdges[j].toPortIndex == sfmlPath[i + 1])
                {
                    algorithmVisualization.exploredEdges[j].isOptimal = true;
                }
            }
        }

        algorithmVisualization.startAnimation();

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl << endl;

        cout << "\nDOCKING SIMULATION" << endl;

        string shipName = "Ship_" + source + "_" + destination;

        for (int i = count - 1; i >= 0; i--)
        {
            int currentPortIdx = path[i];
            string portName = ports[currentPortIdx].port;

            string arrivalTime = "08:00";
            string arrivalDate = "01/01/2025";

            if (i < count - 1)
            {
                Route* routeUsed = bestRoute[currentPortIdx];
                if (routeUsed != NULL)
                {
                    arrivalTime = routeUsed->arrTime;
                    arrivalDate = routeUsed->date;
                }
            }

            cout << "\nArriving at " << portName << endl;
            dockQueues[currentPortIdx].enqueue(shipName, arrivalTime, arrivalDate);

            cout << "Dock Status at " << portName << ":" << endl;
            dockQueues[currentPortIdx].displayQueue();

            int waitTime = dockQueues[currentPortIdx].getEstimatedWaitTime();
            if (waitTime > 0)
            {
                cout << "Estimated wait time: " << waitTime << " minutes" << endl;
            }

            if (i > 0)
            {
                cout << "\nDeparting from " << portName << endl;
                DockingShip* departedShip = dockQueues[currentPortIdx].dequeue();
                if (departedShip != NULL)
                {
                    delete departedShip;
                }
            }
            else
            {
                cout << "\nFinal destination reached: " << portName << endl;
                cout << "Ship remains docked." << endl;
            }
        }

        cout << endl;

        cout << "\nDetailed Route Information" << endl;
        for (int i = count - 1; i > 0; i--)
        {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = edges[from];
            while (temp != NULL)
            {
                if (temp->destination == ports[to].port)
                {
                    cout << "\nLeg " << (count - i) << ": " << ports[from].port << " -> " << ports[to].port << endl;
                    cout << "Date: " << temp->date << endl;
                    cout << "Departure Time: " << temp->depTime << endl;
                    cout << "Arrival Time: " << temp->arrTime << endl;
                    cout << "Cost: " << temp->vCost << "$" << endl;
                    cout << "Company: " << temp->company << endl;

                    if (i > 1)
                    {
                        Route* nextR = edges[to];
                        while (nextR != NULL)
                        {
                            if (nextR->destination == ports[path[i - 2]].port)
                            {
                                int arrMins = timeToMinutes(temp->arrTime);
                                int depMins = timeToMinutes(nextR->depTime);
                                int diff = depMins - arrMins;
                                bool nextDay = false;
                                if (diff < 0)
                                {
                                    diff = diff + 24 * 60;
                                    nextDay = true;
                                }
                                int hrs = diff / 60;
                                int mins = diff % 60;
                                cout << "Layover at " << ports[to].port << ": " << hrs << "hrs " << mins << "mins";
                                if (nextDay)
                                {
                                    cout << " (departure on next day)";
                                }
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

        algorithmLegCount = 0;
        algorithmName = "A*";
        algorithmMetric = "Cost";
        algorithmTotalValue = gCost[destIndex];

        for (int i = count - 1; i > 0 && algorithmLegCount < totalAlgorithmLegs; --i)
        {
            int fromGraphIdx = path[i];
            int toGraphIdx = path[i - 1];

            int sfmlFromIdx = findPortIndexByName(ports[fromGraphIdx].port);
            int sfmlToIdx = findPortIndexByName(ports[toGraphIdx].port);

            if (sfmlFromIdx != -1 && sfmlToIdx != -1)
            {
                algorithmPart[algorithmLegCount].fromPortIndex = sfmlFromIdx;
                algorithmPart[algorithmLegCount].toPortIndex = sfmlToIdx;

                if (bestRoute[toGraphIdx] != NULL)
                {
                    Route* r = bestRoute[toGraphIdx];
                    algorithmPart[algorithmLegCount].date = r->date;
                    algorithmPart[algorithmLegCount].depTime = r->depTime;
                    algorithmPart[algorithmLegCount].arrTime = r->arrTime;
                    algorithmPart[algorithmLegCount].cost = r->vCost;
                    algorithmPart[algorithmLegCount].company = r->company;
                }
                algorithmLegCount++;
            }
        }

        for (int i = count - 1; i >= 0; i--)
        {
            int graphPortIdx = path[i];
            int sfmlPortIdx = findPortIndexByName(ports[graphPortIdx].port);

            if (sfmlPortIdx != -1)
            {
                portDockStatus[sfmlPortIdx].shipsInQueue = dockQueues[graphPortIdx].getCount();
                portDockStatus[sfmlPortIdx].shipsDocking = dockQueues[graphPortIdx].currentlyDocked;
                portDockStatus[sfmlPortIdx].maxSlots = dockQueues[graphPortIdx].totalDockingSlots;
            }
        }
    }

    void aStarMinTime(string source, string destination)
    {
        int numPorts = vertices;
        int srcIndex = getPortIndex(source);
        int destIndex = getPortIndex(destination);

        if (srcIndex == -1 || destIndex == -1)
        {
            cout << "Invalid source or destination port" << endl;
            return;
        }

        algorithmVisualization.clear();

        int gTime[40];
        int fTime[40];
        bool visited[40];
        int predecessors[40];
        Route* bestRoute[40];

        for (int i = 0; i < numPorts; i++)
        {
            gTime[i] = 1e9;
            fTime[i] = 1e9;
            visited[i] = false;
            predecessors[i] = -1;
            bestRoute[i] = NULL;
        }

        gTime[srcIndex] = 0;
        fTime[srcIndex] = heuristicTime(srcIndex, destIndex);

        PriorityQueue pq;
        pq.enqueue(srcIndex, fTime[srcIndex]);
        bestRoute[srcIndex] = NULL;

        while (!pq.isEmpty())
        {
            HeapNode currentNode = pq.dequeue();
            int currentNodeIndex = currentNode.index;

            if (currentNodeIndex == destIndex)
            {
                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
                break;
            }

            if (visited[currentNodeIndex])
            {
                continue;
            }
            else
            {
                visited[currentNodeIndex] = true;

                int sfmlPortIdx = findPortIndexByName(ports[currentNodeIndex].port);
                if (sfmlPortIdx != -1)
                {
                    algorithmVisualization.addExplorationStep(sfmlPortIdx);
                }
            }

            Route* temp = edges[currentNodeIndex];

            while (temp != NULL)
            {
                int neighbour = getPortIndex(temp->destination);
                if (neighbour == -1)
                {
                    temp = temp->next;
                    continue;
                }

                int sfmlFrom = findPortIndexByName(ports[currentNodeIndex].port);
                int sfmlTo = findPortIndexByName(temp->destination);
                if (sfmlFrom != -1 && sfmlTo != -1)
                {
                    algorithmVisualization.addExploredEdge(sfmlFrom, sfmlTo, false);
                }

                int travelTime = calculateVoyageTime(temp->depTime, temp->arrTime);
                bool canTravel = false;

                if (bestRoute[currentNodeIndex] == NULL)
                {
                    canTravel = true;
                }
                else
                {
                    canTravel = isTimeValidForOptimization(bestRoute[currentNodeIndex], temp);
                }

                int tentativeGTime = gTime[currentNodeIndex] + travelTime;

                if (canTravel && tentativeGTime < gTime[neighbour])
                {
                    gTime[neighbour] = tentativeGTime;
                    fTime[neighbour] = gTime[neighbour] + heuristicTime(neighbour, destIndex);
                    predecessors[neighbour] = currentNodeIndex;
                    bestRoute[neighbour] = temp;
                    pq.enqueue(neighbour, fTime[neighbour]);
                }
                temp = temp->next;
            }
        }

        if (gTime[destIndex] == 1e9)
        {
            cout << "No path exists from " << source << " to " << destination << endl;
            return;
        }

        cout << "A* Minimum time from " << source << " to " << destination << " is: " << gTime[destIndex] << " minutes" << endl;

        int path[40];
        int count = 0;
        for (int v = destIndex; v != -1; v = predecessors[v])
        {
            path[count++] = v;
        }

        int sfmlPath[50];
        int sfmlPathCount = 0;
        for (int i = count - 1; i >= 0; i--)
        {
            int sfmlIdx = findPortIndexByName(ports[path[i]].port);
            if (sfmlIdx != -1)
            {
                sfmlPath[sfmlPathCount++] = sfmlIdx;
            }
        }
        algorithmVisualization.setFinalPath(sfmlPath, sfmlPathCount);

        for (int i = 0; i < sfmlPathCount - 1; i++)
        {
            for (int j = 0; j < algorithmVisualization.exploredCount; j++)
            {
                if (algorithmVisualization.exploredEdges[j].fromPortIndex == sfmlPath[i] &&
                    algorithmVisualization.exploredEdges[j].toPortIndex == sfmlPath[i + 1])
                {
                    algorithmVisualization.exploredEdges[j].isOptimal = true;
                }
            }
        }

        algorithmVisualization.startAnimation();

        cout << "Path: ";
        for (int i = count - 1; i >= 0; i--)
        {
            cout << ports[path[i]].port;
            if (i > 0) cout << " -> ";
        }
        cout << endl << endl;

        cout << "\nDOCKING SIMULATION" << endl;

        string shipName = "Ship_" + source + "_" + destination;

        for (int i = count - 1; i >= 0; i--)
        {
            int currentPortIdx = path[i];
            string portName = ports[currentPortIdx].port;

            string arrivalTime = "08:00";
            string arrivalDate = "01/01/2025";

            if (i < count - 1)
            {
                Route* routeUsed = bestRoute[currentPortIdx];
                if (routeUsed != NULL)
                {
                    arrivalTime = routeUsed->arrTime;
                    arrivalDate = routeUsed->date;
                }
            }

            cout << "\nArriving at " << portName << endl;
            dockQueues[currentPortIdx].enqueue(shipName, arrivalTime, arrivalDate);

            cout << "Dock Status at " << portName << ":" << endl;
            dockQueues[currentPortIdx].displayQueue();

            int waitTime = dockQueues[currentPortIdx].getEstimatedWaitTime();
            if (waitTime > 0)
            {
                cout << "Estimated wait time: " << waitTime << " minutes" << endl;
            }

            if (i > 0)
            {
                cout << "\nDeparting from " << portName << endl;
                DockingShip* departedShip = dockQueues[currentPortIdx].dequeue();
                if (departedShip != NULL)
                {
                    delete departedShip;
                }
            }
            else
            {
                cout << "\nFinal destination reached: " << portName << endl;
                cout << "Ship remains docked." << endl;
            }
        }

        cout << endl;

        cout << "\nDetailed Route Information" << endl;
        for (int i = count - 1; i > 0; i--)
        {
            int from = path[i];
            int to = path[i - 1];
            Route* temp = bestRoute[to];

            if (temp != NULL)
            {
                int voyageTime = calculateVoyageTime(temp->depTime, temp->arrTime);

                cout << "\nLeg " << (count - i) << ": " << ports[from].port << " -> " << temp->destination << endl;
                cout << "Date: " << temp->date << endl;
                cout << "Departure Time: " << temp->depTime << endl;
                cout << "Arrival Time: " << temp->arrTime << endl;
                cout << "Travel Time: " << voyageTime << " minutes" << endl;
                cout << "Company: " << temp->company << endl;

                if (i > 1)
                {
                    Route* nextRoute = bestRoute[path[i - 2]];

                    if (nextRoute != NULL)
                    {
                        bool validConnection = isTimeValid(temp, nextRoute);

                        if (validConnection)
                        {
                            int tempDep = timeToMinutes(temp->depTime);
                            int tempArr = timeToMinutes(temp->arrTime);
                            bool overnightArrival = (tempArr < tempDep);

                            int arrivalDateVal = dateConversion(temp->date);
                            if (overnightArrival)
                            {
                                arrivalDateVal = dateConversion(getNextDate(temp->date));
                            }

                            int nextDepDate = dateConversion(nextRoute->date);
                            int nextDepMins = timeToMinutes(nextRoute->depTime);

                            int layoverMinutes = 0;

                            if (nextDepDate == arrivalDateVal)
                            {
                                layoverMinutes = nextDepMins - tempArr;
                            }
                            else
                            {
                                int daysDiff = nextDepDate - arrivalDateVal;
                                int minutesUntilMidnight = (24 * 60) - tempArr;
                                layoverMinutes = minutesUntilMidnight + ((daysDiff - 1) * 24 * 60) + nextDepMins;
                            }

                            int hrs = layoverMinutes / 60;
                            int mins = layoverMinutes % 60;
                            cout << "Layover at " << temp->destination << ": " << hrs << "hrs " << mins << "mins";

                            if (nextDepDate > arrivalDateVal)
                            {
                                cout << " (departure on later day)";
                            }
                            cout << endl;
                        }
                        else
                        {
                            cout << "Layover at " << temp->destination << ": ERROR - Invalid connection detected!" << endl;
                        }
                    }
                }
            }
        }

        algorithmLegCount = 0;
        algorithmName = "A*";
        algorithmMetric = "Time";
        algorithmTotalValue = gTime[destIndex];

        for (int i = count - 1; i > 0 && algorithmLegCount < totalAlgorithmLegs; --i)
        {
            int fromGraphIdx = path[i];
            int toGraphIdx = path[i - 1];

            int sfmlFromIdx = findPortIndexByName(ports[fromGraphIdx].port);
            int sfmlToIdx = findPortIndexByName(ports[toGraphIdx].port);

            if (sfmlFromIdx != -1 && sfmlToIdx != -1)
            {
                algorithmPart[algorithmLegCount].fromPortIndex = sfmlFromIdx;
                algorithmPart[algorithmLegCount].toPortIndex = sfmlToIdx;

                if (bestRoute[toGraphIdx] != NULL)
                {
                    Route* r = bestRoute[toGraphIdx];
                    algorithmPart[algorithmLegCount].date = r->date;
                    algorithmPart[algorithmLegCount].depTime = r->depTime;
                    algorithmPart[algorithmLegCount].arrTime = r->arrTime;
                    algorithmPart[algorithmLegCount].cost = r->vCost;
                    algorithmPart[algorithmLegCount].company = r->company;
                }
                algorithmLegCount++;
            }
        }

        for (int i = count - 1; i >= 0; i--)
        {
            int graphPortIdx = path[i];
            int sfmlPortIdx = findPortIndexByName(ports[graphPortIdx].port);

            if (sfmlPortIdx != -1)
            {
                portDockStatus[sfmlPortIdx].shipsInQueue = dockQueues[graphPortIdx].getCount();
                portDockStatus[sfmlPortIdx].shipsDocking = dockQueues[graphPortIdx].currentlyDocked;
                portDockStatus[sfmlPortIdx].maxSlots = dockQueues[graphPortIdx].totalDockingSlots;
            }
        }
    }

    void routeOptimizationMenu(string source, string destination)
    {
        int choice;
        do
        {
            cout << "\nRoute Optimization Menu" << endl;
            cout << "1. Minimum Cost using Dijkstra" << endl;
            cout << "2. Minimum Time using Dijkstra" << endl;
            cout << "3. Minimum Cost using A*" << endl;
            cout << "4. Minimum Time using A*" << endl;
            cout << "5. Exit" << endl;
            cout << "Choose an option: ";
            cin >> choice;
            while (choice < 1 || choice > 5)
            {
                cout << "Enter the correct choice: ";
                cin >> choice;
            }

            switch (choice)
            {
            case 1:
                dijkstraMinCost(source, destination);
                break;
            case 2:
                dijkstraMinTime(source, destination);
                break;
            case 3:
                aStarMinCost(source, destination);
                break;
            case 4:
                aStarMinTime(source, destination);
                break;
            case 5:
                cout << "Exiting." << endl;
                return;
            }
        } while (choice != 5);
    }
};