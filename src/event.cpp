#include "../include/event.h"

using namespace std;

string fontEvents = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

Event::Event()
{
    window = NULL;
    M = NULL;
    h = NULL;
}

Event::Event(mapi* Mi, hero* hi, sf::RenderWindow* wi)
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

Event::Event(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f)
{
    M = Mi;
    h = hi;
    window = wi;
    sf::Vector2u foo = M->getSizeSprites();
    xSprites = foo.x;
    ySprites = foo.y;
    
    elapsedTime = triggerTime = 0;
    activated = 0;
    f>>name;
}

Event::Event(const Event& e)
{
    M = e.M;
    h = e.h;
    window = e.window;
    xSprites = e.xSprites;
    ySprites = e.ySprites;
    elapsedTime = e.elapsedTime;
    activated = e.activated;
}

string Event::getName()
{
    return name;
}

ChangeMap::ChangeMap(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): Event(Mi,hi,wi,f)
{
    f>>nameMap>>x>>y>>dir;
}

ChangeMap::ChangeMap(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v): Event(Mi,h,w)
{
    x = (int)stringToUnsignedInt(v[0]);
    y = (int)stringToUnsignedInt(v[1]);
    dir = (int)stringToUnsignedInt(v[2]);
    nameMap = v[3]+".txt";
}

ChangeMap::ChangeMap(const ChangeMap& c): Event(c)
{
    x = c.x;
    y = c.y;
    dir = c.dir;
}

void ChangeMap::saveEvent(ofstream& f)
{   
    f<<"ChangeMap: "<<name<<" "<<nameMap<<" "<<x<<" "<<y<<" "<<dir<<" ";
}

void ChangeMap::activate()
{
    int foo = 0;
    if (M->getSaveState() == edited) foo = loadWindow(window);
    if (foo == 1) M->saveMap();
    if (foo == 1 or foo == 0)
    {
        M->drawClearWindow();
        h->setX(x*xSprites);
        h->setY(y*ySprites);
        h->setDir(dir);
        h->releaseKey();
        
        M->setFile(nameMap);
        
        M->setState(heros);
    }
    else
    {
        h->setDir(3-h->getDir());
    }
}

vector<int> ChangeMap::getParams()
{
    vector<int> foo(5);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 1;
    foo[3] = 1;
    foo[4] = 0;
    
    return foo;
}

TextInteraction::TextInteraction(mapi* Mi, hero* hi, sf::RenderWindow* wi): Event(Mi,hi,wi){}

TextInteraction::TextInteraction(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): Event(Mi,hi,wi,f)
{
    f>>stringFile;
    text = vector<sf::String>(0);
    iText = 0;
    thickness = 3;
    characterSize = 40;
    interline = 1.5;
    margin = 0.1;
    ifstream file(stringFile.c_str(), ios::in);
    string foo = "";
    while (foo != "endFile")
    {
        file>>foo;
        text.push_back(sf::String(foo));
    }
    file.close();
    text.erase(text.begin()+text.size()-1);
    
    font.loadFromFile(fontEvents);
}

TextInteraction::TextInteraction(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): Event(Mi,hi,w)
{
    stringFile = "Texts/"+v[3]+".txt";
    text = vector<sf::String>(0);
    iText = 0;
    thickness = 3;
    characterSize = 40;
    interline = 1.5;
    margin = 0.1;
    ifstream file(stringFile.c_str(), ios::in);
    string foo = "";
    while (foo != "endFile")
    {
        file>>foo;
        text.push_back(sf::String(foo));
    }
    file.close();
    text.erase(text.begin()+text.size()-1);
    
    font.loadFromFile(fontEvents);
}

TextInteraction::TextInteraction(const TextInteraction& t)
{
    text = t.text;
}

vector<int> TextInteraction::getParams()
{
    vector<int> foo(5);
    foo[0] = 0;
    foo[1] = 0;
    foo[2] = 0;
    foo[3] = 1;
    foo[4] = 0;
    
    return foo;
}

void TextInteraction::saveEvent(ofstream& f)
{
    f<<"TextInteraction: "<<name<<" "<<stringFile<<" ";
}

void TextInteraction::activate()
{
    if (!activated)
    {
        activated = 1;
        h->disableMove();
    }
}

void TextInteraction::update(float eT)
{
    if (activated && text.size() > iText && h->pullAction())
    {
        int lx, ly;
        sf::Vector2u fooSize = M->getSizeTexture();
        sf::Vector2u fooPos = M->getPosition();
        lx = fooSize.x;
        ly = fooSize.y;
        
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
    }
    else if (iText >= text.size() && h->pullAction())
    {
        activated = 0;
        iText = 0;
        textToDraw.clear();
        h->enableMove();
    }
}

void TextInteraction::draw(int p)
{
    if (p == 4 and activated)
    {
        int lx, ly;
        sf::Vector2u fooSize = M->getSizeTexture();
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
        
        for (int i=0; i<textToDraw.size(); i++)
            window->draw(textToDraw[i]);
        
        sf::CircleShape fooCircle(10);
        fooCircle.setFillColor(sf::Color::Red);
        fooCircle.setPosition(lx-30,ly+fooPos.y-30);
        if (iText < text.size()) window->draw(fooCircle);
    }
}

StaticPNJ::StaticPNJ(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): TextInteraction(Mi,hi,wi,f)
{
    f>>fileCharacter>>x>>y>>dir;
    PNJ = new character(fileCharacter,x,y,dir);
    M->addCharacter(PNJ);
}

StaticPNJ::StaticPNJ(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): TextInteraction(Mi,hi,w)
{
    stringFile = "Texts/"+v[3]+".txt";
    text = vector<sf::String>(0);
    iText = 0;
    thickness = 3;
    characterSize = 40;
    interline = 1.5;
    margin = 0.1;
    ifstream file(stringFile.c_str(), ios::in);
    string foo = "";
    while (foo != "endFile")
    {
        file>>foo;
        text.push_back(sf::String(foo));
    }
    file.close();
    text.erase(text.begin()+text.size()-1);
    
    font.loadFromFile(fontEvents);
    
    x = (int)stringToUnsignedInt(v[0]);
    y = (int)stringToUnsignedInt(v[1]);
    dir = (int)stringToUnsignedInt(v[2]);
    fileCharacter = "Graphics/"+v[4]+".png";
    PNJ = new character(fileCharacter,x,y,dir);
    M->addCharacter(PNJ);
}


StaticPNJ::StaticPNJ(const StaticPNJ& s): TextInteraction(s)
{
    fileCharacter = s.fileCharacter;
    PNJ = new character(*s.PNJ);
    M->addCharacter(PNJ);
}

StaticPNJ::~StaticPNJ()
{
    M->deleteCharacter(PNJ);
    delete PNJ;
}

vector<int> StaticPNJ::getParams()
{
    vector<int> foo(5);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 1;
    foo[3] = 1;
    foo[4] = 1;
    
    return foo;
}

void StaticPNJ::saveEvent(ofstream& f)
{
    f<<"StaticPNJ: "<<name<<" "<<stringFile<<" "<<fileCharacter<<" "<<x<<" "<<y<<" "<<dir<<" ";
}

void StaticPNJ::activate()
{
    if (!activated)
    {
        activated = 1;
        h->disableMove();
        PNJ->setDir(3-h->getDir());
        PNJ->update(0);
    }
}
