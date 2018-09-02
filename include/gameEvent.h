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
    
    virtual string getStringPNJ(){};
    void setPosition(sf::Vector2i pos);
    
    virtual ~gameEvent();
    virtual bool testPos(int xi, int yi);
    virtual bool testHero(hero* h);
    virtual void activate(hero* h){};

};

class changeMap: public gameEvent{

    public:
    
    changeMap(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string n, int xN, int yN, int dN);
    ~changeMap();
    bool testHero(hero* h);
    void activate(hero* h);
};

class textInteraction: public gameEvent{
    
    protected:
    
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
    virtual string getStringPNJ(){};
    virtual bool testHero(hero* h);
    void activate(hero* h);
};

class staticPNJ: public textInteraction{

    private:
    
    character* PNJ;
    
    public:
    
    staticPNJ(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w, character* PNJi);
    ~staticPNJ();
    
    string getStringPNJ();
    bool testHero(hero* h);
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
