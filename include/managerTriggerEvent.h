#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <iostream>
#include <vector>
#include "event.h"
#include "trigger.h"

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
    
    void addCharacter(character* c);
    void deleteCharacter(character* c);
    bool testPNJ(int ix, int iy, int diri);
    
    void save(std::ofstream& f);
    void createEvents(std::ifstream& f);
    void test(double eT);
    
    void update(double eT);
    void draw(int p);
};

#endif
