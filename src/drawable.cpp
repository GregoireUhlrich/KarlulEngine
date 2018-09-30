#include "../include/drawable.h"
#include <string>
#include <iostream>
using namespace std;

drawable::drawable(sf::RenderTarget* w)
{
    x = y = 0;
    lx = ly = 0;
    delta = 0;
    window = w;
    sizeWindow = window->getSize();
    posClick = sf::Vector2i(0,0);
    posMouse = sf::Vector2i(0,0);
    
    isMouseHere = isMousePressed = 0;
}

drawable::~drawable()
{}

void drawable::setPosition(double xNew, double yNew)
{
    x = xNew;
    y = yNew;
}

void drawable::setPosition(sf::Vector2f vNew)
{
    x = vNew.x;
    y = vNew.y;
}

sf::Vector2i drawable::getPosition() const { return sf::Vector2i(x,y);}
sf::Vector2i drawable::getSize() const { return sf::Vector2i(lx,ly);}
sf::RenderTarget* drawable::getWindow() const { return window;}
sf::Vector2u drawable::getSizeWindow() const { return sizeWindow;}
sf::Vector2i drawable::getPosClick() const { return posClick;}
sf::Vector2i drawable::getPosMouse() const { return posMouse;}
int drawable::getDelta() const { return delta;}
bool drawable::getIsMouseHere() const { return isMouseHere;}
bool drawable::getIsMousePressed() const { return isMousePressed;}
bool drawable::getClick() const { return click;}

int drawable::testMouse(sf::Vector2i v)
{
    int xMouse = v.x;
    int yMouse = v.y;
    posMouse = sf::Vector2i(xMouse,yMouse);
    
    if (xMouse>x && xMouse<x+lx && yMouse>y && yMouse<y+ly)
        isMouseHere = 1;
    else
        isMouseHere = 0;
    
    return isMouseHere;
}

void drawable::setMouse(bool s)
{
    if (s == 1) isMouseHere = 1;
    else
    {
        isMouseHere = 0;
        isMousePressed = 0;
        click = 0;
    }
}

void drawable::mousePressed(sf::Vector2i posMouse)
{
    if (testMouse(posMouse))
    {
        isMousePressed = 1;
        if (!click)
        {
            click = 1;
            posClick = posMouse;
        }
    }
}

void drawable::mouseReleased()
{
    isMousePressed = 0;
    click = 0;
}

void drawable::mouseWheel(int d)
{
    if (isMouseHere)
        delta += d;
    else
        delta = 0;
}

void drawable::windowResized(sf::Vector2u newSizeWindow)
{
    sizeWindow = newSizeWindow;
}

void drawable::setColor(const sf::Color& c){}

bool drawable::buttonPressed(sf::Vector2i posMouse) {return 0;}

void drawable::closeWindow()
{
    window = 0;
}

movingCircle::movingCircle(sf::RenderTarget *w, const sf::Color& c, double xi, double yi, double ri): drawable(w)
{
    x=xi; y=yi; r=ri;
    posMouse = sf::Vector2i(0,0);
    shape = sf::CircleShape(r);
    shape.setFillColor(c);
    shape.setPosition(x,y);
}

movingCircle::~movingCircle(){}

sf::CircleShape movingCircle::getShape() const{ return shape;}

void movingCircle::setShape(sf::CircleShape& s)
{
    shape = s;
}

void movingCircle::setRadius(double ri)
{
    r = ri;
    shape.setRadius(r);
}

void movingCircle::setColor(const sf::Color& c)
{
    shape.setFillColor(c);
}

int movingCircle::testMouse(sf::Vector2i v)
{
    double distance = sqrt(pow(v.x-x-r,2)+pow(v.y-y-r,2));
    posMouse = v;
    if (distance < r)
    {
        isMouseHere = 1;
    }
    else 
    {
        isMouseHere = 0;
    }
    return isMouseHere;
}

void movingCircle::update()
{
    if (isMousePressed)
    {
        x += posMouse.x-posClick.x;
        y += posMouse.y-posClick.y;
        posClick = posMouse;
        shape.setPosition(x,y);
    }
}

void movingCircle::draw()
{
    window->draw(shape);
}

imagePNG::imagePNG(sf::RenderTarget* w, string f, char p, int y0i, int thicknessi): drawable(w)
{
    xSprites = ySprites = 32;
    yScroll = ySprites;
    select = 0, loadedSprites = 1;
    y0 = y0i;
    takeMidX = takeMidY = 0;
    
    nxSprites = 0;
    nySprites = 0;
    spriteToPull = 0;
    thickness = thicknessi;
    
    file = f;
    texture.loadFromFile(file);
    sf::Vector2u v = texture.getSize();
    lx = v.x;
    ly = v.y;
    renderTexture = new sf::RenderTexture();
    lyView = sizeWindow.y-y0-2*thickness;
    ly = max(ly,lyView);
    renderTexture->create(lx,ly);
    
    pos = p;
    if (pos == 'R')
        x = sizeWindow.x - lx - 2*thickness;
    else
        x = 0;

    y = 0;
    backGShape.setFillColor(sf::Color::White);
    backGShape.setPosition(0,0);
    backGShape.setSize(sf::Vector2f(lx,ly));
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color(84,106,139));
    selectRect.setOutlineThickness(3.);
    view = sf::IntRect(0,y,lx,lyView);
}

imagePNG::~imagePNG()
{
    if (spriteToPull != 0)
        delete[] spriteToPull;
}

string imagePNG::getFile() const
{
    return file;
}

int imagePNG::testMouse(sf::Vector2i v)
{
    posMouse = v;
    if (posMouse.x > x+thickness && posMouse.x < x+thickness+lx && posMouse.y > y0+thickness && posMouse.y < sizeWindow.y-thickness)
        isMouseHere = 1;
    else
        isMouseHere = 0;
    
    return isMouseHere;
}

sf::Vector2i imagePNG::convertPosMouse(sf::Vector2i p)
{
    sf::Vector2i posTopLeft;
    posTopLeft.x = view.left;
    posTopLeft.y = view.top;
    
    p.x += posTopLeft.x - thickness;
    p.y += posTopLeft.y - (y0+thickness);
    
    return p;
}

sf::Vector2i imagePNG::invConvertPosMouse(sf::Vector2i p)
{
    sf::Vector2i posTopLeft;
    posTopLeft.x = view.left;
    posTopLeft.y = view.top;
    
    p.x -= posTopLeft.x - thickness;
    p.y -= posTopLeft.y - (y0+thickness);
    
    return p;
}

sf::Sprite imagePNG::getSprite() const { return sprite;}
sf::RectangleShape imagePNG::getShapeBG() const { return backGShape;}

sf::Vector2u imagePNG::getNSprites() const { return sf::Vector2u(nxSprites,nySprites);}

void imagePNG::setTakeMidX(bool t) { takeMidX = t;}
void imagePNG::setTakeMidY(bool t) { takeMidY = t;}

int* imagePNG::pullSprites()
{
    return spriteToPull;
}

void imagePNG::freeSprites()
{
    nxSprites = 0;
    nySprites = 0;
    select = 0;
    if (spriteToPull != 0)
    {
        delete[] spriteToPull;
        spriteToPull = 0;
    }
    loadedSprites = 0;
}

void imagePNG::windowResized(sf::Vector2u newSizeWindow)
{
    sizeWindow = newSizeWindow;
    lyView = sizeWindow.y-y0-2*thickness;
    ly = max(ly,lyView);
    renderTexture->create(lx,ly);
    
    if (pos == 'R')
        x = sizeWindow.x - lx - 2*thickness;
    else
        x = 0;

    y = 0;
    backGShape.setSize(sf::Vector2f(lx,ly));
    view = sf::IntRect(0,y,lx,lyView);
}

void imagePNG::update()
{
    sf::Vector2i foo;
    sf::Vector2i effPosClick = convertPosMouse(posClick);
    foo = convertPosMouse(posMouse);
    sf::Vector2i posMouse = foo;
    if (pos == 'R' && sizeWindow.x - lx - 2*thickness != x)
        x = sizeWindow.x - lx - 2*thickness;
    sf::Vector2f fooSize = backGShape.getSize();
    view = sf::IntRect(0,y,lx,lyView);
    if (isMouseHere || select)
    {
        if (delta != 0)
        {
            delta = delta*yScroll;
            if (y-delta < 0) delta = y;
            else if (y-delta+lyView > ly) delta = y+lyView-ly;
            y -= delta;
            view = sf::IntRect(0,y,lx,lyView);
            delta = 0;    
        }
        if (isMousePressed)
        {
            if (!takeMidX)
                effPosClick.x = floor((effPosClick.x*1.)/xSprites)*xSprites;
            else
                effPosClick.x = floor((effPosClick.x*1.-xSprites/2)/xSprites)*xSprites+xSprites/2;
            if (!takeMidY)
                effPosClick.y = floor((effPosClick.y*1.)/ySprites)*ySprites;
            else
                effPosClick.y = floor((effPosClick.y*1.-ySprites/2)/ySprites)*ySprites+ySprites/2;
            if (!select || (select && loadedSprites))
            {
                loadedSprites = 0;
                select = 1;
                if (spriteToPull != 0) delete[] spriteToPull;
                spriteToPull = 0;
                nySprites = 0;
                nxSprites = 0;
            }
            else if (select)
            {
                int fooInt[2];
                if (!takeMidX)
                    fooInt[0] = ceil((posMouse.x*1.)/xSprites)*xSprites;
                else
                    fooInt[0] = ceil((posMouse.x*1.-xSprites/2)/xSprites)*xSprites+xSprites/2;
                if (!takeMidY)
                    fooInt[1] = ceil((posMouse.y*1.)/ySprites)*ySprites;
                else
                    fooInt[1] = ceil((posMouse.y*1.-ySprites/2)/ySprites)*ySprites+ySprites/2;
        
                int xmin = min(effPosClick.x, fooInt[0]);
                int xmax = max(effPosClick.x, fooInt[0]);
                int ymin = min(effPosClick.y, fooInt[1]);
                int ymax = max(effPosClick.y, fooInt[1]);
                if (!takeMidX)
                {
                    xmin = max(xmin, 0);
                    xmax = min(xmax, lx);
                }
                else
                {
                    xmin = max(xmin, xSprites/2);
                    xmax = min(xmax, lx-xSprites/2);
                }
                if (!takeMidY)            
                {
                    ymin = max(ymin, 0);
                    ymax = min(ymax, ly);
                }
                else
                {
                    ymin = max(ymin, ySprites/2);
                    ymax = min(ymax, ly-ySprites/2);
                }
        
                selectRect.setPosition(xmin, ymin);
                selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
            }
        }
        else if (select)
        {
            loadedSprites = 1;
            sf::Vector2f foo;
            sf::Vector2f foo2;
            foo = selectRect.getPosition();
            foo2 = selectRect.getSize();
            double xmin, ymin, xmax, ymax;
            if (!takeMidX)
            {
                xmin = min(floor((foo.x)/xSprites)*xSprites,(float)lx-xSprites);
                xmax = max(xmin+xSprites,round((foo.x+foo2.x)/xSprites)*xSprites);
                xmax = min(xmax,(double)lx);
            }
            else
            {
                xmin = min(floor((foo.x)*2./xSprites)*xSprites/2.,(float)lx-1.5*xSprites);
                xmax = max(xmin+xSprites,round((foo.x+foo2.x-xSprites/2)/xSprites)*xSprites+xSprites/2);    
                xmax = min(xmax, (double)lx-xSprites/2.);
            }
            if (!takeMidY)
            {
                ymin = min(floor((foo.y)/ySprites)*ySprites,(float)ly-ySprites);
                ymax = max(ymin+ySprites,round((foo.y+foo2.y)/ySprites)*ySprites);
                ymax = min(ymax,(double)ly);
            }
            else
            {
                ymin = min(floor((foo.y)*2./ySprites)*ySprites/2.,(float)ly-1.5*ySprites);
                ymax = max(ymin+ySprites,round((foo.y+foo2.y-ySprites/2)/ySprites)*ySprites+ySprites/2);    
                ymax = min(ymax, (double)ly-ySprites/2.);        
            }
            selectRect.setPosition(xmin,ymin);
            selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
    
            nxSprites = round((xmax-xmin)/xSprites);
            nySprites = round((ymax-ymin)/ySprites);
            spriteToPull = new int[2];
            spriteToPull[0] = xmin;
            spriteToPull[1] = ymin;
        }
    }
    posClick = invConvertPosMouse(effPosClick);
}

void imagePNG::draw()
{
    renderTexture->clear(sf::Color::White);
    sprite.setTexture(texture);
    sprite.setPosition(0,0);
    renderTexture->draw(sprite);
    if (select || isMousePressed)
        renderTexture->draw(selectRect);
    renderTexture->display();
    sprite2.setTexture(renderTexture->getTexture());
    view = sf::IntRect(0,y,lx,lyView);
    sprite2.setTextureRect(view);
    sprite2.setPosition(x+thickness, y0+thickness);
    window->draw(sprite2);
}
