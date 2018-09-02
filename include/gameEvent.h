#ifndef GAMEEVENT_H_INCLUDED
#define GAMEEVENT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
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
    sf::RenderWindow* window;
    string type;
    int xNew, yNew, dirNew;
    string nameMap;
    string stringFile;
    
    gameEvent();
    gameEvent(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w);
    gameEvent(const gameEvent& g);
    virtual ~gameEvent();
    virtual bool testPos(int xi, int yi);
    virtual void testHero(character* h){};
    virtual void activate(character* h){};

};

class changeMap: public gameEvent{

    public:
    
    changeMap(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string n, int xN, int yN, int dN);
    ~changeMap();
    void testHero(character* h);
    void activate(character* h);
};

class textInteraction: public gameEvent{
    
    private:
    
    bool activated;
    unsigned int characterSize;
    int thickness;
    int iText;
    float margin;
    float interline;
    vector<sf::String> text;
    vector<sf::Text> textToDraw;
    sf::Font font;
    
    public:
    
    textInteraction(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w);
    ~textInteraction();
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
