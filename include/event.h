#ifndef EVENT_H_INCLUDED 
#define EVENT_H_INCLUDED

#include "map.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Event{

    protected:
    
    mapi* M;
    hero* h;
    sf::RenderWindow* window;
    
    bool activated;
    int xSprites, ySprites;
    float elapsedTime;
    float triggerTime;
    
    public:
    
    Event();
    Event(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Event(const Event& g);
    virtual ~Event(){};
    
    virtual void saveEvent(std::ofstream& f){};
    virtual void activate(){};
    virtual void update(float eT){};
    virtual void draw(int p){};

};

class ChangeMap: public Event
{
    protected:
    
    int x, y, dir;
    std::string nameMap;
    
    public:
    
    ChangeMap(mapi* Mi, hero* h, sf::RenderWindow* w, std::ifstream& f);
    ChangeMap(const ChangeMap& c);
    ~ChangeMap(){};
    
    void saveEvent(std::ofstream& f);
    void activate();
    void update(float eT){};
    void draw(int p){};
};

class TextInteraction: public Event
{
    protected:
    
    std::string stringFile;
    
    unsigned int characterSize;
    int thickness;
    int iText;
    float margin;
    float interline;
    std::vector<sf::String> text;
    std::vector<sf::Text> textToDraw;
    sf::Font font;
    
    public:
    
    TextInteraction(mapi* Mi, hero* h, sf::RenderWindow* w, std::ifstream& f);
    TextInteraction(const TextInteraction& t);
    virtual ~TextInteraction(){};
    
    virtual void saveEvent(std::ofstream& f);
    virtual void activate();
    void update(float eT);
    void draw(int p);
};

class StaticPNJ: public TextInteraction
{
    private:
    
    int x, y, dir;
    string fileCharacter;
    character* PNJ;
    
    public:
    
    StaticPNJ(mapi* Mi, hero* h, sf::RenderWindow* w, std::ifstream& f);
    StaticPNJ(const StaticPNJ& s);
    ~StaticPNJ();
    
    void activate();
    void saveEvent(std::ofstream& f);
};

#endif