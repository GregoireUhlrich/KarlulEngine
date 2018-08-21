#ifndef GAMEEVENT_H_INCLUDED
#define GAMEEVENT_H_INCLUDED

#include <string>
using namespace std;

class mapi;

class gameEvent{

    protected:
    
    int x, y;
    mapi* M;
    
    public:
    
    gameEvent(int xi, int yi, mapi* Mi);
    virtual ~gameEvent();
    virtual bool testPos(int xi, int yi);
    virtual void activate(){};

};

class changeMap: public gameEvent{

    private:
    
    string nameMap;
    int xNew, yNew, dirNew;
    
    public:
    
    changeMap(int xi, int yi, mapi* Mi, string n, int xN, int yN, int dN);
    ~changeMap();
    void activate();
};


struct varEvents
{
    int nEvents;
    string* nameEvent;
};

struct paramsEvents
{
    int xNewMap;
    int yNewMap;
    int dirNew;
    string newMap;
};

void initEvents(varEvents v);

gameEvent* createEvent(string definition, mapi*Mi, paramsEvents);


#endif
