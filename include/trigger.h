#ifndef TRIGGER_H_INCLUDED
#define TRIGGER_H_INCLUDED

#include "map.h"
#include "character.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>

class Trigger{

    protected:
    
    mapi* M;
    hero* h;
    sf::RenderWindow* window;
    
    bool activated;
    int xSprites, ySprites;
    float elapsedTime;
    float triggerTime;
    
    public:
    
    Trigger();
    Trigger(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Trigger(const Trigger& t);
    virtual ~Trigger(){};
    
    virtual bool test(double eT){return 0;};
};

class Cross: public Trigger{
    
    protected:
    
    int x, y;
    
    public:
    
    Cross(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Cross(const Cross& c);
    ~Cross(){};
    
    bool test(double eT);
};

class Action: public Trigger{
    
    protected:
    
    int x, y;
    
    public:
    
    Action(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Action(const Action& c);
    ~Action(){};
    
    bool test(double eT);
};

#endif
