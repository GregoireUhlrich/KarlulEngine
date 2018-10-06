#include "../include/trigger.h"
using namespace std;

Trigger::Trigger()
{
    window = NULL;
    M = NULL;
    h = NULL;
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

Cross::Cross(mapi* Mi, hero* hi, sf::RenderWindow* w, ifstream& f): Trigger(Mi,h,w,f)
{
    f>>x>>y;
}

Cross::Cross(const Cross& c): Trigger(c)
{
    x = c.x;
    y = c.y;
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

Action::Action(mapi* Mi, hero* hi, sf::RenderWindow* w, ifstream& f): Trigger(Mi,h,w,f)
{
    f>>x>>y;
}

Action::Action(const Action& a): Trigger(a)
{
    x = a.x;
    y = a.y;
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
