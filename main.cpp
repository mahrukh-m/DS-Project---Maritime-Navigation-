#include <iostream>
#include "Graph.h"
using namespace std;

int main()
{
    Graph g;
    g.loadFile("Routes.txt");
    g.printGraph();
}
