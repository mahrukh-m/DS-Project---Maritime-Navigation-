#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

struct Booking
{
    string customerName;
    string cargoType;
    int cargoWeight;
    string source;
    string destination;
    string date;
    int routeChoice;
    int totalCost;
    string bookingID;
};

struct Graph
{
    int vertices;
    Route** edges;
    string ports[40];
    Route* head;

    static const int totalRoutes = 100;
    static const int totalDepth = 10;
    static const int maxBookings = 50;

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

    Booking bookings[maxBookings];
    int bookingCount;

    Graph()
    {
        vertices = 40;
        edges = new Route * [vertices];
        for (int i = 0; i < vertices; i++)
        {
            edges[i] = NULL;
        }
        string temp[40] = { "AbuDhabi", "Alexandria", "Antwerp", "Athens", "Busan",
                          "CapeTown", "Chittagong", "Colombo", "Copenhagen", "Doha",
                          "Dubai", "Dublin", "Durban", "Genoa", "Hamburg", "Helsinki",
                          "HongKong", "Istanbul", "Jakarta", "Jeddah", "Karachi",
                          "Lisbon", "London", "LosAngeles", "Manila", "Marseille",
                          "Melbourne", "Montreal", "Mumbai", "NewYork", "Osaka", "Oslo",
                          "PortLouis", "Rotterdam", "Shanghai", "Singapore", "Stockholm",
                          "Sydney", "Tokyo", "Vancouver" };
        for (int i = 0; i < 40; i++)
        {
            ports[i] = temp[i];
        }
        head = NULL;
        pathCount = 0;
        bookingCount = 0;
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
            if (name == ports[i])
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
        cout << "Successfully loaded " << count << " routes from file" << endl << endl;
        fin.close();
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

        if (nextDate == expectedNextDateInt)
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
            cout << "\nSorry, no routes have been found for the given criteria." << endl;
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
                    cout << "Direct Routes: " << endl;
                }
                else if (stops == 1)
                {
                    cout << "Routes with 1 stop: " << endl;
                }
                else
                {
                    cout << "Routes with " << stops << " stops: " << endl;
                }
                cout << endl;
            }

            if (stops == 0)
            {
                cout << "Route no. " << (i + 1) << ": " << source << " --> " << destination << endl;
                Route* r = foundPaths[i].routes[0];
                cout << "Date: " << r->date << endl;
                cout << "Departure: " << r->depTime << endl;
                cout << "Arrival: " << r->arrTime << endl;
                cout << "Cost: " << r->vCost << "$" << endl;
                cout << "Company: " << r->company << endl;
            }
            else
            {
                cout << "Route no. " << (i + 1) << ":" << endl;
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
                    cout << "Leg " << (j + 1) << ": " << currentPort << " to " << r->destination << endl;
                    cout << "Date: " << r->date << endl;
                    cout << "Departure: " << r->depTime << endl;
                    cout << "Arrival: " << r->arrTime << endl;
                    cout << "Cost: " << r->vCost << "$" << endl;
                    cout << "Company: " << r->company << endl;

                    if (j < foundPaths[i].length - 1)
                    {
                        Route* nextRoute = foundPaths[i].routes[j + 1];
                        int arrivalMinutes = timeToMinutes(r->arrTime);
                        int departureMinutes = timeToMinutes(nextRoute->depTime);

                        int arrivalDateInt = dateConversion(r->date);
                        int departureDateInt = dateConversion(nextRoute->date);
                        int layoverTime = 0;

                        if (arrivalDateInt == departureDateInt)
                        {
                            layoverTime = departureMinutes - arrivalMinutes;
                        }
                        else
                        {
                            int minutesUntilMidnight = (24 * 60) - arrivalMinutes;
                            layoverTime = minutesUntilMidnight + departureMinutes;
                        }

                        int layoverHours = layoverTime / 60;
                        int layoverMinutes = layoverTime % 60;
                        cout << "Layover: " << layoverHours << "h " << layoverMinutes << "m";

                        if (arrivalDateInt != departureDateInt)
                        {
                            cout << "(next day)";
                        }
                        cout << endl;
                    }
                    cout << endl;

                    currentPort = r->destination;
                }

                cout << "  Total Cost: $" << foundPaths[i].totalCost << endl;
            }
            cout << endl;
        }

        cout << "Total routes found: " << pathCount << endl;
    }

    string generateBookingID()
    {
        string id = "BK";
        int num = bookingCount + 10001;
        id = id + to_string(num);
        return id;
    }

    void bookRoute()
    {
        if (bookingCount >= maxBookings)
        {
            cout << "\nBooking system is full. Cannot accept more bookings." << endl;
            return;
        }

        string customerName, cargoType, source, destination, date;
        int cargoWeight;

        cout << "Enter your name: ";
        getline(cin >> ws, customerName);

        cout << "Enter cargo type: ";
        getline(cin >> ws, cargoType);

        cout << "Enter cargo weight (kg): ";
        cin >> cargoWeight;

        cout << "\nEnter origin port: ";
        getline(cin >> ws, source);

        cout << "Enter destination port: ";
        getline(cin >> ws, destination);

        cout << "Enter departure date (DD/MM/YYYY): ";
        getline(cin >> ws, date);

        searchRoutes(source, destination, date);

        if (pathCount == 0)
        {
            cout << "\nNo routes available. Booking cannot be completed." << endl;
            return;
        }

        cout << "\nSelect a route to book (1-" << pathCount << "): ";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > pathCount)
        {
            cout << "Invalid choice. Booking cancelled." << endl;
            return;
        }

        bookings[bookingCount].customerName = customerName;
        bookings[bookingCount].cargoType = cargoType;
        bookings[bookingCount].cargoWeight = cargoWeight;
        bookings[bookingCount].source = source;
        bookings[bookingCount].destination = destination;
        bookings[bookingCount].date = date;
        bookings[bookingCount].routeChoice = choice;
        bookings[bookingCount].totalCost = foundPaths[choice - 1].totalCost;
        bookings[bookingCount].bookingID = generateBookingID();

		cout << "Booking has been confirmed" << endl;
        cout << "Booking ID: " << bookings[bookingCount].bookingID << endl;
        cout << "Customer: " << customerName << endl;
        cout << "Route: " << source << " to " << destination << endl;
        cout << "Date: " << date << endl;
        cout << "Cargo: " << cargoType << " (" << cargoWeight << " kg)" << endl;
        cout << "Total Cost: " << bookings[bookingCount].totalCost << "$" << endl;

        bookingCount++;
    }

    void printGraph()
    {
        for (int i = 0; i < vertices; i++)
        {
            Route* temp = edges[i];
            cout << ports[i] << ": ";
            while (temp != NULL)
            {
                cout << " --> " << temp->destination;
                temp = temp->next;
            }
            cout << endl;
        }
    }

    void viewBookings()
    {
        if (bookingCount == 0)
        {
            cout << "\nNo bookings found." << endl;
            return;
        }
        cout << endl;

        for (int i = 0; i < bookingCount; i++)
        {
            cout << "Booking " << (i + 1) << endl;
            cout << "ID: " << bookings[i].bookingID << endl;
            cout << "Customer: " << bookings[i].customerName << endl;
            cout << "Route: " << bookings[i].source << " to " << bookings[i].destination << endl;
            cout << "Date: " << bookings[i].date << endl;
            cout << "Cargo: " << bookings[i].cargoType << " (" << bookings[i].cargoWeight << " kg)" << endl;
            cout << "Cost: $" << bookings[i].totalCost << endl;
            cout << endl;
        }

        cout << "Total bookings: " << bookingCount << endl;
    }
};