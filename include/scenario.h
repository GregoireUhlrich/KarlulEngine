#ifndef SCENARIO_H_INCLUDED
#define SCENARIO_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

class Arrow
{
    private:
    
    sf::RenderTarget* target;
    sf::RectangleShape line;
    sf::RectangleShape leftArrow;
    sf::RectangleShape rightArrow;
    
    int x,y;
    int length;
    bool direction; //0 up 1 down
    int thickness;
    int sizeArrow;
    
    public:
    
    Arrow();
    Arrow(sf::RenderTarget* t);
    Arrow (const Arrow& a);
    ~Arrow();
    
    void setPosition(int xi, int yi);
    void setLength(int l);
    void setThickness(int t);
    void setDirection(bool d);
    void setColor(sf::Color c);
    
    void draw();
    
    Arrow& operator=(const Arrow& a);
};

class EventRepresentation{

    private:
    
    sf::RenderTarget* window;
    Arrow upArrow;
    Arrow downArrow;
    std::string nameEvent;
    
    int x,y;
    int lx,ly;
    int lengthUpArrow, lengthDownArrow;
    
    sf::RenderTexture texture;
    sf::Font font;
    sf::Text text;
    
    public:
    
    EventRepresentation();
    EventRepresentation(sf::RenderTarget* w);
    EventRepresentation(const EventRepresentation& e);
    ~EventRepresentation(){};
    
    EventRepresentation& operator=(const EventRepresentation& e);
};

#endif
