#include "../include/scenario.h"

using namespace std;

Arrow::Arrow()
{
    target = NULL;
    x = y = 0;
    direction = 0;
    length = 100;
    thickness = 4;
    sizeArrow = 3*thickness;
    line.setSize(sf::Vector2f(thickness,length));
    leftArrow.setSize(sf::Vector2f(sizeArrow,thickness));
    leftArrow.setRotation(135);
    rightArrow.setSize(sf::Vector2f(sizeArrow,thickness));
    rightArrow.setRotation(45);
    line.setFillColor(sf::Color::Black);
    leftArrow.setFillColor(sf::Color::Black);
    rightArrow.setFillColor(sf::Color::Black);
    setLength(length);
}

Arrow::Arrow(sf::RenderTarget* t)
{
    target = t;
    x = y = 0;
    direction = 0;
    length = 100;
    thickness = 4;
    sizeArrow = 3*thickness;
    line.setSize(sf::Vector2f(thickness,length));
    leftArrow.setSize(sf::Vector2f(sizeArrow,thickness));
    leftArrow.setRotation(135);
    rightArrow.setSize(sf::Vector2f(sizeArrow,thickness));
    rightArrow.setRotation(45);
    line.setFillColor(sf::Color::Black);
    leftArrow.setFillColor(sf::Color::Black);
    rightArrow.setFillColor(sf::Color::Black);
    setLength(length);
}

Arrow::Arrow(const Arrow& a)
{
    target = a.target;
    x = a.x;
    y = a.y;
    direction = a.direction;
    length = a.length;
    thickness = a.thickness;
    sizeArrow = a.sizeArrow;
    line = a.line;
    rightArrow = a.rightArrow;
}

Arrow::~Arrow(){};

void Arrow::setPosition(int xi, int yi)
{
    x = xi;
    y = yi;
    setLength(length);
}

void Arrow::setLength(int l)
{
    length = l;
    if (direction)
    {
        line.setPosition(x-thickness/2,y);
        leftArrow.setPosition(round(x+thickness/2+sizeArrow/sqrt(2)),round(y+length-sizeArrow/sqrt(2)));
        rightArrow.setPosition(round(x+(thickness-sizeArrow)/sqrt(2)-thickness/2),round(y+length-(thickness+sizeArrow)/sqrt(2)));
    }
    else
    {
        line.setPosition(x-thickness/2,y-length);
        leftArrow.setPosition(round(x+thickness/sqrt(2)-thickness/2),round(y-length+thickness/sqrt(2)));
        rightArrow.setPosition(round(x+thickness/2),round(y-length));
    }
}

void Arrow::setThickness(int t)
{
    thickness = t;
    setLength(length);
}

void Arrow::setDirection(bool dir)
{
    direction = dir;
    setLength(length);
}

void Arrow::setColor(sf::Color c)
{
    line.setFillColor(c);
    leftArrow.setFillColor(c);
    rightArrow.setFillColor(c);
}

void Arrow::draw()
{
    if (target != NULL)
    {
        target->draw(leftArrow);
        target->draw(rightArrow);
        target->draw(line);
    }
}

Arrow& Arrow::operator=(const Arrow& a)
{
    *this = Arrow(a);
    return *this;
}

EventRepresentation::EventRepresentation()
{
    x = y;
    lx = 150;
    ly = 50;
    nameEvent = "";
    lengthDownArrow = lengthUpArrow = 0;
    window = NULL;
}

EventRepresentation::EventRepresentation(sf::RenderTarget* w)
{
    window = w;
    x = y;
    lx = 150;
    ly = 50;
    nameEvent = "";
    lengthDownArrow = lengthUpArrow = 0;
}

EventRepresentation::EventRepresentation(const EventRepresentation& e)
{
    window = e.window;
    x = e.x;
    y = e.y;
    lx = e.lx;
    ly = e.ly;
    nameEvent = e.nameEvent;
    upArrow = e.upArrow;
    downArrow = e.downArrow;
    lengthDownArrow = e.lengthDownArrow;
    lengthUpArrow = e.lengthUpArrow;
}

EventRepresentation& EventRepresentation::operator=(const EventRepresentation& e)
{
    *this = EventRepresentation(e);
    return *this;
}
