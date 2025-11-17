#include <SFML/Graphics.hpp>
#include "SFMLrun.h"
#include <iostream>
#include "Graph.h"
using namespace std;

int main()
{
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


