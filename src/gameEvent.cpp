#include "../include/map.h"
#include "../include/gameEvent.h"
using namespace std;

std::string fontEvents = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

void initEvents(varEvents v)
{
    v.nEvents = 1;
    v.nameEvent = new string[1];
    v.nameEvent[0] = "Change map";
}

gameEvent::gameEvent()
{
    x = y = 0;
    type = "None";
    M = NULL;
}

gameEvent::gameEvent(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w)
{
    window = w;
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

void gameEvent::setPosition(sf::Vector2i pos)
{
    x = pos.x;
    y = pos.y;
}

bool gameEvent::testPos(int xi, int yi)
{
    return (x==xi && y==yi);
}

bool gameEvent::testHero(hero* h){ return 0;}

changeMap::changeMap(int xi, int yi, int di, mapi* Mi, sf::RenderWindow* w, string n, int xN, int yN, int dN): gameEvent(xi,yi,di,Mi,w)
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
    float epsilon = 0.1;
    int xSprites, ySprites;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    if (abs(h->getX()/xSprites-x) < epsilon && abs(h->getY()/ySprites-y) < epsilon && h->getWantedMove() == dir)
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

textInteraction::textInteraction(int xi, int yi, int di, string f, mapi* M, sf::RenderWindow* w): gameEvent(xi,yi,di,M,w)
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
    float epsilon = 0.1;
    int xSprites, ySprites;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    if (abs(h->getX()/xSprites-x) < epsilon && abs(h->getY()/ySprites-y) < epsilon && h->getDir() == dir && !activated && h->pullAction())
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

staticPNJ::staticPNJ(int xi, int yi, int di, string f, mapi* Mi, sf::RenderWindow* w, character* PNJi): textInteraction(xi,yi,di,f,Mi,w)
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
    float epsilon = 0.1;
    int xSprites, ySprites;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    if (h->getDir() == 0 && h->getX()/xSprites == x && h->getY()/ySprites == y-1 && !activated && h->pullAction())
    {
        PNJ->setDir(3);
        activate(h);
        h->disableMove();
        return 1;
    }
    else if (h->getDir() == 1 && h->getX()/xSprites == x+1 && h->getY()/ySprites == y && !activated && h->pullAction())
    {
        PNJ->setDir(2);
        activate(h);
        h->disableMove();
        return 1;
    }
    else if (h->getDir() == 2 && h->getX()/xSprites == x-1 && h->getY()/ySprites == y && !activated && h->pullAction())
    {
        PNJ->setDir(1);
        activate(h);
        h->disableMove();
        return 1;
    }
    else if (h->getDir() == 3 && h->getX()/xSprites == x && h->getY()/ySprites == y+1 && !activated && h->pullAction())
    {
        PNJ->setDir(0);
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
