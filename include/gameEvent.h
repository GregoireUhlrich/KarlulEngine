#ifndef GAMEEVENT_H_INCLUDED
#define GAMEEVENT_H_INCLUDED

#include <string>
using namespace std;

class mapi;

struct gate
{
    int x, y;
    string name;
};

class gameEvent{

    public:
    
    int x, y, dir;
    mapi* M;
    string type;
    int xNew, yNew, dirNew;
    string nameMap;
    
    gameEvent();
    gameEvent(int xi, int yi, int di, mapi* Mi);
    gameEvent(const gameEvent& g);
    virtual ~gameEvent();
    virtual bool testPos(int xi, int yi);
    virtual void testHero(character* h){};
    virtual void activate(character* h){};

};

class changeMap: public gameEvent{

    public:
    
    changeMap(int xi, int yi, int di, mapi* Mi, string n, int xN, int yN, int dN);
    ~changeMap();
    void testHero(character* h);
    void activate(character* h);
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

#endif
