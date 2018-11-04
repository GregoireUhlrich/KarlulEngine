#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <iostream>
#include <vector>
#include "event.h"
#include "trigger.h"
#include "scenario.h"

class Manager{
    
    private:
    
    sf::RenderWindow* window;
    mapi* M;
    hero* h;
    
    int nEvents;
    std::vector<Event*> events;
    std::vector<Trigger*> triggers;
    ListCharacter listCharacter;
    
    public:
    
    Manager(mapi* M, hero* h, sf::RenderWindow* w);
    Manager(const Manager& m);
    ~Manager();
    
    std::vector<std::string> getNames();
    
    void addEvent();
    void deleteEvent(string c);
    
    void addCharacter(character* c);
    void deleteCharacter(character* c);
    bool testPNJ(int ix, int iy, int diri);
    
    void save(std::ofstream& f);
    void createEvents(std::ifstream& f);
    void test(float eT);
    
    void update(float eT);
    void draw(int p);
    
    void addEventWindow();
    void modifyEventWindow(int indexEvent);
};



#endif
