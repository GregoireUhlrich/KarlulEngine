#include "../include/map.h"
#include "../include/gameEvent.h"
using namespace std;

std::string fontEvents = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

gameEvent::gameEvent()
{
    x = y = 0;
    type = "None";
    M = NULL;
}

gameEvent::gameEvent(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string t)
{
    window = w;
    trigger = t;
    type = "None";
    x = xi;
    y = yi;
    dir = di;
    M = Mi;
}

gameEvent::gameEvent(const gameEvent& g)
{
    type = "None";
    x = g.x;
    y = g.y;
    dir = g.dir;
    M = g.M;
}

gameEvent::~gameEvent(){}

string gameEvent::getTrigger() const{ return trigger;}

void gameEvent::setPosition(sf::Vector2i pos)
{
    x = pos.x;
    y = pos.y;
}

bool gameEvent::testHero(hero* h){ return 0;}

changeMap::changeMap(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string t, string n, int xN, int yN, int dN): gameEvent(xi,yi,di,Mi,w,t)
{
    type = "changeMap";
    nameMap = n;
    xNew = xN;
    yNew = yN;
    dirNew = dN;
}

changeMap::~changeMap(){}

bool changeMap::testHero(hero* h)
{
    if (triggerEvent(h))
    {
        activate(h);
        return 1;
    }
    return 0;
}

void changeMap::activate(hero* h)
{
    M->drawClearWindow();
    M->saveMap();
    sf::Vector2u fooVec = M->getSizeSprites();
    h->setX(xNew*fooVec.x);
    h->setY(yNew*fooVec.y);
    h->setDir(dirNew);
    h->releaseKey();
    M->setFile(nameMap);
    M->loadMap();
    M->setState(heros);
}

textInteraction::textInteraction(int xi, int yi, int di, string f, mapi* M, sf::RenderWindow* w, string t): gameEvent(xi,yi,di,M,w,t)
{
    type = "textInteraction";
    stringFile = f;
    activated = 0;
    text = vector<sf::String>(0);
    iText = 0;
    thickness = 3;
    characterSize = 40;
    interline = 1.5;
    margin = 0.1;
    ifstream file(f.c_str(), ios::in);
    string foo = "";
    while (foo != "endFile")
    {
        file>>foo;
        text.push_back(sf::String(foo));
    }
    text.erase(text.begin()+text.size()-1);
    
    font.loadFromFile(fontEvents);
}

textInteraction::~textInteraction(){}

bool textInteraction::testHero(hero* h)
{
    if (triggerEvent(h))
    {
        activate(h);
        h->disableMove();
        return 1;
    }
    else
    {
        if (iText >= text.size() && h->pullAction())
        {
            activated = 0;
            iText = 0;
            textToDraw.clear();
            h->enableMove();
            return 0;
        }
        else if (activated)
        {
            activate(h);
            return 1;
        }
    }
    return 0;
}

void textInteraction::activate(hero* h)
{
    int lx, ly;
    sf::Vector2u fooSize = M->getSizeTexture();
    sf::Vector2u fooSizeSprites = M->getSizeSprites();
    sf::Vector2u fooPos = M->getPosition();
    lx = fooSize.x;
    ly = fooSize.y;
    
    sf::Sprite fooSprite;
    fooSprite.setPosition(0, 2*ly/3.);
    sf::RectangleShape fooShape(sf::Vector2f(lx-thickness*2, ly/3.-thickness));
    fooShape.setPosition(thickness, fooPos.y+2*ly/3.);
    fooShape.setFillColor(sf::Color::White);
    fooShape.setOutlineColor(sf::Color::Blue);
    fooShape.setOutlineThickness(thickness);
    window->draw(fooShape);
    if (text.size() > iText)
    {
        if (h->pullAction() == 1 or activated == 0)
        {
            activated = 1;
            textToDraw.clear();
            int iLine = 0;
            int nLines = floor((ly/3.-characterSize*interline)/(characterSize+characterSize*interline));
            sf::String fooString;
            do{
                fooString = text[iText]+sf::String(" ");
                iText += 1;
                while(text.size() > iText and (fooString.getSize()+text[iText].getSize())*characterSize<lx*2.3*(1-2*margin))
                {
                    fooString += text[iText]+sf::String(" ");
                    iText += 1;
                    if (iLine == nLines - 1)
                    {
                        char lastCharacter = (fooString.toAnsiString())[(int)fooString.getSize()-2];
                        if (lastCharacter == '.')
                            break;
                    }
                }
                textToDraw.push_back(sf::Text(fooString, font, characterSize));
                textToDraw[iLine].setPosition(margin*lx,fooPos.y+2*ly/3.+interline*characterSize+iLine*characterSize*(1+interline));
                textToDraw[iLine].setColor(sf::Color::Black);
                iLine += 1;
            }while(iLine < nLines and text.size() > iText);
            for (int i=0; i<textToDraw.size(); i++)
                window->draw(textToDraw[i]);
        }
        else
        {
            for (int i=0; i<textToDraw.size(); i++)
                window->draw(textToDraw[i]);
        }
    }
    else
    {
        for (int i=0; i<textToDraw.size(); i++)
            window->draw(textToDraw[i]);
    }
    sf::CircleShape fooCircle(10);
    fooCircle.setFillColor(sf::Color::Red);
    fooCircle.setPosition(lx-30,ly+fooPos.y-30);
    if (iText < text.size()) window->draw(fooCircle);
}

staticPNJ::staticPNJ(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w, string t, character* PNJi): textInteraction(xi,yi,di,f,Mi,w,t)
{
    type = "staticPNJ";
    PNJ = PNJi;
    sf::Vector2u foo = M->getSizeSprites();
    x = PNJ->getX()/foo.x;
    y = PNJ->getY()/foo.y;
    PNJ->setDir(dir);
}

staticPNJ::~staticPNJ(){ PNJ = NULL;};

string staticPNJ::getStringPNJ() { return PNJ->getFile();}

bool staticPNJ::testHero(hero* h)
{
    if (!activated && triggerEvent(h))
    {
        PNJ->setDir(3-h->getDir());
        activate(h);
        h->disableMove();
        return 1;
    }
    else
    {
        if (iText >= text.size() && h->pullAction())
        {
            activated = 0;
            iText = 0;
            textToDraw.clear();
            h->enableMove();
            return 0;
        }
        else if (activated)
        {
            activate(h);
            return 1;
        }
    }
    return 0;
}

bool gameEvent::triggerEvent(hero* h)
{
    string trigger = getTrigger();
    if (trigger == "cross")
        return cross(h, x, y);
    else if (trigger == "action")
        return action(h, x, y);
    else if (trigger == "boundaryMap")
        return boundaryMap(h, x, y, dir);
    else if (trigger == "gate")
        return gate(h, x, y, dir);
    else if (trigger == "interactionStaticPNJ")
        return interactionStaticPNJ(h, x, y);
    else return 0;
}

gameEvent* createEvents(ifstream& f, mapi* M)
{/*
    string typeEvent, trigger;
    f>>typeEvent;
    gameEvent* newEvent;
    int fooX, fooY;
    string foo;
    if (typeEvent == "changeMap:")
    {
        int fooXNew, fooYNew, fooDirNew;
        f>>fooX>>fooY>>foo>>fooXNew>>fooYNew>>fooDirNew;
        newEvent = new changeMap(fooX, fooY,M,M->getWindow(), "gate", foo, fooXNew, fooYNew, fooDirNew);
        //cout<<events[i]->x<<" "<<events[i]->y<<" "<<events[i]->dir<<" "<<events[i]->nameMap<<" "<<events[i]->xNew<<" "<<events[i]->yNew<<" "<<events[i]->dirNew<<endl;
    }
    else if (typeEvent == "textInteraction:")
    {
        f>>fooX>>fooY>>foo;
        newEvent = new textInteraction(fooX, fooY,M,M->getWindow(), "action", foo);
        newEvent->setDir(fooDir);
    }
    else if (typeEvent == "staticPNJ:")
    {
        string foo2;
        f>>fooX>>fooY>>foo>>foo2;
        PNJ.push_back(new character("None", foo, fooX,fooY));
        nPNJ += 1;
        newEvent = new staticPNJ(fooX,fooY,M,M->getWindow(),"interactionStaticPNJ", PNJ[nPNJ-1], foo2);
        //cout<<events[i]->x<<" "<<events[i]->y<<" "<<events[i]->dir<<" "<<events[i]->stringFile<<endl;
    }
    else
        newEvent = NULL;
    
    return newEvent;*/
}
