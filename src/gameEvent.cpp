#include "../include/map.h"
#include "../include/gameEvent.h"
using namespace std;

void initEvents(varEvents v)
{
    v.nEvents = 1;
    v.nameEvent = new string[1];
    v.nameEvent[0] = "Change map";
}

gameEvent* createEvent(string definition, mapi*Mi, int xi, int yi, int optionInt1, int optionInt2, int optionInt3, string optionString1)
{
    gameEvent* newgameEvent;
    if (definition == "Change map")
        newgameEvent = new changeMap(xi,yi,Mi,optionString1,optionInt1,optionInt2,optionInt3);
    return newgameEvent;
}

gameEvent::gameEvent(int xi, int yi, mapi* Mi)
{
    x = xi;
    y = yi;
    M = Mi;
}

gameEvent::~gameEvent(){}

bool gameEvent::testPos(int xi, int yi)
{
    return (x==xi && y==yi);
}

changeMap::changeMap(int xi, int yi, mapi* Mi, string n, int xN, int yN, int dN): gameEvent(xi,yi,Mi)
{
    nameMap = n;
    xNew = xN;
    yNew = yN;
    dirNew = dN;
}

changeMap::~changeMap(){}

void changeMap::activate()
{
    M->saveMap();
    M->setFile(nameMap);
    M->loadMap();
}
