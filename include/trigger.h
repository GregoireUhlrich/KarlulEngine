#ifndef TRIGGER_H_INCLUDED
#define TRIGGER_H_INCLUDED

#include "map.h"
#include "button.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Trigger{

    protected:
    
    mapi* M;
    hero* h;
    sf::RenderWindow* window;
    std::string name;
    
    bool activated;
    int xSprites, ySprites;
    float elapsedTime;
    float triggerTime;
    
    public:
    
    Trigger();
    Trigger(mapi* Mi, hero* h, sf::RenderWindow* w);
    Trigger(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Trigger(const Trigger& t);
    virtual ~Trigger(){};
    
    std::string getName();
    virtual std::vector<std::string> getStrings(){ return std::vector<std::string>(0);};
    virtual void saveTrigger(ofstream& f){};
    virtual bool test(double eT){return 0;};
};

class Cross: public Trigger{
    
    protected:
    
    int x, y;
    
    public:
    
    Cross(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Cross(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v);
    Cross(const Cross& c);
    ~Cross(){};
    
    static vector<int> getParams();
    std::vector<std::string> getStrings();
    void saveTrigger(ofstream& f);
    bool test(double eT);
};

class Action: public Trigger{
    
    protected:
    
    int x, y;
    
    public:
    
    Action(mapi* Mi, hero* h, sf::RenderWindow* w, ifstream& f);
    Action(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v);
    Action(const Action& c);
    ~Action(){};
    
    static vector<int> getParams();
    std::vector<std::string> getStrings();
    void saveTrigger(ofstream& f);
    bool test(double eT);
};

class Gate: public Trigger{

    protected:
    
    int x, y, dir;
    bool activated;
    float threshold, elapsedTime;
    
    public:
    
    Gate(mapi* M, hero* h, sf::RenderWindow* w, ifstream& f);
    Gate(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v);
    Gate(const Gate& g);
    ~Gate(){};
    
    static vector<int> getParams();
    std::vector<std::string> getStrings();
    void saveTrigger(ofstream& f);
    bool test(double eT);
};

class TurningAround: public Trigger{

    protected:
    
    int x, y;
    int step;
    int dir0;
    bool activated;
    float elapsedTime, threshold;
    
    public:
    
    TurningAround(mapi* M, hero* h, sf::RenderWindow* w, ifstream& f);
    TurningAround(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v);
    TurningAround(const TurningAround& g);
    ~TurningAround(){};
    
    static vector<int> getParams();
    std::vector<std::string> getStrings();
    void saveTrigger(ofstream& f);
    bool test(double eT);
};

#endif
