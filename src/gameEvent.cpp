#include "../include/map.h"
#include "../include/gameEvent.h"
using namespace std;

void initEvents(varEvents v)
{
    v.nEvents = 1;
    v.nameEvent = new string[1];
    v.nameEvent[0] = "Change map";
}

gameEvent::gameEvent()
{
    x = y = 0;
    type = "None";
    M = NULL;
}

gameEvent::gameEvent(int xi, int yi, int di, mapi* Mi)
{
    type = "None";
    x = xi;
    y = yi;
    dir = di;
    M = Mi;
}

gameEvent::gameEvent(const gameEvent& g)
{
    type = "None";
    x = g.x;
    y = g.y;
    dir = g.dir;
    M = g.M;
}

gameEvent::~gameEvent(){}

bool gameEvent::testPos(int xi, int yi)
{
    return (x==xi && y==yi);
}

changeMap::changeMap(int xi, int yi, int di, mapi* Mi, string n, int xN, int yN, int dN): gameEvent(xi,yi,di,Mi)
{
    type = "changeMap";
    nameMap = n;
    xNew = xN;
    yNew = yN;
    dirNew = dN;
}

changeMap::~changeMap(){}

void changeMap::testHero(character* h)
{
    float epsilon = 0.1;
    int xSprites, ySprites;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    if (abs(h->getX()/xSprites-x) < epsilon && abs(h->getY()/ySprites-y) < epsilon && h->getWantedMove() == dir)
        activate(h);
}

void changeMap::activate(character* h)
{
    M->drawClearWindow();
    M->saveMap();
    sf::Vector2u fooVec = M->getSizeSprites();
    h->setX(xNew*fooVec.x);
    h->setY(yNew*fooVec.y);
    h->setDir(dirNew);
    h->releaseKey();
    M->setFile(nameMap);
    M->loadMap();
    M->setState(heros);
}
