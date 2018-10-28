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

string Event::getType()
{
    return type;
}

ChangeMap::ChangeMap(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): Event(Mi,hi,wi,f)
{
    f>>nameMap>>x>>y>>dir;
    type = "Change Map";
}

ChangeMap::ChangeMap(mapi* Mi, hero* h, sf::RenderWindow* w, std::vector<std::string> v): Event(Mi,h,w)
{
    name = v[0];
    x = (int)stringToUnsignedInt(v[1]);
    y = (int)stringToUnsignedInt(v[2]);
    dir = (int)stringToUnsignedInt(v[3]);
    nameMap = v[4]+".txt";
    type = "Change Map";
}

ChangeMap::ChangeMap(const ChangeMap& c): Event(c)
{
    x = c.x;
    y = c.y;
    dir = c.dir;
    type = "Change Map";
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

vector<string> ChangeMap::getStrings()
{
    vector<string> foo(6);
    foo[0] = name;
    foo[1] = unsignedIntToString((unsigned int)x);
    foo[2] = unsignedIntToString((unsigned int)y);
    foo[3] = unsignedIntToString((unsigned int)dir);
    string foo2 = nameMap.substr(nameMap.find('/')+1,nameMap.length()-(nameMap.find('/')+1));
    foo2 = foo2.substr(0,foo2.find('.'));
    foo[4] = foo2;
    foo[5] = "";
    
    return foo;
} 

TextInteraction::TextInteraction(mapi* Mi, hero* hi, sf::RenderWindow* wi): Event(Mi,hi,wi)
{
    type = "Text Interaction";
}

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
    type = "Text Interaction";
}

TextInteraction::TextInteraction(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): Event(Mi,hi,w)
{
    name = v[0];
    stringFile = "Texts/"+v[4]+".txt";
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
    type = "Text Interaction";
}

TextInteraction::TextInteraction(const TextInteraction& t)
{
    text = t.text;
    type = "Text Interaction";
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

vector<string> TextInteraction::getStrings()
{
    vector<string> foo(6);
    foo[0] = name;
    foo[1] = "";
    foo[2] = "";
    foo[3] = "";
    string foo2 = stringFile.substr(stringFile.find('/')+1,stringFile.length()-(stringFile.find('/')+1));
    foo2 = foo2.substr(0,foo2.find('.'));
    foo[4] = foo2;
    foo[5] = "";
    
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
    type = "Static PNJ";
}

StaticPNJ::StaticPNJ(mapi* Mi, hero* hi, sf::RenderWindow* w, std::vector<std::string> v): TextInteraction(Mi,hi,w)
{
    stringFile = "Texts/"+v[4]+".txt";
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
    
    name = v[0];
    x = (int)stringToUnsignedInt(v[1]);
    y = (int)stringToUnsignedInt(v[2]);
    dir = (int)stringToUnsignedInt(v[3]);
    fileCharacter = "Graphics/"+v[5]+".png";
    PNJ = new character(fileCharacter,x,y,dir);
    M->addCharacter(PNJ);
    type = "Static PNJ";
}


StaticPNJ::StaticPNJ(const StaticPNJ& s): TextInteraction(s)
{
    fileCharacter = s.fileCharacter;
    PNJ = new character(*s.PNJ);
    M->addCharacter(PNJ);
    type = "Static PNJ";
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

vector<string> StaticPNJ::getStrings()
{
    vector<string> foo(6);
    foo[0] = name;
    foo[1] = unsignedIntToString((unsigned int)x);
    foo[2] = unsignedIntToString((unsigned int)y);
    foo[3] = unsignedIntToString((unsigned int)dir);
    string foo2 = stringFile.substr(stringFile.find('/')+1,stringFile.length()-(stringFile.find('/')+1));
    foo2 = foo2.substr(0,foo2.find('.'));
    foo[4] = foo2;
    foo2 = fileCharacter.substr(fileCharacter.find('/')+1,fileCharacter.length()-(fileCharacter.find('/')+1));
    foo2 = foo2.substr(0,foo2.find('.'));
    foo[5] = foo2;
    
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

MiniJeuDamier::MiniJeuDamier(mapi* Mi, hero* hi, sf::RenderWindow* wi, ifstream& f): Event(Mi,hi,wi,f)
{
    f>>n>>modulo;
    type = "Mini Jeu Damier";
}

MiniJeuDamier::MiniJeuDamier(mapi* Mi, hero* hi, sf::RenderWindow* wi, vector<string> v): Event(Mi,hi,wi)
{
    name = v[0];
    type = "Mini Jeu Damier";
    n = stringToUnsignedInt(v[1]);
    int foo = stringToUnsignedInt(v[2]);
    modulo = 0;
    if (foo == 1) modulo = 1;
}

MiniJeuDamier::MiniJeuDamier(const MiniJeuDamier& m): Event(m)
{
    n = m.n;
    modulo = m.modulo;
}

vector<int> MiniJeuDamier::getParams()
{
    vector<int> foo(5);
    foo[0] = 1;
    foo[1] = 1;
    foo[2] = 0;
    foo[3] = 0;
    foo[4] = 0;
    
    return foo;
}

vector<string> MiniJeuDamier::getStrings()
{
    vector<string> foo(6);
    foo[0] = name;
    foo[1] = unsignedIntToString((unsigned int)n);
    foo[2] = unsignedIntToString((unsigned int)modulo);
    foo[3] = "";
    foo[4] = "";
    foo[5] = "";
    
    return foo;
} 

void MiniJeuDamier::saveEvent(ofstream& f)
{
    f<<"MiniJeuDamier: "<<name<<" "<<n<<" "<<modulo<<" ";
}

bool MiniJeuDamier::testFin(vector<vector<bool > > d)
{
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (!d[i][j])
                return 0;
    return 1;
}

void MiniJeuDamier::activate()
{
    int mode = 0;
    int tailleCase = 100;
    vector<vector<bool> > damier(n);
    for (int i=0; i<n; i++)
    {
        damier[i] = vector<bool>(n);
        for (int j=0; j<n; j++)
            damier[i][j] = 0;
    }
    vector<sf::Color> color(2);
    color[0] = sf::Color::Blue;
    color[1] = sf::Color::Red;
    sf::Color colorFin = sf::Color::Yellow;
    
    vector<vector<sf::RectangleShape> > shapes(n);
    for (int i=0; i<n; i++)
    {
        shapes[i] = vector<sf::RectangleShape>(n);
        for (int j=0; j<n; j++)
        {
            shapes[i][j].setFillColor(color[0]);
            shapes[i][j].setSize(sf::Vector2f(tailleCase,tailleCase));
            shapes[i][j].setPosition(i*tailleCase,j*tailleCase);
            shapes[i][j].setOutlineThickness(2);
            shapes[i][j].setOutlineColor(sf::Color::Black);
        }
    }
    
    sf::RenderWindow windowJeu(sf::VideoMode(n*tailleCase,n*tailleCase), "Damier");
    sf::SoundBuffer buffer;
    buffer.loadFromFile("Audio/lowDown.ogg");
    sf::Sound son;
    son.setBuffer(buffer);
    sf::Vector2i posMouse;
    sf::Event event;
    sf::Clock clock;
    sf::Time time = clock.restart();
    double timeFromWin = 0;
    
    while (windowJeu.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowJeu);
        posMouse.x = min(posMouse.x/tailleCase,n-1);
        posMouse.y = min(posMouse.y/tailleCase,n-1);
        if (window->hasFocus()) windowJeu.requestFocus();
        while(window->pollEvent(event));
        while(windowJeu.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left and mode == 0)
                {
                    int ix = posMouse.x;
                    int iy = posMouse.y;
                    if (modulo)
                    {
                        damier[ix][iy] = !damier[ix][iy];
                        damier[(ix+1)%n][iy] = !damier[(ix+1)%n][iy];
                        damier[ix][(iy+1)%n] = !damier[ix][(iy+1)%n];
                        damier[ix][(iy+2)%n] = !damier[ix][(iy+2)%n];
                        damier[(ix+2)%n][iy] = !damier[(ix+2)%n][iy];
                    }
                    else
                    {
                        damier[ix][iy] = !damier[ix][iy];
                        if (ix+1 < n) damier[ix+1][iy] = !damier[ix+1][iy];
                        if (ix-1 >= 0) damier[ix-1][iy] = !damier[ix-1][iy];
                        if (iy+1 < n) damier[ix][iy+1] = !damier[ix][iy+1];
                        if (iy-1 >= 0) damier[ix][iy-1] = !damier[ix][iy-1];
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right and mode == 0)
                {
                    for (int i=0; i<n; i++)
                        for (int j=0; j<n; j++)
                            damier[i][j] = 0;
                }
            }
            
        }
        time = clock.restart();
        if (mode == 0)
        {
            for (int i=0; i<n; i++)
                for (int j=0; j<n; j++)
                    shapes[i][j].setFillColor(color[damier[i][j]]);
        }
        if (mode == 0 and testFin(damier)) mode = 1;
        if (mode == 1)
        {
            timeFromWin += time.asSeconds();
            if (timeFromWin > 0.5)
            {
                son.play();
                for (int i=0; i<n; i++)
                    for (int j=0; j<n; j++)
                        shapes[i][j].setFillColor(colorFin);
                timeFromWin = 0;
                mode = 2;
            }
        }
        windowJeu.clear();
        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++)
                windowJeu.draw(shapes[i][j]);
        windowJeu.display();
        
        if (mode == 2)
        {
            timeFromWin += time.asSeconds();
            if (timeFromWin > 1) windowJeu.close();
        }
    }
}
