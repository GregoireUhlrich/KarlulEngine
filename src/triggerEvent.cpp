#include "../include/triggerEvent.h"

double epsilon = 0.1;

bool cross(hero* h, int x, int y)
{
    sf::Vector2u sizeSprite = h->getSizeSprite();
    return (abs(h->getX()/sizeSprite.x-x) < epsilon && abs(h->getY()/sizeSprite.y-y) < epsilon);
}

bool action(hero* h, int x, int y)
{
    sf::Vector2u sizeSprite = h->getSizeSprite();
    return (abs(h->getX()/sizeSprite.x-x) < epsilon && abs(h->getY()/sizeSprite.y-y) < epsilon && h->pullAction());
}

bool boundaryMap(hero* h, int x, int y, int dir)
{
    sf::Vector2u sizeSprite = h->getSizeSprite();
    return (h->getX()/sizeSprite.x == x && h->getY()/sizeSprite.y == y && h->getWantedMove() == dir);
}

bool gate(hero* h, int x, int y, int dir)
{
    sf::Vector2u sizeSprite = h->getSizeSprite();
    return (abs(h->getX()/sizeSprite.x-x) < epsilon && abs(h->getY()/sizeSprite.y-y) < epsilon && h->getDir() == dir);
}

bool interactionStaticPNJ(hero* h, int x, int y)
{
    int xSprites, ySprites;
    sf::Vector2u foo = h->getSizeSprite();
    xSprites = foo.x;
    ySprites = foo.y;
    if (h->getDir() == 0 && h->getX()/xSprites == x && h->getY()/ySprites == y-1 && h->pullAction()) return 1;
    else if (h->getDir() == 1 && h->getX()/xSprites == x+1 && h->getY()/ySprites == y && h->pullAction()) return 1;
    else if (h->getDir() == 2 && h->getX()/xSprites == x-1 && h->getY()/ySprites == y && h->pullAction()) return 1;
    else if (h->getDir() == 3 && h->getX()/xSprites == x && h->getY()/ySprites == y+1 && h->pullAction()) return 1;
    else return 0;
}
