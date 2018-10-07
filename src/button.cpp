#include "../include/button.h"
#include "../include/supportWindow.h"
#include <cmath>
#include <iostream>
#include <string>
#include <dirent.h>

sf::String fonts = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

button::button(sf::RenderTarget* w, string t, double xi, double yi, double lxi, double lyi, bool isRighti): drawable(w)
{
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    isPressed = 0;
    rect.setSize(sf::Vector2f(lx,ly));
    rect.setPosition(x,y);
    rect.setFillColor(sf::Color(128,128,128,255));
    rect.setOutlineThickness(3.);
    rect.setOutlineColor(sf::Color(0,0,0,255));
    ratioPressed = 0.95;
    characterSize = 20;
    text.setString(t);
    font.loadFromFile(fonts);
    text.setFont(font);
    text.setCharacterSize(characterSize);
    
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-1.15*characterSize)/2;
    text.setPosition(round(xText), round(yText));
}

button::~button(){} 

sf::RectangleShape button::getRect() const{ return rect;}
sf::Text button::getText() const{ return text;}
sf::Font button::getFont() const{ return font;}
double button::getRatioPressed() const{return ratioPressed;}
unsigned int button::getCharacterSize() const{ return characterSize;}
bool button::getIsRight() const{ return isRight;}

void button::pressButton()
{
    double x0 = x + lx*(1-ratioPressed)/2;
    double y0 = y + ly*(1-ratioPressed)/2;
    rect.setPosition(x0,y0);
    rect.setSize(sf::Vector2f(lx*ratioPressed,ly*ratioPressed));
    
    text.setCharacterSize(characterSize*ratioPressed);
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(xText, yText);
}

void button::releaseButton()
{
    rect.setPosition(x,y);
    rect.setSize(sf::Vector2f(lx,ly));
    
    text.setCharacterSize(characterSize);
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(xText, yText);
}

void button::update()
{
    if (isMousePressed && !isPressed)
    {
        pressButton();
        isPressed = 1;
    }
    else if (!isMousePressed && isPressed)
    {
        releaseButton();
        isPressed = 0;
    }
}

void button::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));
    if (isRight)
    {
        x += newSizeWindow.x - sizeWindow.x;    
        rect.setPosition(x,y);
        sf::Vector2f foo = text.getPosition();
        text.setPosition(foo.x+newSizeWindow.x - sizeWindow.x,foo.y);
    }
    sizeWindow.x = newSizeWindow.x;
    sizeWindow.y = newSizeWindow.y;
}

void button::draw()
{
    window->draw(rect);
    window->draw(text);
}

signalButton::signalButton(sf::RenderTarget *w, string t, sf::Color ci, double x,double y,double lx,double ly,bool isRighti): button(w,t,x,y,lx,ly,isRighti)
{
    enabled = 1;
    c = ci;
    rect.setFillColor(c);
    rect.setOutlineThickness(0);
    text.setColor(sf::Color::White);
}

signalButton::~signalButton(){};

void signalButton::enable()
{
    enabled = 1;
    rect.setFillColor(c);
}

void signalButton::disable()
{
    enabled = 0;
    rect.setFillColor(sf::Color(217,217,217));
    isMouseHere = 0;
    isPressed = 0;
    isMousePressed = 0;
}

bool signalButton::updateSignal()
{
    if (isMousePressed && !isPressed && enabled)
    {
        pressButton();
        isPressed = 1;
    }
    else if (!isMousePressed && isPressed && enabled)
    {
        releaseButton();
        isPressed = 0;
        if (isMouseHere) return 1;
    }
    return 0;
}

buttonIm::buttonIm(sf::RenderTarget* w, mapi* Mi, char c, string t, double x,double y,double lx,double ly,bool isRighti): button(w,t,x,y,lx,ly,isRighti)
{M = Mi; chirality = c;}

buttonIm::~buttonIm(){}

void buttonIm::update()
{
    if (isMousePressed && !isPressed)
    {
        pressButton();
        isPressed = 1;
    }
    else if (!isMousePressed && isPressed)
    {
        releaseButton();
        isPressed = 0;
        M->deletePNG(chirality);
    }
}

buttonMapp::buttonMapp(sf::RenderTarget* w, mapi* Mi, StateMap sMi, string t, double x,double y,double lx,double ly,bool isRighti): button(w,t,x,y,lx,ly,isRighti)
{M = Mi; sM = sMi;}

buttonMapp::~buttonMapp(){}

void buttonMapp::update()
{
    if (isMousePressed && !isPressed)
    {
        pressButton();
        isPressed = 1;
    }
    else if (!isMousePressed && isPressed)
    {
        releaseButton();
        isPressed = 0;
        M->setState(sM);
    }
}

buttonSave::buttonSave(sf::RenderTarget* w, mapi* Mi, char m, string t, double x,double y,double lx,double ly,bool isRighti): button(w,t,x,y,lx,ly,isRighti)
{M = Mi; mode = m;}

buttonSave::~buttonSave(){}

void buttonSave::update()
{
    if (isMousePressed && !isPressed)
    {
        pressButton();
        isPressed = 1;
    }
    else if (!isMousePressed && isPressed)
    {
        releaseButton();
        isPressed = 0;
        if (mode == 'S') M->saveMap();
        else if (mode == 'L') M->loadMap();
        else if (mode == 'I') M->reinitMap();
    }
}

pushButton::pushButton(sf::RenderTarget* w, mapi* Mi, string t, double xi, double yi, double lxi, double lyi, bool isRighti): drawable(w)
{
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    M = Mi;
    isPressed = 0;
    rect.setSize(sf::Vector2f(lx,ly));
    rect.setPosition(x,y);
    rect.setFillColor(sf::Color(128,128,128,255));
    rect.setOutlineThickness(3.);
    rect.setOutlineColor(sf::Color(0,0,0,255));
    characterSize = 20;
    text.setString(t);
    font.loadFromFile(fonts);
    text.setFont(font);
    text.setCharacterSize(characterSize);
    
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(xText, yText);
}

pushButton::pushButton(const pushButton& b): drawable(b.window)
{
    isRight = b.isRight;
    x = b.x; y = b.y; lx = b.lx; ly = b.ly;
    M = b.M;
    isPressed = b.isPressed;
    rect = b.rect;
    characterSize = b.characterSize;
    text = b.text;
    font = b.font;
}

pushButton::~pushButton(){} 

sf::RectangleShape pushButton::getRect() const{ return rect;}
sf::Text pushButton::getText() const{ return text;}

bool pushButton::buttonPressed(sf::Vector2i posMouse)
{
    if (testMouse(posMouse)) isPressed = !isPressed;
    return isMouseHere;
}

void pushButton::setPressed(bool s) {isPressed = s;}

bool pushButton::getPressed() const { return isPressed;} 

void pushButton::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
}

void pushButton::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));    
    if (isRight)
    {
        x += newSizeWindow.x - sizeWindow.x;    
        rect.setPosition(x,y);
        sf::Vector2f foo = text.getPosition();
        text.setPosition(foo.x+newSizeWindow.x - sizeWindow.x,foo.y);
    }
    sizeWindow.x = newSizeWindow.x;
    sizeWindow.y = newSizeWindow.y;
}

void pushButton::draw()
{
    window->draw(rect);
    window->draw(text);
}

pushButtonUX::pushButtonUX(sf::RenderTarget* w, mapi* Mi, double xi, double yi, double lxi, double lyi, bool isRighti): drawable(w)
{
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    M = Mi;
    isPressed = 0;
    
    textureOutline.loadFromFile("Icons/outline.png");
    outline.setTexture(textureOutline);
    outline.setPosition(x,y);
    sf::Vector2u foo = textureOutline.getSize();
    outline.scale(lx*1./foo.x, ly*1./foo.y);
}

pushButtonUX::pushButtonUX(const pushButtonUX& b): drawable(b.window)
{
    isRight = b.isRight;
    x = b.x; y = b.y; lx = b.lx; ly = b.ly;
    M = b.M;
    isPressed = b.isPressed;
    textureOutline = b.textureOutline;
    outline.setTexture(textureOutline);
    outline.setPosition(x,y);
    sf::Vector2u foo = textureOutline.getSize();
    outline.scale(lx*1./foo.x, ly*1./foo.y);
}

pushButtonUX::~pushButtonUX(){} 

bool pushButtonUX::buttonPressed(sf::Vector2i posMouse)
{
    if (testMouse(posMouse)) isPressed = !isPressed;
    return isMouseHere;
}

void pushButtonUX::setPressed(bool s) {isPressed = s;}

bool pushButtonUX::getPressed() const { return isPressed;} 

void pushButtonUX::update()
{}

void pushButtonUX::windowResized(sf::Vector2u newSizeWindow)
{
    if (isRight)
    {
        x += newSizeWindow.x - sizeWindow.x;    
        outline.setPosition(x,y);
    }
    sizeWindow.x = newSizeWindow.x;
    sizeWindow.y = newSizeWindow.y;
}

void pushButtonUX::draw()
{
    if (isMouseHere || isPressed) window->draw(outline);
}

buttonGrid::buttonGrid(sf::RenderTarget* w, mapi* Mi, string t, double xi, double yi, double lxi, double lyi, bool isRighti): pushButton(w,Mi,t,xi,yi,lxi,lyi,isRighti)
{ grid=0;}


buttonGrid::~buttonGrid(){} 

void buttonGrid::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
    if (isPressed && !grid)
    {
        grid = 1;
        M->setGrid(grid);
    }
    else if (!isPressed && grid)
    {
        grid = 0;
        M->setGrid(grid);
    }
}

buttonGridUX::buttonGridUX(sf::RenderTarget* w, mapi* Mi, double xi, double yi, double lxi, double lyi, bool isRighti): pushButtonUX(w,Mi,xi,yi,lxi,lyi,isRighti)
{
    grid=0;
    textureA.loadFromFile("Icons/grid-grey.png");
    textureB.loadFromFile("Icons/grid-blue.png");
    spriteA.setTexture(textureA);
    spriteB.setTexture(textureB);
    xScaleIcon = yScaleIcon = 0.8;
    spriteA.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    spriteB.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    sf::Vector2u foo = textureA.getSize();
    spriteA.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
    foo = textureB.getSize();
    spriteB.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
}


buttonGridUX::~buttonGridUX(){} 

void buttonGridUX::update()
{
    if (isPressed && !grid)
    {
        grid = 1;
        M->setGrid(grid);
    }
    else if (!isPressed && grid)
    {
        grid = 0;
        M->setGrid(grid);
    }
}

void buttonGridUX::draw()
{
    if (isMouseHere || isPressed)
    {
        window->draw(outline);
        window->draw(spriteB);
    }
    else window->draw(spriteA);
}

buttonMap::buttonMap(sf::RenderTarget* w, mapi* Mi, StateMap si, string t, double xi, double yi, double lxi, double lyi, bool isRighti): pushButton(w,Mi,t,xi,yi,lxi,lyi,isRighti)
{ s = si; active = 0;}

buttonMap::~buttonMap(){} 

void buttonMap::setPressed(bool s){ isPressed = s; active=0;}

void buttonMap::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
    if (isPressed && !active)
    {
        active = 1;
        M->setState(s);
    }
    else if (!isPressed && active)
    {
        active = 0;
        M->setState(s);
    }
}

buttonMapUX::buttonMapUX(sf::RenderTarget* w, mapi* Mi, StateMap si, double xi, double yi, double lxi, double lyi, bool isRighti): pushButtonUX(w,Mi,xi,yi,lxi,lyi,isRighti)
{
    s = si;
    active = 0;
    if (s == adding)
    {
        textureA.loadFromFile("Icons/add-grey.png");
        textureB.loadFromFile("Icons/add-green.png");
    }
    else if (s == erasing)
    {
        textureA.loadFromFile("Icons/erase-grey.png");
        textureB.loadFromFile("Icons/erase-red.png");    
    }
    else if (s == selecting)
    {
        textureA.loadFromFile("Icons/select-grey.png");
        textureB.loadFromFile("Icons/select-yellow.png");    
    }
    else if (s == moving)
    {
        textureA.loadFromFile("Icons/move-grey.png");
        textureB.loadFromFile("Icons/move-blue.png");    
    }
    spriteA.setTexture(textureA);
    spriteB.setTexture(textureB);
    xScaleIcon = yScaleIcon = 0.8;
    spriteA.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    spriteB.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    sf::Vector2u foo = textureA.getSize();
    spriteA.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
    foo = textureB.getSize();
    spriteB.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
}

buttonMapUX::~buttonMapUX(){} 

void buttonMapUX::setPressed(bool s){ isPressed = s; active=0;}

void buttonMapUX::update()
{
    if (isPressed && !active)
    {
        active = 1;
        M->setState(s);
    }
    else if (!isPressed && active)
    {
        active = 0;
        M->setState(s);
    }
}

void buttonMapUX::draw()
{
    if (isMouseHere || isPressed)
    {
        window->draw(outline);
        window->draw(spriteB);
    }
    else window->draw(spriteA);
    
}

buttonPrio::buttonPrio(sf::RenderTarget* w, mapi* Mi, int p, string t, double xi, double yi, double lxi, double lyi, bool isRighti): pushButton(w,Mi,t,xi,yi,lxi,lyi,isRighti)
{ prio = p; active = 0;}

buttonPrio::~buttonPrio(){} 

void buttonPrio::setPressed(bool s){ isPressed = s; active=0;}

void buttonPrio::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
    if (isPressed && !active)
    {
        active = 1;
        M->setPrio(prio);
    }
    else if (!isPressed && active)
    {
        active = 0;
        M->setPrio(prio);
    }
}

buttonAllPrio::buttonAllPrio(sf::RenderTarget* w, mapi* Mi, string t, double xi, double yi, double lxi, double lyi, bool isRighti): pushButton(w,Mi,t,xi,yi,lxi,lyi,isRighti)
{ allPrio=0;}

buttonAllPrio::~buttonAllPrio(){} 

void buttonAllPrio::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
    if (isPressed && !allPrio)
    {
        allPrio = 1;
        M->setAllPrio(1);
    }
    else if (!isPressed && allPrio)
    {
        allPrio = 0;
        M->setAllPrio(0);
    }
}

buttonPrioUX::buttonPrioUX(sf::RenderTarget* w, mapi* Mi, int p, double xi, double yi, double lxi, double lyi, bool isRighti): pushButtonUX(w,Mi,xi,yi,lxi,lyi,isRighti)
{
    prio = p;
    active = 0;
    characterSize = 20;
    switch(prio)
    {
    case 0:
        text.setString("0");
        break;
    case 1:
        text.setString("1");
        break;
    case 2:
        text.setString("2");
        break;
    case 3:
        text.setString("3");
        break;
    default:
        text.setString("0");
    }
    font.loadFromFile(fonts);
    text.setFont(font);
    text.setCharacterSize(characterSize);
    
    sf::FloatRect foo = text.getLocalBounds();
    double xText = x+(lx-foo.width)/2;
    double yText = y+(ly-foo.height*2)/2;
    text.setPosition(xText, yText);
}

buttonPrioUX::~buttonPrioUX(){} 

void buttonPrioUX::setPressed(bool s){ isPressed = s; active=0;}

void buttonPrioUX::update()
{
    if (isMouseHere || isPressed) text.setColor(sf::Color(84,106,121));
    else text.setColor(sf::Color(217,217,217));
    if (isPressed && !active)
    {
        active = 1;
        M->setPrio(prio);
    }
    else if (!isPressed && active)
    {
        active = 0;
        M->setPrio(prio);
    }
}

void buttonPrioUX::draw()
{
    if (isMouseHere || isPressed) window->draw(outline);
    window->draw(text);
}

buttonAllPrioUX::buttonAllPrioUX(sf::RenderTarget* w, mapi* Mi, double xi, double yi, double lxi, double lyi, bool isRighti): pushButtonUX(w,Mi,xi,yi,lxi,lyi,isRighti)
{
    allPrio=0;
    textureA.loadFromFile("Icons/layers-grey.png");
    textureB.loadFromFile("Icons/layers-blue.png");
    spriteA.setTexture(textureA);
    spriteB.setTexture(textureB);
    xScaleIcon = yScaleIcon = 0.8;
    spriteA.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    spriteB.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    sf::Vector2u foo = textureA.getSize();
    spriteA.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
    foo = textureB.getSize();
    spriteB.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
}

buttonAllPrioUX::~buttonAllPrioUX(){} 

void buttonAllPrioUX::update()
{
    if (isPressed && !allPrio)
    {
        allPrio = 1;
        M->setAllPrio(1);
    }
    else if (!isPressed && allPrio)
    {
        allPrio = 0;
        M->setAllPrio(0);
    }
}

void buttonAllPrioUX::draw()
{
    if (isMouseHere || isPressed)
    {
        window->draw(outline);
        window->draw(spriteB);
    }
    else window->draw(spriteA);
    
}

buttonShowPass::buttonShowPass(sf::RenderTarget* w, mapi* Mi, string t, double xi, double yi, double lxi, double lyi, bool isRighti): pushButton(w,Mi,t,xi,yi,lxi,lyi,isRighti)
{ showPass = 1;}

buttonShowPass::~buttonShowPass(){} 

void buttonShowPass::update()
{
    if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
    else rect.setOutlineColor(sf::Color(0,0,0,255));
    if (isPressed && !showPass)
    {
        showPass = 1;
        M->setShowPass(1);
    }
    else if (!isPressed && showPass)
    {
        showPass = 0;
        M->setShowPass(0);
    }
}

buttonShowPassUX::buttonShowPassUX(sf::RenderTarget* w, mapi* Mi, double xi, double yi, double lxi, double lyi, bool isRighti): pushButtonUX(w,Mi,xi,yi,lxi,lyi,isRighti)
{
    showPass = 1;
    textureA.loadFromFile("Icons/forbidden-grey.png");
    textureB.loadFromFile("Icons/forbidden-red.png");
    spriteA.setTexture(textureA);
    spriteB.setTexture(textureB);
    xScaleIcon = yScaleIcon = 0.8;
    spriteA.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    spriteB.setPosition(x+(1-xScaleIcon)*lx/2,y+(1-yScaleIcon)*ly/2);
    sf::Vector2u foo = textureA.getSize();
    spriteA.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
    foo = textureB.getSize();
    spriteB.scale(lx*xScaleIcon/foo.x, ly*xScaleIcon/foo.y);
}

buttonShowPassUX::~buttonShowPassUX(){} 

void buttonShowPassUX::update()
{
    if (isPressed && !showPass)
    {
        showPass = 1;
        M->setShowPass(1);
    }
    else if (!isPressed && showPass)
    {
        showPass = 0;
        M->setShowPass(0);
    }
}

void buttonShowPassUX::draw()
{
    if (isMouseHere || isPressed)
    {
        window->draw(outline);
        window->draw(spriteB);
    }
    else window->draw(spriteA);
    
}

interactiveButtonUX::interactiveButtonUX(sf::RenderTarget* w, mapi* Mi, double xi, double yi, double lxi, double lyi, bool isRighti): drawable(w)
{
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    M = Mi;
    ratioPressed = 0.9;
    isPressed = 0;
    
    characterSize = 20;
    click = 0;
    font.loadFromFile(fonts);
    text.setString(sf::String("Run game"));
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setPosition(round(ly+5),round(ly/2+characterSize/2-23));
    text.setColor(sf::Color(97,184,114));
    renderTexture.create(lx,ly);
    
    textureIcon1.loadFromFile("Icons/run-green.png");
    icon1.setTexture(textureIcon1);
    icon1.setPosition(x,y);
    sf::Vector2u foo = textureIcon1.getSize();
    icon1.scale(ly*0.9/foo.y, ly*0.9/foo.y);
    
    icon1.setPosition(0.05*ly,0.05*ly);
}

interactiveButtonUX::interactiveButtonUX(const interactiveButtonUX& b): drawable(b.window)
{
    isRight = b.isRight;
    isPressed = 0;
    ratioPressed = b.ratioPressed;
    x = b.x; y = b.y; lx = b.lx; ly = b.ly;
    M = b.M;
    characterSize = b.characterSize;
    font.loadFromFile(fonts);
    text.setString(sf::String("Run game"));
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setPosition(b.text.getPosition());
    text.setColor(sf::Color(97,184,114));
    textureIcon1 = b.textureIcon1;
    icon1.setTexture(textureIcon1);
    icon1.setPosition(x,y);
    sf::Vector2u foo = textureIcon1.getSize();
    icon1.scale(ly*0.9/foo.y, ly*0.9/foo.y);
    
    icon1.setPosition(0.05*ly,0.05*ly);
}

interactiveButtonUX::~interactiveButtonUX(){} 

void interactiveButtonUX::update()
{
    if (isMousePressed and isPressed == 0) isPressed = 1;
}

bool interactiveButtonUX::getSignal()
{
    if (isPressed and !isMousePressed and isMouseHere)
    {
        isPressed = 0;
        return 1;
    }
    else return 0;
}

void interactiveButtonUX::windowResized(sf::Vector2u newSizeWindow)
{
    if (isRight)
    {
        x += newSizeWindow.x - sizeWindow.x;    
        sprite.setPosition(x,y);
    }
    sizeWindow.x = newSizeWindow.x;
    sizeWindow.y = newSizeWindow.y;
}

void interactiveButtonUX::draw()
{
    if (isMouseHere) renderTexture.clear(sf::Color(226,242,229));
    else renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(icon1);
    renderTexture.draw(text);
    renderTexture.display();
    sprite.setTexture(renderTexture.getTexture());
    if (isMousePressed)
    {
        sprite.setScale(ratioPressed, ratioPressed);
        sprite.setPosition(round(x+(1-ratioPressed)*lx/2),round(y+(1-ratioPressed)*ly/2));
    }
    else
    {
        sprite.setScale(1,1);
        sprite.setPosition(round(x),round(y));
    }
    window->draw(sprite);
}


textBox::textBox(sf::RenderTarget *w, mapi* Mi, char c, sf::String t, double x,double y,double lx,double ly, bool isRighti): pushButton(w,Mi,"",x,y,lx,ly,isRighti)
{
    active = 0;
    enabled = 1;
    chirality = c;
    len = t.getSize();
    pos = len;
    
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color(217,217,217));
    rect.setOutlineThickness(1);
    stringText = t;
    textit.setString(stringText);
    textit.setColor(sf::Color::Black);
    textit.setFont(font);
    textit.setCharacterSize(characterSize);
    underline.setSize(sf::Vector2f(1,1.2*characterSize));
    underline.setFillColor(sf::Color::Black);
    
    texture.create(lx,ly);
    sprite.setPosition(x,y);
    view.reset(sf::FloatRect(-5,0,lx,ly));
    texture.setView(view);
    
    threshold = 0.4;
    elapsedTime = threshold + 1;
    stateUnderline = 0;
}


textBox::~textBox(){}

void textBox::enable()
{
    enabled = 1;
}

void textBox::disable()
{
    enabled = 0;
    isMouseHere = isMousePressed = isPressed = active = 0;
}

void textBox::setString(string s)
{
    stringText = sf::String(s);
}

string textBox::getString() const { return stringText.toAnsiString();}

void textBox::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));    
    if (isRight)
    {
        x += newSizeWindow.x - sizeWindow.x;    
        sprite.setPosition(x,y);
        rect.setPosition(x,y);
    }
    sizeWindow.x = newSizeWindow.x;
    sizeWindow.y = newSizeWindow.y;
}

void textBox::setActive(bool s)
{
    isPressed = s;
    active = s;
}

bool textBox::getEnabled() { return enabled;}

void textBox::mousePressed(sf::Vector2i p)
{
    if (testMouse(p) and enabled)
    {
        isMousePressed = 1;
        if (!click)
        {
            click = 1;
            posClick = p;
        }
    }
    else
    {
        isPressed = 0;
        active = 0;
    }
}

void textBox::backSpace()
{
    if (isPressed && !stringText.isEmpty() && pos > 0)
    {
        stringText.erase(pos-1);
        pos -= 1;
        len -= 1;
    }
}

void textBox::enter()
{
    if (isPressed)
    {
        if (chirality == 'L' || chirality == 'R')
            M->setDirPNG(stringText.toAnsiString());
        else if (chirality == 'M')
            M->setFile(stringText.toAnsiString());
        /*
        else if (chirality == 'X')
            M->setLxMap(stringToUnsignedInt(stringText.toAnsiString()));
        else if (chirality == 'Y')
            M->setLyMap(stringToUnsignedInt(stringText.toAnsiString()));
        */
        isPressed = active = 0;
    }
}

void textBox::moveLeft()
{
    if (pos > 0 && isPressed)
        pos -= 1;
}

void textBox::moveRight()
{
    if (pos < len && isPressed)
        pos += 1;
}

void textBox::textEntered(sf::String f)
{
    if (isPressed)
    {
        if (f=="0" || f=="1" || f=="2" || f=="3" || f=="4" || f=="5" || f=="6" || f=="7" || f=="8" || f=="9")
        {
            stringText.insert(pos,f);
            pos += 1;
            len += 1;
        }
        else if ((chirality == 'N') && (f=='A' || f=='B' || f=='C' || f=='D' || f=='E' || f=='F' || f=='G' || f=='H' || f=='I' || f=='J' || f=='K' || f=='L' || f=='M' || f=='N' || f=='O' || f=='P' || f=='Q' || f=='R' || f=='S' || f=='T' || f=='U' || f=='V' || f=='W' || f=='X' || f=='Y' || f=='Z' || f=='a' || f=='b' || f=='c' || f=='d' || f=='e' || f=='f' || f=='g' || f=='h' || f=='i' || f=='j' || f=='k' || f=='l' || f=='m' || f=='n' || f=='o' || f=='p' || f=='q' || f=='r' || f=='s' || f=='t' || f=='u' || f=='v' || f=='w' || f=='x' || f=='y' || f=='z' || f=='_' || f=='-'))
        {
            stringText.insert(pos,f);
            pos += 1;
            len += 1;
        }
    }
}

void textBox::update()
{
    textit.setString(stringText);        
    textit.setColor(sf::Color::Black);
    sf::FloatRect foo = textit.getLocalBounds();
    double sizeString = foo.width;
    double yText = (ly-characterSize)/2;
    double xText;
    sf::Vector2f foo2 = textit.findCharacterPos(pos);
    underline.setPosition(foo2.x,0);
    if (foo2.x > 3.*lx/4)
        xText = 3.*lx/4-foo2.x;
    else
        xText = 0;
    view.reset(sf::FloatRect(round(-5-xText),-yText,lx,ly));
    texture.setView(view);
    if (isPressed && !active)
    {
        active = 1;
        pos = len;
    }
    else if (!isPressed && active)
    {
        active = 0;
    }
}

void textBox::draw()
{
    window->draw(rect);
    if (enabled) texture.clear(sf::Color::White);
    else texture.clear(sf::Color(217,217,217));
    texture.draw(textit);
    texture.draw(underline);
    texture.display();
    sprite.setTexture(texture.getTexture());
    window->draw(sprite);
}

void textBox::draw(double eT)
{
    elapsedTime += eT;
    window->draw(rect);
    if (enabled) texture.clear(sf::Color::White);
    else texture.clear(sf::Color(217,217,217));
    texture.draw(textit);
    if (elapsedTime > threshold)
    {
        stateUnderline = !stateUnderline;
        elapsedTime = 0;
    }
    if (stateUnderline and isPressed) texture.draw(underline);
    texture.display();
    sprite.setTexture(texture.getTexture());
    window->draw(sprite);
    if (elapsedTime > threshold) elapsedTime = 0;
}

wrapMenu::wrapMenu(sf::RenderTarget *w, mapi* Mi, string t, double xi,double yi,double lxi,double lyi, bool isRighti): drawable(w)
{
    yScroll = 30;
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    lyMenu = 0;
    yMenu = y+ly;
    ly0 = ly;
    sprite = new sf::Sprite;
    texture = 0;
    sprite = 0;
    
    nChoice = 0;
    choice = 0;
    heightChoice = ly;
    sizeWrapInWindow = round((int)sizeWindow.y/(2*heightChoice))*heightChoice;
    offsetTexture = 3;
    
    M = Mi;
    isPressed = 0;
    isWrapped = 0;
    rect.setSize(sf::Vector2f(lx,ly));
    rect.setPosition(x,y);
    rect.setFillColor(sf::Color(128,128,128,255));
    rect.setOutlineThickness(3.);
    rect.setOutlineColor(sf::Color(0,0,0,255));
    characterSize = 20;
    text.setString(t);
    font.loadFromFile(fonts);
    text.setFont(font);
    text.setCharacterSize(characterSize);
    
    backGShape.setFillColor(sf::Color(128,128,128,255));
    backGShape.setPosition(0,0);
    backGShape.setSize(sf::Vector2f(lx,lyMenu));
    
    contourShape.setPosition(x,y+ly0+offsetTexture);
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    contourShape.setFillColor(sf::Color::Transparent);
    contourShape.setOutlineColor(sf::Color::Black);
    contourShape.setOutlineThickness(3.);
    
    selectShape.setFillColor(sf::Color(0,0,255,128));
    selectShape.setPosition(x,y);
    selectShape.setSize(sf::Vector2f(lx,heightChoice));
    
    sizeScrollBar = sf::Vector2f(5,10);
    scrollBar.setPosition(lx-sizeScrollBar.x-1,1);
    scrollBar.setSize(sizeScrollBar);
    scrollBar.setFillColor(sf::Color::Blue);
    scrollBar.setOutlineColor(sf::Color::Black);
    scrollBar.setOutlineThickness(1);
    
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(xText, yText);
}

wrapMenu::~wrapMenu()
{
    delete[] choice;
    if (texture != 0) delete texture;
    if (sprite != 0) delete sprite;
}

void wrapMenu::setWrapped(bool s)
{
    isWrapped = s;
    if (s == 1)
    {
        ly = ly0;
        yMenu = y+ly;
        view.reset(sf::FloatRect(0,0,lx,min(lyMenu,sizeWrapInWindow)));
        texture->setView(view);
        isWrapped = 1;
    }
}

void wrapMenu::addChoice(string c)
{
    lyMenu += heightChoice;
    if (texture != 0) delete texture;
    texture = new sf::RenderTexture;
    texture->create(lx,min(lyMenu,sizeWrapInWindow));
    view.reset(sf::FloatRect(0,0,lx,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    sprite->setTexture(texture->getTexture());
    
    sf::Text foo;
    foo.setString(c);
    foo.setFont(font);
    foo.setCharacterSize(characterSize);
    int insertIndex = -1;
    for (int i=0; i<nChoice; i++)
        if (foo.getString()<choice[i].getString() && insertIndex == -1)
            insertIndex = i;
    if (insertIndex == -1) insertIndex = nChoice;
    sf::Text* fooChoice = new sf::Text[nChoice+1];
    fooChoice[insertIndex] = foo;
    for (int i=0; i<nChoice; i++)
    {
        if (i<insertIndex)
            fooChoice[i] = choice[i];
        else
        {
            fooChoice[i+1] = choice[i];
            sf::Vector2f fooPos = choice[i].getPosition();
            fooPos.y += heightChoice;
            fooChoice[i+1].setPosition(fooPos);
        }
    }    
    sf::FloatRect fooRect = fooChoice[insertIndex].getLocalBounds();
    double sizeString = fooRect.width;
    double xText = (lx-(int)sizeString)/2;
    double yText = insertIndex*heightChoice+(heightChoice-characterSize)/2;
    fooChoice[insertIndex].setPosition(xText, yText);
    if (choice != 0) delete[] choice;
    nChoice += 1;
    
    choice = new sf::Text[nChoice];
    for (int i=0; i<nChoice; i++)
        choice[i] = fooChoice[i];
    delete[] fooChoice;    
}

void wrapMenu::deleteChoice(int iChoice)
{
    delete texture;
    texture = 0;
    lyMenu -= heightChoice;
    if (lyMenu > 0)
    {
        texture = new sf::RenderTexture;
        texture->create(lx,min(lyMenu,sizeWrapInWindow));
        view.reset(sf::FloatRect(0,0,lx,min(lyMenu,sizeWrapInWindow)));
        texture->setView(view);
    }
    if (iChoice < nChoice && iChoice >= 0)
    {
        sf::Text* foo = new sf::Text[nChoice-1];
        for (int i=0; i<nChoice; i++)
        {
            if (i < iChoice) foo[i] = choice[i];
            else if (i > iChoice)
            {
                foo[i-1] = choice[i];
                sf::FloatRect fooRect = foo[i-1].getLocalBounds();
                double sizeString = fooRect.width;
                double xText = (lx-sizeString)/2;
                double yText = (i-1)*heightChoice+(heightChoice-characterSize)/2;
                foo[i-1].setPosition(xText, yText); 
            }
        }
        delete[] choice;
        
        nChoice -= 1;
        choice = new sf::Text[nChoice];
        for (int i=0; i<nChoice; i++)
            choice[i] = foo[i];
        delete[] foo;
    }
}

void wrapMenu::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    sizeWrapInWindow = round((int)newSizeWindow.y/(2*heightChoice))*heightChoice;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));
    delete texture;    
    texture = new sf::RenderTexture;
    texture->create(lx,min(lyMenu,sizeWrapInWindow));
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    view.reset(sf::FloatRect(0,0,lx,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    if (isRight)
    {
        x += (int)newSizeWindow.x - sizeWindow.x;
        rect.setPosition(x,y);
        contourShape.setPosition(x,y+ly0+offsetTexture);
        sf::Vector2f foo = text.getPosition();
        foo.x += (int)newSizeWindow.x - (int)sizeWindow.x;
        text.setPosition(foo);
    }
    sizeWindow = newSizeWindow;
}

void wrapMenu::update(){}

void wrapMenu::draw()
{
    window->draw(rect);
    window->draw(text);
    if (not isWrapped)
    {
        texture->clear();
        texture->draw(backGShape);
        int foo = posMouse.y-3-yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        for (int i=0; i<nChoice; i++)
            texture->draw(choice[i]);
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x,y+ly0+offsetTexture);
        window->draw(*sprite);
        window->draw(contourShape);
    }
}

wrapMenuLoad::wrapMenuLoad(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenu(w,Mi,t,x,y,lx,ly,isRighti)
{
    DIR * rep = opendir("./Maps/"); 
  
    if (rep != NULL) 
    { 
        struct dirent * ent; 
  
        while ((ent = readdir(rep)) != NULL) 
        { 
            if (string(ent->d_name) != "." && string(ent->d_name) != ".." && string(ent->d_name) != "dataMap.txt") addChoice(string(ent->d_name));
        } 
  
        closedir(rep); 
    } 
}

wrapMenuLoad::~wrapMenuLoad(){}
    
void wrapMenuLoad::update()
{
    backGShape.setSize(sf::Vector2f(lx,lyMenu));
    view.reset(sf::FloatRect(0,y+ly0-yMenu,lx,min(lyMenu, sizeWrapInWindow)));
    texture->setView(view);
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(ly0+lyMenu, ly0+sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped)
    {
        setWrapped(1);
    }    
    if (isMouseHere && !isWrapped && delta != 0)
    {
        delta = delta*heightChoice;
        int sizeY = min(lyMenu, sizeWrapInWindow);
        if (yMenu+delta>y+ly0)
        {
            delta = (y+ly0-yMenu);
        }
        else if (yMenu+delta+lyMenu<y+ly0+sizeY)
        {
            delta = (-yMenu-lyMenu+y+ly0+sizeY);
        }
        yMenu = yMenu+delta;
        view.reset(sf::FloatRect(0,-yMenu+y+ly0,lx,sizeY));
        texture->setView(view);
            
        posClick = sf::Vector2i(posClick.x, posClick.y+delta);
        delta = 0;        
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y-3 - yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                M->loadMap(choice[foo].getString().toAnsiString());
                setWrapped(1);
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

wrapMenuTexture::wrapMenuTexture(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenu(w,Mi,t,x,y,lx,ly,isRighti)
{
    dirPNG = "./Tileset/";
    DIR * rep = opendir(dirPNG.c_str()); 
  
    if (rep != NULL) 
    { 
        struct dirent * ent; 
        string fooString;
        while ((ent = readdir(rep)) != NULL) 
        { 
            fooString = string(ent->d_name);
            if (fooString.find(".png") != -1) addChoice(fooString);
        } 
        closedir(rep); 
    } 
    else cout<<"Invalid directory !\n";
}

wrapMenuTexture::~wrapMenuTexture(){}
    
void wrapMenuTexture::update()
{
    if (M->getDirPNG() != dirPNG)
    {
        dirPNG = M->getDirPNG();
        if (choice != 0)
        {
            delete[] choice;
            choice = 0;
        }
        nChoice = 0;
        lyMenu = 0;
        ly = ly0;
        DIR * rep = opendir(dirPNG.c_str()); 
        if (rep != NULL) 
        { 
            struct dirent * ent; 
            string fooString;
            while ((ent = readdir(rep)) != NULL) 
            { 
                fooString = string(ent->d_name);
                if (fooString.find(".png") != -1) addChoice(fooString);
            } 
            closedir(rep); 
        } 
        else cout<<"Invalid directory !\n";
        setWrapped(1);
    }
    if (nChoice > 0)
    {
        backGShape.setSize(sf::Vector2f(lx,lyMenu));
        view.reset(sf::FloatRect(0,y+ly0-yMenu,lx,min(lyMenu, sizeWrapInWindow)));
        texture->setView(view);
        if (isMouseHere and isWrapped)
        {
            lyMenu = nChoice*heightChoice;
            ly = min(ly0+lyMenu, ly0+sizeWrapInWindow);
            isWrapped = 0;
        }
        else if (!isMouseHere and not isWrapped)
        {
            M->deletePNG('R');
            setWrapped(1);
        }    
        if (isMouseHere && !isWrapped && delta != 0)
        {
            delta = delta*heightChoice;
            int sizeY = min(lyMenu, sizeWrapInWindow);
            if (yMenu+delta>y+ly0)
            {
                delta = (y+ly0-yMenu);
            }
            else if (yMenu+delta+lyMenu<y+ly0+sizeY+offsetTexture)
            {
                delta = (-yMenu-lyMenu+y+ly0+sizeY+offsetTexture);
            }
            yMenu = yMenu+delta;
            view.reset(sf::FloatRect(0,-yMenu+y+ly0,lx,sizeY));
            texture->setView(view);
            
            posClick = sf::Vector2i(posClick.x, posClick.y+delta);
            delta = 0;        
        }
        if (!isMousePressed && isMouseHere)
        {
            int foo = posMouse.y-3 - yMenu;
            if (foo >= 0)
            {
                foo = foo/heightChoice;
                if (foo < nChoice)
                {
                    M->initPNG((sf::String(dirPNG)+choice[foo].getString()).toAnsiString(),'R');
                }
            }
        }
        if (isPressed && isMouseHere && !isMousePressed)
        {
            isPressed = 0;
            int foo = posMouse.y - yMenu;
            if (foo >= 0)
            {
                foo = foo/heightChoice;
                if (foo < nChoice)
                {
                    M->initPNG((sf::String(dirPNG)+choice[foo].getString()).toAnsiString(),'L');
                    M->deletePNG('R');
                    setWrapped(1);
                }
            }
        }
        if(isMouseHere && isMousePressed)
            isPressed = 1;
    }
}

wrapMenuUX::wrapMenuUX(sf::RenderTarget *w, mapi* Mi, string t, double xi,double yi,double lxi,double lyi, bool isRighti): drawable(w)
{
    yScroll = 30;
    isRight = isRighti;
    x=xi; y=yi; lx=lxi; ly=lyi;
    lyMenu = 0;
    yMenu = y+ly;
    ly0 = ly;
    sprite = new sf::Sprite;
    texture = 0;
    sprite = 0;
    
    nChoice = 0;
    choice = 0;
    heightChoice = ly;
    sizeWrapInWindow = round((int)sizeWindow.y/(2*heightChoice))*heightChoice;
    offsetTexture = 0;
    maxSizeChoice = 0;
    
    M = Mi;
    isMouseHere = 0;
    isPressed = 0;
    isWrapped = 0;
    rect.setSize(sf::Vector2f(lx,ly));
    rect.setPosition(x,y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::Transparent);
    characterSize = 20;
    text.setString(t);
    font.loadFromFile(fonts);
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setColor(sf::Color(84,106,139));
    
    contourShape.setPosition(x,y+ly0+offsetTexture);
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    contourShape.setFillColor(sf::Color::Transparent);
    contourShape.setOutlineColor(sf::Color(232,232,232));
    contourShape.setOutlineThickness(1.);
    
    selectShape.setFillColor(sf::Color(84,106,121));
    selectShape.setPosition(x,y);
    selectShape.setSize(sf::Vector2f(lx,heightChoice));
    
    sizeScrollBar = sf::Vector2f(5,10);
    scrollBar.setPosition(lx-sizeScrollBar.x-1,1);
    scrollBar.setSize(sizeScrollBar);
    scrollBar.setFillColor(sf::Color(146,161,176));
    
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(round(xText), round(yText));
}

wrapMenuUX::~wrapMenuUX()
{
    delete[] choice;
    if (texture != 0) delete texture;
    if (sprite != 0) delete sprite;
}

string wrapMenuUX::getName()
{
    return text.getString().toAnsiString();
}

int wrapMenuUX::testMouse(sf::Vector2i v)
{
    int xMouse = v.x;
    int yMouse = v.y;
    posMouse = sf::Vector2i(xMouse,yMouse);
    
    if ((xMouse>x && xMouse<x+lx && yMouse>y && yMouse<=y+ly0) || (!isWrapped && (xMouse>x && xMouse<x+max((double)lx, maxSizeChoice*4./3+30) && yMouse>=y+ly0 && yMouse<y+ly0+min(lyMenu,sizeWrapInWindow))))
        isMouseHere = 1;
    else
        isMouseHere = 0;
    
    return isMouseHere;
}

bool wrapMenuUX::getIsMouseHere() const { return isMouseHere;}

void wrapMenuUX::setWrapped(bool s)
{
    isWrapped = s;
    if (s == 1)
    {
        ly = ly0;
        yMenu = y+ly;
        view.reset(sf::FloatRect(0,0,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow)));
        texture->setView(view);
        isWrapped = 1;
    }
}

bool wrapMenuUX::getWrapped() { return isWrapped;}

void wrapMenuUX::addChoice(string c)
{
    lyMenu += heightChoice;
    
    sf::Text foo;
    foo.setString(c);
    foo.setFont(font);
    foo.setCharacterSize(characterSize);
    
    sf::Text* fooChoice = new sf::Text[nChoice+1];
    fooChoice[nChoice] = foo;
    for (int i=0; i<nChoice; i++)
        fooChoice[i] = choice[i];
    sf::FloatRect fooRect = fooChoice[nChoice].getLocalBounds();
    if (fooRect.width > maxSizeChoice) maxSizeChoice = fooRect.width;
    double sizeString = fooRect.width;
    double xText = 20;
    double yText = nChoice*heightChoice+(heightChoice-characterSize)/2;
    fooChoice[nChoice].setPosition(xText, yText);
    if (choice != 0) delete[] choice;
    nChoice += 1;
    
    choice = new sf::Text[nChoice];
    for (int i=0; i<nChoice; i++)
        choice[i] = fooChoice[i];
    delete[] fooChoice;    
    
    if (texture != 0) delete texture;
    texture = new sf::RenderTexture;
    texture->create(max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow));
    view.reset(sf::FloatRect(0,0,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow)));
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = max((double)lx, maxSizeChoice*4./3+30)-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    sprite->setTexture(texture->getTexture());
}

void wrapMenuUX::deleteChoice(int iChoice)
{
    delete texture;
    texture = 0;
    lyMenu -= heightChoice;
    if (lyMenu > 0)
    {
        texture = new sf::RenderTexture;
        texture->create(lx,min(lyMenu,sizeWrapInWindow));
        view.reset(sf::FloatRect(0,0,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow)));
        texture->setView(view);
    }
    if (iChoice < nChoice && iChoice >= 0)
    {
        sf::Text* foo = new sf::Text[nChoice-1];
        for (int i=0; i<nChoice; i++)
        {
            if (i < iChoice) foo[i] = choice[i];
            else if (i > iChoice)
            {
                foo[i-1] = choice[i];
                sf::FloatRect fooRect = foo[i-1].getLocalBounds();
                double sizeString = fooRect.width;
                double xText = (lx-sizeString)/2;
                double yText = (i-1)*heightChoice+(heightChoice-characterSize)/2;
                foo[i-1].setPosition(xText, yText); 
            }
        }
        delete[] choice;
        
        nChoice -= 1;
        choice = new sf::Text[nChoice];
        for (int i=0; i<nChoice; i++)
            choice[i] = foo[i];
        delete[] foo;
    }
}

void wrapMenuUX::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    sizeWrapInWindow = round((int)newSizeWindow.y/(2*heightChoice))*heightChoice;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));
    delete texture;    
    texture = new sf::RenderTexture;
    texture->create(max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow));
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    view.reset(sf::FloatRect(0,0,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    if (isRight)
    {
        x += (int)newSizeWindow.x - sizeWindow.x;
        rect.setPosition(x,y);
        contourShape.setPosition(x,y+ly0+offsetTexture);
        sf::Vector2f foo = text.getPosition();
        foo.x += (int)newSizeWindow.x - (int)sizeWindow.x;
        text.setPosition(foo);
    }
    sizeWindow = newSizeWindow;
}

void wrapMenuUX::update()
{
    if (isMousePressed and !isMouseHere) isMousePressed = 0;
    selectShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),heightChoice));
    contourShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),heightChoice*nChoice));
    if (isMouseHere)
    {
        rect.setFillColor(sf::Color(232,232,232));
        rect.setOutlineColor(sf::Color(232,232,232));
    }
    else
    {
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Transparent);
    }
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(ly0+lyMenu, ly0+sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped)
    {
        setWrapped(1);
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y- yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                setWrapped(1);
                text = choice[foo];
                text.setColor(sf::Color(84,106,139));
                sf::FloatRect foo = text.getLocalBounds();
                double sizeString = foo.width;
                double xText = x+(lx-sizeString)/2;
                double yText = y+(ly-characterSize)/2;
                text.setPosition(round(xText), round(yText));
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

void wrapMenuUX::draw()
{
    window->draw(rect);
    window->draw(text);
    if (not isWrapped)
    {
        texture->clear(sf::Color::White);
        int foo = posMouse.y-yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        for (int i=0; i<nChoice; i++)
        {
            if (i == foo) choice[i].setColor(sf::Color::White);
            else choice[i].setColor(sf::Color(132,148,159));
            texture->draw(choice[i]);
        }
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x,y+ly0+offsetTexture);
        window->draw(*sprite);
        window->draw(contourShape);
    }
}


wrapMenuFile::wrapMenuFile(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenuUX(w,Mi,t,x,y,lx,ly,isRighti)
{
    addChoice("New map");
    addChoice("Load map");
    addChoice("Save");
    mLoad = new wrapMenuSideLoad(w,Mi,"",x,y+ly+heightChoice,round(max((double)lx, maxSizeChoice*4./3+30)),ly,isRighti);
    //addChoice("Save as");
}

wrapMenuFile::~wrapMenuFile()
{
    delete mLoad;
}

int wrapMenuFile::testMouse(sf::Vector2i v)
{
    int xMouse = v.x;
    int yMouse = v.y;
    posMouse = sf::Vector2i(xMouse,yMouse);
    if (isMouseHere or not mLoad->getWrapped()) mLoad->testMouse(v);
    else mLoad->setWrapped(1);
    
    if ((xMouse>x && xMouse<x+lx && yMouse>y && yMouse<=y+ly0) || (!isWrapped && (xMouse>x && xMouse<x+max((double)lx, maxSizeChoice*4./3+30) && yMouse>=y+ly0 && yMouse<y+ly0+min(lyMenu,sizeWrapInWindow))))
        isMouseHere = 1;
    else
        isMouseHere = 0;
        
    return isMouseHere;
}

bool wrapMenuFile::getIsMouseHere() const
{
    return (isMouseHere or mLoad->getIsMouseHere());
}

void wrapMenuFile::mousePressed(sf::Vector2i posMouse)
{
    if (isMouseHere)
    {
        isMousePressed = 1;
        if (!click)
        {
            click = 1;
            posClick = posMouse;
        }
    }
    else if (mLoad->getIsMouseHere()) mLoad->mousePressed(posMouse);
}

void wrapMenuFile::mouseReleased()
{
    isMousePressed = 0;
    click = 0;
    if (mLoad->getIsMouseHere()) mLoad->mouseReleased();
}

void wrapMenuFile::update()
{
    mLoad->update();
    selectShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),heightChoice));
    contourShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),nChoice*heightChoice));
    if (isMouseHere or mLoad->getIsMouseHere())
    {
        rect.setFillColor(sf::Color(232,232,232));
        rect.setOutlineColor(sf::Color(232,232,232));
    }
    else
    {
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Transparent);
    }
    view.reset(sf::FloatRect(0,y+ly0-yMenu,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu, sizeWrapInWindow)));
    texture->setView(view);
    
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(ly0+lyMenu, ly0+sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped and mLoad->getWrapped())
    {
        setWrapped(1);
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y- yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                setWrapped(1);
                if (choice[foo].getString().toAnsiString() == "New map") M->newMap();
                else if (choice[foo].getString().toAnsiString() == "Save") M->saveMap();
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

void wrapMenuFile::draw()
{
    window->draw(rect);
    window->draw(text);
    if (not isWrapped)
    {
        texture->clear(sf::Color::White);
        int foo = posMouse.y-yMenu;
        if (foo >= 0 && mLoad->getWrapped())
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        else if (not mLoad->getWrapped())
        {
            foo = 1;
            selectShape.setPosition(0,foo*heightChoice);
            texture->draw(selectShape);
        }
        for (int i=0; i<nChoice; i++)
        {
            if (i == foo) choice[i].setColor(sf::Color::White);
            else choice[i].setColor(sf::Color(132,148,159));
            texture->draw(choice[i]);
        }
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x,y+ly0+offsetTexture);
        window->draw(*sprite);
        window->draw(contourShape);
        mLoad->draw();
    }
}
wrapMenuEdit::wrapMenuEdit(sf::RenderWindow *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenuUX(w,Mi,t,x,y,lx,ly,isRighti)
{
    windowFIX = w;
    addChoice("Change size map");
    addChoice("Add event");
    addChoice("Delete event");
}

wrapMenuEdit::~wrapMenuEdit(){}

void wrapMenuEdit::update()
{
    selectShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),heightChoice));
    contourShape.setSize(sf::Vector2f(max((double)lx, maxSizeChoice*4./3+30),nChoice*heightChoice));
    if (isMouseHere)
    {
        rect.setFillColor(sf::Color(232,232,232));
        rect.setOutlineColor(sf::Color(232,232,232));
    }
    else
    {
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Transparent);
    }
    view.reset(sf::FloatRect(0,y+ly0-yMenu,max((double)lx, maxSizeChoice*4./3+30),min(lyMenu, sizeWrapInWindow)));
    texture->setView(view);
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(ly0+lyMenu, ly0+sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped)
    {
        setWrapped(1);
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y- yMenu;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                setWrapped(1);
                if (choice[foo].getString().toAnsiString() == "Change size map")
                {
                    vector<int> foo(2);
                    foo = changeSizeMapWindow(windowFIX, M);
                    M->setLxMap(foo[0]);
                    M->setLyMap(foo[1]);
                }
                else if (choice[foo].getString().toAnsiString() == "Add event")
                {
                    M->addEvent();
                }
                else if (choice[foo].getString().toAnsiString() == "Delete event")
                {
                    M->deleteEvent();
                }
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

wrapMenuSide::wrapMenuSide(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenuUX(w,Mi,t,x,y,lx,ly,isRighti)
{
    lxMenu = 0;
    contourShape.setPosition(x+lx, y);
    isWrapped = 1;
    isMouseHere = 0;
    
    textureArrow.loadFromFile("Icons/arrow-right-blue.png");
    sf::Vector2u fooSize = textureArrow.getSize();
    spriteArrow.setTexture(textureArrow);
    ratio = 0.5;
    spriteArrow.setScale(ratio*ly/fooSize.y,ratio*ly/fooSize.y);
    spriteArrow.setPosition(x+lx-ratio*ly-3,y+(1-ratio)*ly/2);
}

wrapMenuSide::~wrapMenuSide(){}

int wrapMenuSide::testMouse(sf::Vector2i v)
{
    int xMouse = v.x;
    int yMouse = v.y;
    posMouse = sf::Vector2i(xMouse,yMouse);
    
    if ((xMouse>x && xMouse<=x+lx && yMouse>y && yMouse<=y+ly0) || (!isWrapped && (xMouse>=x+lx && xMouse<x+lx+lxMenu && yMouse>=y && yMouse<y+min(lyMenu,sizeWrapInWindow))))
        isMouseHere = 1;
    else
        isMouseHere = 0;
        
    return isMouseHere;
}

void wrapMenuSide::setWrapped(bool s)
{
    isWrapped = s;
    if (s == 1)
    {
        ly = ly0;
        yMenu = y+ly;
        view.reset(sf::FloatRect(0,0,lxMenu,min(lyMenu,sizeWrapInWindow)));
        texture->setView(view);
        isWrapped = 1;
    }
}

void wrapMenuSide::addChoice(string c)
{
    lyMenu += heightChoice;
    
    sf::Text foo;
    foo.setString(c);
    foo.setFont(font);
    foo.setCharacterSize(characterSize);
    
    sf::Text* fooChoice = new sf::Text[nChoice+1];
    fooChoice[nChoice] = foo;
    for (int i=0; i<nChoice; i++)
        fooChoice[i] = choice[i];
    sf::FloatRect fooRect = fooChoice[nChoice].getLocalBounds();
    if (fooRect.width > maxSizeChoice) maxSizeChoice = fooRect.width;
    lxMenu = maxSizeChoice*4./3+30;
    double sizeString = fooRect.width;
    double xText = 20;
    double yText = nChoice*heightChoice+(heightChoice-characterSize)/2;
    fooChoice[nChoice].setPosition(xText, yText);
    if (choice != 0) delete[] choice;
    nChoice += 1;
    
    choice = new sf::Text[nChoice];
    for (int i=0; i<nChoice; i++)
        choice[i] = fooChoice[i];
    delete[] fooChoice;    
    
    if (texture != 0) delete texture;
    texture = new sf::RenderTexture;
    texture->create(lxMenu,min(lyMenu,sizeWrapInWindow));
    view.reset(sf::FloatRect(0,0,lxMenu,min(lyMenu,sizeWrapInWindow)));
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = lxMenu-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    sprite->setTexture(texture->getTexture());
}

void wrapMenuSide::windowResized(sf::Vector2u newSizeWindow)
{
    double rx = newSizeWindow.x * 1./sizeWindow.x;
    double ry = newSizeWindow.y * 1./sizeWindow.y;
    sizeWrapInWindow = round((int)newSizeWindow.y/(2*heightChoice))*heightChoice;
    rect.setSize(sf::Vector2f(lx/rx,ly/ry));
    delete texture;    
    texture = new sf::RenderTexture;
    texture->create(lxMenu,min(lyMenu,sizeWrapInWindow));
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    contourShape.setSize(sf::Vector2f(lxMenu,min(lyMenu,sizeWrapInWindow)));
    view.reset(sf::FloatRect(0,0,lxMenu,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    if (isRight)
    {
        x += (int)newSizeWindow.x - sizeWindow.x;
        contourShape.setPosition(x+lx,y+offsetTexture);
        sf::Vector2f foo = text.getPosition();
        foo.x += (int)newSizeWindow.x - (int)sizeWindow.x;
    }
    sizeWindow = newSizeWindow;
}

void wrapMenuSide::update()
{
    selectShape.setSize(sf::Vector2f(lxMenu,heightChoice));
    contourShape.setSize(sf::Vector2f(lxMenu,heightChoice*nChoice));
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(lyMenu, sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped)
    {
        setWrapped(1);
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y - y;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                M->loadMap(choice[foo].getString().toAnsiString());
                setWrapped(1);
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

void wrapMenuSide::draw()
{
    if (not isWrapped)
    {
        texture->clear(sf::Color::White);
        int foo = posMouse.y-y;
        if (foo >= 0 and (posMouse.x>=x+lx && posMouse.x<x+lx+lxMenu && posMouse.y>=y && posMouse.y<y+lyMenu))
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        else foo = -1;
        for (int i=0; i<nChoice; i++)
        {
            if (i == foo) choice[i].setColor(sf::Color::White);
            else choice[i].setColor(sf::Color(132,148,159));
            texture->draw(choice[i]);
        }
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x+lx,y);
        window->draw(*sprite);
        window->draw(contourShape);
    }
    window->draw(spriteArrow);
}

wrapMenuTextureUX::wrapMenuTextureUX(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti, int modei): wrapMenuSide(w,Mi,t,x,y,lx,ly,isRighti)
{
    nTexture = M->getNTextures();
    fileMap = M->getFile();
    dirPNG = "./Tileset/";
    if (modei == 0)
    {
        mode = 0;
        text.setString(sf::String("Load texture from file"));
        DIR * rep = opendir(dirPNG.c_str()); 
      
        if (rep != NULL) 
        { 
            struct dirent * ent; 
            string fooString;
            while ((ent = readdir(rep)) != NULL) 
            { 
                fooString = string(ent->d_name);
                if (fooString.find(".png") != -1) addChoice(fooString);
            } 
            closedir(rep); 
        } 
        else cout<<"Invalid directory !\n";
    }
    else
    {
        mode = 1;
        text.setString(sf::String("Load texture from map"));
        string* fooString = M->getFileTextures();
        for (int i=0; i<M->getNTextures(); i++)
        {
            int foo = fooString[i].find('/');
            fooString[i].erase(0,foo+1);
            addChoice(fooString[i]);
        }
        delete[] fooString;
    }
    sf::FloatRect foo = text.getLocalBounds();
    double sizeString = foo.width;
    double xText = x+(lx-sizeString)/2;
    double yText = y+(ly-characterSize)/2;
    text.setPosition(round(xText), round(yText));
}

wrapMenuTextureUX::~wrapMenuTextureUX(){};

void wrapMenuTextureUX::addChoice(string c)
{
    lyMenu += heightChoice;
    int fooInt = c.find("Tileset/");
    if (fooInt != -1) c.erase(0,fooInt+8);
    sf::Text foo;
    foo.setString(c);
    foo.setFont(font);
    foo.setCharacterSize(characterSize);
    int insertIndex = -1;
    for (int i=0; i<nChoice; i++)
        if (foo.getString()<choice[i].getString() && insertIndex == -1)
            insertIndex = i;
    if (insertIndex == -1) insertIndex = nChoice;
    sf::Text* fooChoice = new sf::Text[nChoice+1];
    fooChoice[insertIndex] = foo;
    for (int i=0; i<nChoice; i++)
    {
        if (i<insertIndex) fooChoice[i] = choice[i];
        else
        {
            fooChoice[i+1] = choice[i];
            sf::Vector2f fooPos = choice[i].getPosition();
            fooPos.y += heightChoice;
            fooChoice[i+1].setPosition(fooPos);
        }
    }
    sf::FloatRect fooRect = fooChoice[insertIndex].getLocalBounds();
    if (fooRect.width > maxSizeChoice) maxSizeChoice = fooRect.width;
    lxMenu = maxSizeChoice*4./3+30;
    double sizeString = fooRect.width;
    double xText = 20;
    double yText = insertIndex*heightChoice+(heightChoice-characterSize)/2;
    fooChoice[insertIndex].setPosition(xText, yText);
    if (choice != 0) delete[] choice;
    nChoice += 1;
    
    choice = new sf::Text[nChoice];
    for (int i=0; i<nChoice; i++)
        choice[i] = fooChoice[i];
    delete[] fooChoice;    
    
    if (texture != 0) delete texture;
    texture = new sf::RenderTexture;
    texture->create(lxMenu,min(lyMenu,sizeWrapInWindow));
    view.reset(sf::FloatRect(0,0,lxMenu,min(lyMenu,sizeWrapInWindow)));
    contourShape.setSize(sf::Vector2f(lx,min(lyMenu,sizeWrapInWindow)));
    texture->setView(view);
    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = lxMenu-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    if (sprite != 0) delete sprite;
    sprite = new sf::Sprite;
    sprite->setTexture(texture->getTexture());
}

void wrapMenuTextureUX::update()
{
    sf::Vector2i fooSizeIm = M->getSizeIm();
    int fooThickness = M->getThicknessIm();
    if (mode == 1 && (nTexture != M->getNTextures() or fileMap != M->getFile()))
    {
        while(nChoice > 0) deleteChoice(0);
        string* fooString = M->getFileTextures();
        for (int i=0; i<M->getNTextures(); i++)
        {
            int foo = fooString[i].find('/');
            if (foo != -1)
            {
                fooString[i].erase(0,foo+1);
                int foo = fooString[i].find('/');
                if (foo != -1) fooString[i].erase(0,foo+1);
            }
            addChoice(fooString[i]);
        }
        delete[] fooString;
        nTexture = M->getNTextures();
        fileMap = M->getFile();
    }
    if (lx != fooSizeIm.x+2*fooThickness)
    {
        lx = fooSizeIm.x+2*fooThickness;
        rect.setSize(sf::Vector2f(lx,ly));
        contourShape.setPosition(x+lx, y);
        contourShape.setSize(sf::Vector2f(lxMenu,min(lyMenu,sizeWrapInWindow)));
        selectShape.setSize(sf::Vector2f(lxMenu,heightChoice));
        scrollBar.setSize(sizeScrollBar);
        
        sf::FloatRect foo = text.getLocalBounds();
        double sizeString = foo.width;
        double xText = x+(lx-sizeString)/2;
        double yText = y+(ly-characterSize)/2;
        text.setPosition(xText, yText);
        spriteArrow.setPosition(x+lx-ratio*ly-3,y+(1-ratio)*ly/2);
    }
    selectShape.setSize(sf::Vector2f(lxMenu,heightChoice));
    contourShape.setSize(sf::Vector2f(lxMenu,min(lyMenu,sizeWrapInWindow)));
    if (isMouseHere)
    {
        rect.setFillColor(sf::Color(232,232,232));
        rect.setOutlineColor(sf::Color(232,232,232));
    }
    else
    {
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Transparent);
    }
    if (isMouseHere and isWrapped)
    {
        lyMenu = nChoice*heightChoice;
        ly = min(lyMenu, sizeWrapInWindow);
        isWrapped = 0;
    }
    else if (!isMouseHere and not isWrapped)
    {
        M->deletePNG('R');
        setWrapped(1);
    }
    if (!isMousePressed and isMouseHere)
    {
        int foo = posMouse.y - yMenu+ly0;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                M->initPNG((sf::String(dirPNG)+choice[foo].getString()).toAnsiString(),'R');
            }
        }
    }
    if (isMouseHere && !isWrapped && delta != 0)
    {
        delta = delta*heightChoice;
        int sizeY = min(lyMenu, sizeWrapInWindow);
        if (yMenu+delta>y+ly0)
        {
            delta = (y+ly0-yMenu);
        }
        else if (yMenu+delta+lyMenu<y+ly0+sizeY)
        {
            delta = (-yMenu-lyMenu+y+ly0+sizeY);
        }
        yMenu = yMenu+delta;
        view.reset(sf::FloatRect(0,-yMenu+y+ly0,lxMenu,sizeY));
        texture->setView(view);
        
        posClick = sf::Vector2i(posClick.x, posClick.y+delta);
        delta = 0;        
    }
    if (isPressed && isMouseHere && !isMousePressed)
    {
        isPressed = 0;
        int foo = posMouse.y - yMenu+ly0;
        if (foo >= 0)
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                M->deletePNG('R');
                M->initPNG("./Tileset/"+choice[foo].getString().toAnsiString(),'L');
                setWrapped(1);
            }
        }
    }
    if(isMouseHere && isMousePressed)
        isPressed = 1;
}

void wrapMenuTextureUX::draw()
{
    if (isWrapped)
    {
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color(232,232,232));
        window->draw(rect);
        window->draw(text);
        window->draw(spriteArrow);
    }
    if (not isWrapped)
    {
        rect.setFillColor(sf::Color(232,232,232));
        rect.setOutlineColor(sf::Color(232,232,232));
        window->draw(rect);
        window->draw(text);
        window->draw(spriteArrow);
        texture->clear(sf::Color::White);
        int foo = posMouse.y-yMenu+ly0;
        if (foo >= 0 and (posMouse.x>=x+lx && posMouse.x<x+lx+lxMenu && posMouse.y>=y && posMouse.y<y+lyMenu))
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        else foo = -1;
        for (int i=0; i<nChoice; i++)
        {
            if (i == foo) choice[i].setColor(sf::Color::White);
            else choice[i].setColor(sf::Color(132,148,159));
            texture->draw(choice[i]);
        }
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x+lx,y);
        window->draw(*sprite);
        window->draw(contourShape);
    }
}

wrapMenuSideLoad::wrapMenuSideLoad(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti): wrapMenuSide(w,Mi,t,x,y,lx,ly,isRighti)
{
    isMouseHere = 0;
    DIR * rep = opendir("./Maps/"); 
  
    if (rep != NULL) 
    { 
        struct dirent * ent; 
  
        while ((ent = readdir(rep)) != NULL) 
        { 
            if (string(ent->d_name) != "." && string(ent->d_name) != ".." && string(ent->d_name) != "dataMap.txt") addChoice(string(ent->d_name));
        } 
  
        closedir(rep); 
    }
    ratio = 0.5;
    spriteArrow.setPosition(x+lx-ratio*ly-3,y+(1-ratio)*ly/2);
    
    textureArrow2.loadFromFile("Icons/arrow-right-white.png");
    sf::Vector2u fooSize = textureArrow2.getSize();
    spriteArrow2.setTexture(textureArrow2);
    spriteArrow2.setScale(ratio*ly/fooSize.y,ratio*ly/fooSize.y);
    spriteArrow2.setPosition(x+lx-ratio*ly-3,y+(1-ratio)*ly/2);
}

wrapMenuSideLoad::~wrapMenuSideLoad(){}

void wrapMenuSideLoad::draw()
{
    if (not isWrapped)
    {
        texture->clear(sf::Color::White);
        int foo = posMouse.y-y;
        if (foo >= 0 and (posMouse.x>=x+lx && posMouse.x<x+lx+lxMenu && posMouse.y>=y && posMouse.y<y+lyMenu))
        {
            foo = foo/heightChoice;
            if (foo < nChoice)
            {
                selectShape.setPosition(0,foo*heightChoice);
                texture->draw(selectShape);
            }
        }
        else foo = -1;
        for (int i=0; i<nChoice; i++)
        {
            if (i == foo) choice[i].setColor(sf::Color::White);
            else choice[i].setColor(sf::Color(132,148,159));
            texture->draw(choice[i]);
        }
        if (lyMenu > sizeWrapInWindow)
        {
            sizeScrollBar.y = sizeWrapInWindow*sizeWrapInWindow/lyMenu;
            scrollBar.setSize(sizeScrollBar);
            sf::Vector2f fooPos = scrollBar.getPosition();
            fooPos.y = (sizeWrapInWindow-sizeScrollBar.y)*(y+ly0-yMenu)/(lyMenu-sizeWrapInWindow) + (y+ly0-yMenu);
            scrollBar.setPosition(fooPos);
            texture->draw(scrollBar);
        }   
        texture->display();
        sprite->setTexture(texture->getTexture());
        sprite->setPosition(x+lx,y);
        window->draw(*sprite);
        window->draw(contourShape);
    }
    if (!isMouseHere) window->draw(spriteArrow);
    else window->draw(spriteArrow2);
}
