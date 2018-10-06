#include "../include/character.h"
#include <iostream>
#include <cmath>
using namespace std;

string path_sprites = "";
///*** Constructor ***///

character::character()
{
    file = "";
    name = "";
    x = y = 0;
    direction = 0;
    onGrid = 0;
    sx = sy = dx = dy = 0;
}

character::character(string n, string f, double xi, double yi)
{
    file = f; // file of the texture
    name = n; // name of the carachter
    xSprites = ySprites = 32;
    typeSprite = 0;
    x = xi*xSprites;
    y = yi*ySprites;
    direction = 0; // int between 0 and 3, direction in which the sprite looks
    onGrid = 1; // bool: is the character on a square or not ?
    
    ///* Reading of the texture, loading the init sprite at the init pos *///
    if (!texture.loadFromFile(path_sprites+file))
    {
        cout<<"File not found!"<<endl;
    }
    sf::Vector2u v = texture.getSize();
    sx = v.x/4;
    sy = v.y/4;
    dx = (xSprites - sx)/2;
    dy = ySprites - sy;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
}

character::character(string f, int xi, int yi, int dir)
{
    file = f; // file of the texture
    name = ""; // name of the carachter
    xSprites = ySprites = 32;
    typeSprite = 0;
    x = xi*xSprites;
    y = yi*ySprites;
    direction = dir; // int between 0 and 3, direction in which the sprite looks
    onGrid = 1; // bool: is the character on a square or not ?
    
    ///* Reading of the texture, loading the init sprite at the init pos *///
    if (!texture.loadFromFile(path_sprites+file))
    {
        cout<<"File not found!"<<endl;
    }
    sf::Vector2u v = texture.getSize();
    sx = v.x/4;
    sy = v.y/4;
    dx = (xSprites - sx)/2;
    dy = ySprites - sy;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
}

character::character(const character& c)
{
    file = c.file;
    name = c.name;
    xSprites = c.xSprites;
    ySprites = c.ySprites;
    x = c.x;
    y = c.y;
    direction = c.direction;
    onGrid = c.onGrid;
    sx = c.sx;
    sy = c.sy;
    dx = c.dx;
    dy = c.dy;
    if (!texture.loadFromFile(path_sprites+file))
    {
        cout<<"File not found!"<<endl;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
}

///*** get functions for private attributes ***///
bool character::isOnGrid() { return onGrid;}
string character::getName() const{ return name;}
string character::getFile() const{ return file;}
double character::getX() const{ return x;}
double character::getY() const{ return y;}
int character::getDir() const{ return direction;}
double character::getTypeSprite() const{ return typeSprite;}
sf::Sprite character::getSprite() const{ return sprite;}
sf::Vector2u character::getSizeSprite() const{ return sf::Vector2u(xSprites, ySprites);}

///*** set fucntions for private variables ***///
void character::setName(string n){ name = n;}
void character::setFile(string f){ file = f;}
void character::setX(double xNew){ x = xNew;}
void character::setY(double yNew){ y = yNew;}
void character::setDir(int d){ if (onGrid) direction = d;}

void character::update(double elapsedTime)
{
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
}

///*** Useless function to print some of the attributes ***///
void character::print() const
{
    cout<<"Name: "<<name<<endl;
    cout<<"File: "<<file<<endl;
    cout<<"Position = ["<<x<<" , "<<y<<"]"<<endl;
    cout<<"Direction: "<<direction<<endl;
}

hero::hero(string n, string f, double xi, double yi): character(n,f,xi,yi)
{
    action = 0; // if the player is pressing action
    wantedMove = -1;
    timeKeyPressed = 0; // time during which a key is pressed continuously
    thresholdMove = 0.15; // time from which the character starts to move (in seconds)
    moveEnabled = 1;
    keyPressed = -1; // int between 0 and 3, or -1 if no key is pressed
    speed = 6; // speed of the character in square/sec
    walk = 0.5; // number of times the total animation of walk is played in one square distance 
    walk *= 3*speed; // normalize to the right unit
    
}

void hero::enableMove(){ moveEnabled = 1;}
void hero::disableMove(){ moveEnabled = 0;}
int hero::getKeyPressed() const{ return keyPressed;}
int hero::getWantedMove() const{ return wantedMove;}

void hero::setWantedMove(int wM){  if (timeKeyPressed >= thresholdMove) wantedMove = wM;}
void hero::setAction(bool a){ action = a;}
bool hero::pullAction() { return action; bool foo = action; action = 0; return foo;}
void hero::releaseKey(){ keyPressed = -1;}
    
///*** set the key interpreted by the game ***///
void hero::setKeyPressed(int key)
{
    if (moveEnabled)
    {
        if (key >= 0) // if the key is valid
        {
            if (key == keyPressed+5) 
            {
                keyPressed = -1; // key+5 = the key has been released
                wantedMove = -1;
            }
            else if (key == direction && key!= keyPressed) timeKeyPressed = thresholdMove + 1; 
            if (key < 4 && keyPressed == -1 && onGrid) // else if no key was already pressed
            {                                      // we take the new key 
                keyPressed = key;
                direction = key;
            }
        }
    }
    else keyPressed = -1;
}

///*** Update position and sprite for a given elapsed time ***///
void hero::update(double elapsedTime)
{
    if (moveEnabled)
    {
        if (keyPressed >= 0) //we move
        {
            if (timeKeyPressed <= thresholdMove) timeKeyPressed += elapsedTime; // to make sure the variable timeKeyPressed don't overflow if we don't stop moving ! 
            else
            {
                if (onGrid) onGrid = 0;
                move(elapsedTime);
                direction = keyPressed;
                wantedMove = direction;
            }
        }
        else if (!onGrid)  // we finish/continue the move the arrive the next square
        {
            moveCarefully(elapsedTime);
        }
        else timeKeyPressed = 0; // if we don't move at all we 
        updateSprite(elapsedTime); //update of the sprite with the new position
    }
}

///*** Move the caracter, d = v*t ***///
void hero::move(double elapsedTime)
{
    int dir = -1;
    if (keyPressed != -1)
        dir = keyPressed;
    else
        dir = direction;
    switch(dir)
    {
        case 0:
        y += speed*elapsedTime*ySprites;
        break;
        
        case 1:
        x -= speed*elapsedTime*xSprites;
        break;
        
        case 2:
        x += speed*elapsedTime*xSprites;
        break;
        
        case 3:
        y -= speed*elapsedTime*ySprites;
        break;
    }
}

///*** Finish a movement when the character is between 2 squares ***///
void hero::moveCarefully(double elapsedTime)
{
    double deltaX, deltaY; // distance before the next square
    double d = speed*elapsedTime; //distance the character would walk normally
    switch(direction)
    {
        case 0:
        deltaY = ceil(y/ySprites)*ySprites - y; // distance between the caracter and the next square
        if (d*ySprites < deltaY) y += d*ySprites; // we walk normally
        else {y += deltaY; onGrid = 1; typeSprite=0.;} // we finish on the square
        break;
        
        case 1:
        deltaX = x - floor(x/xSprites)*xSprites;
        if (d*xSprites < deltaX) x -= d*xSprites;
        else {x -= deltaX; onGrid = 1;; typeSprite=0.;}
        break;
        
        case 2:
        deltaX = ceil(x/xSprites)*xSprites - x;
        if (d*xSprites < deltaX) x += d*xSprites;
        else {x += deltaX; onGrid = 1;; typeSprite=0.;}
        break;
        
        case 3:
        deltaY = y - floor(y/ySprites)*ySprites;
        if (d*ySprites < deltaY) y -= d*ySprites;
        else {y -= deltaY; onGrid = 1;; typeSprite=0.;}
        break;
    }
}

/*///*** Update of the sprite with new positions x(t), y(t),
direction and type of the sprite ***///*/
void hero::updateSprite(double elapsedTime)
{
    if (!onGrid) // if not on the grid, we must animate the character 
    {
        typeSprite += walk*elapsedTime;
        if (typeSprite > 3) typeSprite -= 4;
    }
    else typeSprite = 0;
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
}

character& character::operator=(const character& c) // copy assignment
{
    file = c.file;
    name = c.name;
    xSprites = c.xSprites;
    ySprites = c.ySprites;
    x = c.x;
    y = c.y;
    direction = c.direction;
    onGrid = c.onGrid;
    sx = c.sx;
    sy = c.sy;
    dx = c.dx;
    dy = c.dy;
    if (!texture.loadFromFile(path_sprites+file))
    {
        cout<<"File not found!"<<endl;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sx*ceil(typeSprite),sy*direction,sx,sy));
    sprite.setPosition(sf::Vector2f(x+dx,y+dy));
    return *this;
}

ListCharacter::ListCharacter()
{
    nCharacter = 0;
    list = vector<character*>(0);
}

ListCharacter::ListCharacter(sf::RenderTexture* w)
{
    window = w;
    nCharacter = 0;
    list = vector<character*>(0);
}

ListCharacter::~ListCharacter()
{
    for (int i=0; i<nCharacter; i++)
        delete list[i];
    nCharacter = 0;
    list.clear();
}

bool ListCharacter::testPNJ(int ix, int iy, int dir)
{
    switch(dir)
    {
        case 0:
        for (int i=0; i<nCharacter; i++)
            if (list[i]->getX()/xSprites == ix && list[i]->getY()/ySprites == iy+1)
                return 0;
        break;
        
        case 1:
        for (int i=0; i<nCharacter; i++)
            if (list[i]->getX()/xSprites == ix-1 && list[i]->getY()/ySprites == iy)
                return 0;
        break;
        
        case 2:
        for (int i=0; i<nCharacter; i++)
            if (list[i]->getX()/xSprites == ix+1 && list[i]->getY()/ySprites == iy)
                return 0;
        break;
        
        case 3:
        for (int i=0; i<nCharacter; i++)
            if (list[i]->getX()/xSprites == ix && list[i]->getY()/ySprites == iy-1)
                return 0;
        break;    
    }
    return 1;
}

void ListCharacter::setWindow(sf::RenderTexture* w)
{   
    window = w;
}

void ListCharacter::addCharacter(character* c)
{
    list.push_back(c);
    nCharacter += 1;
    if (nCharacter == 1)
    {
        sf::Vector2u foo = c->getSizeSprite();
        xSprites = foo.x;
        ySprites = foo.y;
    }
}

void ListCharacter::deleteCharacter(character* c)
{
    bool characterErased = 0;
    for (int i=0; i<nCharacter; i++)
    {
        if (c == list[i])
        {   
            characterErased = 1;
            list.erase(list.begin()+i);
            nCharacter -= 1;
            break;
        }
    }
    if (!characterErased) cout<<"Unable to erase character, was not in the list!\n";
}

void ListCharacter::draw()
{
    vector<float> yCharacter(nCharacter);
    vector<int> posToDraw(nCharacter);
    for (int i=0; i<nCharacter; i++)
    {
        yCharacter[i] = list[i]->getY();
        posToDraw[i] = 0;
    }
    float max;
    int iMax;
    for (int i=0; i<nCharacter; i++)
    {   
        max = 0;
        iMax = 0;
        for (int j=0; j<nCharacter; j++)
        {
            if (yCharacter[j] > max)
            {
                max = list[j]->getY();
                iMax = j;
            }
        }
        posToDraw[i] = iMax;
        yCharacter[iMax] = -1;
    }
    for (int i=0; i<nCharacter; i++)
    {
        window->draw(list[posToDraw[i]]->getSprite());
    }
}
