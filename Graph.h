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
    Route *next;

    Route(string dest, string d, string dTime, string aTime, int cost, string comp)
    {
        destination=dest;
        date=d;
        depTime=dTime;
        arrTime=aTime;
        vCost=cost;
        company=comp;
        next=NULL;
    }
};

struct Vertex{
    string port;
    int portCharge;
};

struct Graph{
    int vertices;
    Route **edges;
    string ports[40];
    Route *head;

    Graph()
    {
        vertices=40;
        edges= new Route*[vertices];
        for(int i=0;i<vertices;i++)
        {
            edges[i]=NULL;
        }

        string temp[40]={"AbuDhabi", "Alexandria", "Antwerp", "Athens", "Busan", "CapeTown", "Chittagong", "Colombo", "Copenhagen", "Doha", "Dubai", "Dublin", "Durban", "Genoa", "Hamburg", "Helsinki",
             "HongKong", "Istanbul", "Jakarta", "Jeddah", "Karachi", "Lisbon", "London", "LosAngeles", "Manila", "Marseille", "Melbourne", "Montreal", "Mumbai", "NewYork", "Osaka", "Oslo", "PortLouis", 
             "Rotterdam", "Shanghai", "Singapore", "Stockholm", "Sydney", "Tokyo", "Vancouver"};
        
        for(int i=0;i<40;i++)
        {
            ports[i]=temp[i];
        }
        head=NULL;
    
    }

    void addToGraph(Route *route,string origin, int index)
    {
        if (index < 0 || index >= vertices) 
        {
            return;
        }
            

        if(edges[index]==NULL)
        {
            edges[index]=route;
            return;
        }

        head=edges[index];
        Route *temp=head;

        while(temp->next!=NULL)
        {
            temp=temp->next;
        }

        temp->next=route;
    }

    int getPortIndex(string name)
    {
        for(int i=0;i<vertices;i++)
        {
            if(name==ports[i])
            {
                return i;
            }
        }
        return -1;
        
    }

    void loadFile(string fname)
    {
        ifstream fin(fname);

        if(!fin)
        {
            return;
        }

        string origin, destination, date, depTime, arrTime, company;
        int vCost;
        while(fin>>origin>>destination>> date>> depTime>>arrTime>> vCost>> company)
        {
            Route *route = new Route(destination,date, depTime, arrTime, vCost, company);
            int index=getPortIndex(origin);
            addToGraph(route,origin,index);
        }
        cout<<"Successfully loaded file"<<endl;

        fin.close();
    }

    void printGraph()
    {
        for(int i=0;i<vertices;i++)
        {
            Route *temp=edges[i];
            cout<<ports[i]<<": ";
            while(temp!=NULL)
            {
                cout<<" --> "<<temp->destination;
                temp=temp->next;
            }
            cout<<endl;
        }

    }

};

