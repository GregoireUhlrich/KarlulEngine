#include "../include/managerTriggerEvent.h"

using namespace std;

Manager::Manager(mapi* Mi, hero* hi, sf::RenderWindow* w)
{
    M = Mi,
    h = hi;
    window = w;
    nEvents = 0;
    events = vector<Event*>(0);
    triggers = vector<Trigger*>(0);
    listCharacter.setWindow(M->getMap());
}

Manager::Manager(const Manager& m)
{
    M = m.M,
    h = m.h;
    window = m.window;
    nEvents = m.nEvents;
    events = m.events;
    triggers = m.triggers;
    listCharacter = m.listCharacter;
}

Manager::~Manager()
{
    for (int i=0; i<nEvents; i++)
    {
        delete events[i];
        delete triggers[i];
    }
    events.clear();
    triggers.clear();
}

void Manager::addCharacter(character* c)
{
    listCharacter.addCharacter(c);
}

void Manager::deleteCharacter(character* c)
{
    listCharacter.deleteCharacter(c);
}

bool Manager::testPNJ(int ix, int iy, int diri)
{
    return listCharacter.testPNJ(ix,iy,diri);
}

void Manager::save(ofstream& f)
{
    f<<nEvents<<" #nEvents\n\n";
    for (int i=0; i<nEvents; i++)
    {
        events[i]->saveEvent(f);
        triggers[i]->saveTrigger(f);
    }
}

void Manager::createEvents(ifstream& f)
{
    string foo;
    f>>nEvents>>foo;
    for (int i=0; i<nEvents; i++)
    {
        f>>foo;
        if (foo == "ChangeMap:")
        {
            events.push_back(new ChangeMap(M,h,window,f));
        }
        else if (foo == "TextInteraction:")
        {
            events.push_back(new TextInteraction(M,h,window,f));
        }
        else if (foo == "StaticPNJ:")
        {
            events.push_back(new StaticPNJ(M,h,window,f));
        }
        else
        {
            events.push_back(NULL);
            triggers.push_back(NULL);
            continue;
        }
        f>>foo;
        if (foo == "Cross:")
        {
            triggers.push_back(new Cross(M,h,window,f));
        }
        else if (foo == "Action:")
        {
            triggers.push_back(new Action(M,h,window,f));
        }
        else if (foo == "Gate:")
        {
            triggers.push_back(new Gate(M,h,window,f));
        }
        else
        {
            delete events[i];
            events[i] = NULL;
            triggers.push_back(NULL);
            continue;
        }
    }
}

void Manager::test(double eT)
{
    for (int i=0; i<nEvents; i++)
    {
        if (triggers[i]->test(eT))
        {
            events[i]->activate();
        }
    }
}

void Manager::update(double eT)
{
    test(eT);
    for (int i=0; i<nEvents; i++)
        events[i]->update(eT);
    h->setAction(0);
}

void Manager::draw(int p)
{
    if (p == 2) listCharacter.draw();
    for (int i=0; i<nEvents; i++)
        events[i]->draw(p);
}

