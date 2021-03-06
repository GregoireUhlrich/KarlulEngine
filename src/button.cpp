#include "../include/button.h"
#include <cmath>
#include <iostream>
#include <string>
#include <dirent.h>

std::string fonts = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

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
	double yText = y+(ly-characterSize)/2;
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

signalButton::signalButton(sf::RenderTarget *w, string t, double x,double y,double lx,double ly,bool isRighti): button(w,t,x,y,lx,ly,isRighti)
{
    rect.setFillColor(sf::Color::White);
    text.setColor(sf::Color::Black);
}

signalButton::~signalButton(){};

bool signalButton::updateSignal()
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

textBox::textBox(sf::RenderTarget *w, mapi* Mi, char c, sf::String t, double x,double y,double lx,double ly, bool isRighti): pushButton(w,Mi,"",x,y,lx,ly,isRighti)
{
	active = 0;
	chirality = c;
	len = t.getSize();
	pos = len;
	
	rect.setFillColor(sf::Color::White);
	rect.setOutlineColor(sf::Color::Black);
	stringText = t;
	textit.setString(stringText);
	textit.setColor(sf::Color::Black);
	textit.setFont(font);
	textit.setCharacterSize(characterSize);
	underline.setSize(sf::Vector2f(characterSize*2./3,5));
	underline.setFillColor(sf::Color::Black);
	
	texture.create(lx,ly);
	sprite.setPosition(x,y);
	view.reset(sf::FloatRect(-5,0,lx,ly));
	texture.setView(view);
}


textBox::~textBox(){}

void textBox::setString(string s)
{
    stringText = sf::String(s);
}

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

void textBox::mousePressed(sf::Vector2i p)
{
	if (testMouse(p))
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
		{
			M->setFile(stringText.toAnsiString());
			M->loadMap();
		}
		else if (chirality == 'X')
			M->setLxMap(stringToUnsignedInt(stringText.toAnsiString()));
		else if (chirality == 'Y')
			M->setLyMap(stringToUnsignedInt(stringText.toAnsiString()));
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
		if ((chirality != 'X' && chirality != 'Y' ) || f=="0" || f=="1" || f=="2" || f=="3" || f=="4" || f=="5" || f=="6" || f=="7" || f=="8" || f=="9")
		{
			stringText.insert(pos,f);
			pos += 1;
			len += 1;
		}
	}
}

void textBox::update()
{
	if (isPressed) rect.setOutlineColor(sf::Color(0,0,255,255));
	else
	{
	    rect.setOutlineColor(sf::Color(0,0,0,255));
	    sf::Vector2u fooVec = M->getSizeMap();
	    if (chirality == 'X')
	    {
	        stringText = sf::String(unsignedIntToString(fooVec.x));
	        len = stringText.getSize();
	        pos = len;
	    }
	    else if (chirality == 'Y')
	    {
	        stringText = sf::String(unsignedIntToString(fooVec.y));
	        len = stringText.getSize();
	        pos = len;
	    }
	    else if (chirality == 'M')
	    {
	        stringText = sf::String(M->getFile());
	        len = stringText.getSize();
	        pos = len;
	    }
	    else if (chirality == 'L' || chirality == 'R')
	    {
			stringText = M->getDirPNG();
	        len = stringText.getSize();
	        pos = len;
	    }
	}       
	textit.setString(stringText);		
	textit.setColor(sf::Color::Black);
	sf::FloatRect foo = textit.getLocalBounds();
	double sizeString = foo.width;
	double yText = (ly-characterSize)/2;
	double xText;
	sf::Vector2f foo2 = textit.findCharacterPos(pos);
	underline.setPosition(foo2.x,characterSize);
	if (foo2.x > 3.*lx/4)
		xText = 3.*lx/4-foo2.x;
	else
		xText = 0;
	view.reset(sf::FloatRect(-5-xText,-yText,lx,ly));
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
	texture.clear(sf::Color::White);
	texture.draw(textit);
	texture.draw(underline);
	texture.display();
	sprite.setTexture(texture.getTexture());
	window->draw(sprite);
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
	            M->setFile(choice[foo].getString().toAnsiString());
	            M->loadMap();
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
