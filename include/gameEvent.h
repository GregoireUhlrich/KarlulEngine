#ifndef GAMEEVENT_H_INCLUDED
#define GAMEEVENT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "triggerEvent.h"
using namespace std;

class mapi;

class gameEvent{

    public:
    
    int x, y, dir;
    mapi* M;
    sf::RenderWindow* window;
    string type;
    int xNew, yNew, dirNew;
    string nameMap;
    string stringFile;
    
    string trigger;
    
    gameEvent();
    gameEvent(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string t);
    gameEvent(const gameEvent& g);
    
    virtual string getStringPNJ(){};
    string getTrigger() const;
    void setPosition(sf::Vector2i pos);
    
    virtual ~gameEvent();
    virtual bool testHero(hero* h);
    virtual void activate(hero* h){};
    bool triggerEvent(hero* h);

};

class changeMap: public gameEvent{

    public:
    
    changeMap(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string t, string n, int xN, int yN, int dN);
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
    
    textInteraction(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w, string t);
    ~textInteraction();
    virtual string getStringPNJ(){};
    virtual bool testHero(hero* h);
    void activate(hero* h);
};

class staticPNJ: public textInteraction{

    private:
    
    character* PNJ;
    
    public:
    
    staticPNJ(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w, string t, character* PNJi);
    ~staticPNJ();
    
    string getStringPNJ();
    bool testHero(hero* h);
};

gameEvent* createEvents(ifstream& f, mapi* M);

#endif
