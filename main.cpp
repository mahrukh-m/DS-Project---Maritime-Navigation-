#include <SFML/Graphics.hpp>
#include "SFMLrun.h"
#include <iostream>
#include "Graph.h"
using namespace std;

int main()
{
    Graph g;
    g.loadFile("Routes.txt");

    int choice;
    do
    {
        cout << "\nSHIPPING ROUTE MANAGEMENT SYSTEM" << endl;
        cout << "1. Search Routes" << endl;
        cout << "2. Book a Route" << endl;
        cout << "3. View All Bookings" << endl;
        cout << "4. View Routes from a Port" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        while(choice < 1 || choice > 5)
        {
            cout << "Enter a valid choice: ";
            cin >> choice;
        }
		switch (choice)
		{
			case 1:
            {
                string source, destination, date;
                cout << "\nEnter origin port: ";
                getline(cin >> ws, source);
                cout << "Enter destination port: ";
                getline(cin >> ws, destination);
                cout << "Enter departure date (DD/MM/YYYY): ";
                getline(cin >> ws, date);

                g.searchRoutes(source, destination, date);
                break;
            }
			
			case 2:
            {
                g.bookRoute();
                break;
            }
			
			case 3:
            {
                g.viewBookings();
                break;
            }
			
			case 4:
            {
                string portName;
                cout << "\nEnter port name: ";
                getline(cin >> ws, portName);
                g.displayPortRoutes(portName);
                break;
            }
            
            case 5:
            {
            	cout << "Exiting" << endl;
            	return;
            }
        }
    } while (choice != 5);
}


