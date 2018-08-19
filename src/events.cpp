#include "../include/events.h"
#include "../include/map.h"

event::event(int xi, int yi, mapi* Mi)
{
    x = xi;
    y = yi;
    M = Mi;
}

event::~event(){}

changeMap::changeMap(int xi, int yi, mapi* Mi, string n, int xN, int yN, int dN): event(xi,yi,Mi)
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
