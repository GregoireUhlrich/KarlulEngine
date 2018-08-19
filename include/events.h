#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <string>
using namespace std;

class mapi;

class event{

    protected:
    
    int x, y;
    mapi* M;
    
    public:
    
    event(int xi, int yi, mapi* Mi);
    virtual ~event();
    virtual void activate(){};

};

class changeMap: public event{

    private:
    
    string nameMap;
    int xNew, yNew, dirNew;
    
    public:
    
    changeMap(int xi, int yi, mapi* Mi, string n, int xN, int yN, int dN);
    ~changeMap();
    void activate();
};

#endif
