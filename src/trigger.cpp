#include "../include/trigger.h"
using namespace std;

Trigger::Trigger()
{
    window = NULL;
    M = NULL;
    h = NULL;
}

Trigger::Trigger(mapi* Mi, hero* hi, sf::RenderWindow* wi)
{
    M = Mi;
    h = hi;
    window = wi;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    
    elapsedTime = triggerTime = 0;
    activated = 0;
}

Trigger::Trigger(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f)
{
    M = Mi;
    h = hi;
    window = wi;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    
    elapsedTime = triggerTime = 0;
    activated = 0;
}

Trigger::Trigger(const Trigger& t)
{
    M = t.M;
    h = t.h;
    window = t.window;
    
    xSprites = t.xSprites;
    ySprites = t.ySprites;
    elapsedTime = t.elapsedTime;
    activated = t.activated;
}

Cross::Cross(mapi* Mi, hero* hi, sf::RenderWindow* w, ifstream& f): Trigger(Mi,hi,w,f)
{
    f>>x>>y;
}

Cross::Cross(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): Trigger(Mi,hi,w)
{
    x = (int)stringToUnsignedInt(v[0]);
    y = (int)stringToUnsignedInt(v[1]);
}

Cross::Cross(const Cross& c): Trigger(c)
{
    x = c.x;
    y = c.y;
}

vector<int> Cross::getParams()
{
    vector<int> foo(3);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 0;
    
    return foo;
}

void Cross::saveTrigger(ofstream& f)
{
    f<<"Cross: "<<x<<" "<<y<<endl;
}

bool Cross::test(double eT)
{
    float xH, yH;
    int dirH;
    xH = h->getX()/xSprites;
    yH = h->getY()/ySprites;
    dirH = h->getDir();
    if (h->isOnGrid()) return (round(xH) == x and round(yH) == y);
    else
    {
        switch(dirH)
        {
            case 0:
            return (round(xH) == x and yH > y and yH < y+1);
            break;
            
            case 1:
            return (xH > x-1 and xH < x and round(yH) == y);
            break;
            
            case 2:
            return (xH > x and xH < x+1 and round(yH) == y);
            break;
            
            case 3:
            return (round(xH) == x and yH > y-1 and yH < y);
            break;
            
            default:
            return 0;
        }
    }
}

Action::Action(mapi* Mi, hero* hi, sf::RenderWindow* w, ifstream& f): Trigger(Mi,hi,w,f)
{
    f>>x>>y;
}

Action::Action(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): Trigger(Mi,hi,w)
{
    x = (int)stringToUnsignedInt(v[0]);
    y = (int)stringToUnsignedInt(v[1]);
}

Action::Action(const Action& a): Trigger(a)
{
    x = a.x;
    y = a.y;
}

vector<int> Action::getParams()
{
    vector<int> foo(3);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 0;
    
    return foo;
}

void Action::saveTrigger(ofstream& f)
{
    f<<"Action: "<<x<<" "<<y<<endl;
}

bool Action::test(double eT)
{
    if (!h->isOnGrid()) return 0;
    float xH, yH;
    int dirH;
    xH = round(h->getX()/xSprites);
    yH = round(h->getY()/ySprites);
    dirH = h->getDir();
    if (abs(xH-x)+abs(yH-y) > 1) return 0;
    switch(dirH)
    {
        case 0:
        return (xH == x and yH == y-1 and h->pullAction());
        break;
        
        case 1:
        return (xH == x+1 and yH == y and h->pullAction());
        break;
        
        case 2:
        return (xH == x-1 and yH == y and h->pullAction());
        break;
        
        case 3:
        return (xH == x and yH == y+1 and h->pullAction());
        break;
        
        default:
        return 0;
    }
}

Gate::Gate(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): Trigger(Mi, hi, wi, f)
{
    f>>x>>y>>dir;
}

Gate::Gate(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): Trigger(Mi,hi,w)
{
    x = (int)stringToUnsignedInt(v[0]);
    y = (int)stringToUnsignedInt(v[1]);
    dir = (int)stringToUnsignedInt(v[2]);
}

Gate::Gate(const Gate& g): Trigger(g)
{
    x = g.x;
    y = g.y;
    dir = g.dir;
}

vector<int> Gate::getParams()
{
    vector<int> foo(3);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 1;
    
    return foo;
}

void Gate::saveTrigger(ofstream& f)
{
    f<<"Gate: "<<x<<" "<<y<<" "<<dir<<endl;
}

bool Gate::test(double eT)
{
    return (h->isOnGrid() and round(h->getX()/xSprites) == x and round(h->getY()/ySprites) == y and h->getDir() == dir);
}
