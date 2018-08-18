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

imagePNG::imagePNG(sf::RenderTarget* w, string f, char p, int y0i): drawable(w)
{
	yScroll = 30;
	xSprites = ySprites = 32;
	select = 0, loadedSprites = 1;
	y0 = y0i;
	takeMidX = takeMidY = 0;
	
	nxSprites = 0;
	nySprites = 0;
	spriteToPull = 0;
	
	file = f;
	texture.loadFromFile(file);
	sprite.setTexture(texture);
	sf::Vector2u v = texture.getSize();
	lx = v.x;
	ly = v.y;
	
	pos = p;
	if (pos == 'R')
		x = sizeWindow.x - lx;
	else if (pos == 'L')
		x = 0;
	else 
	{
		cout<<"Invalid position!\n";
		x = sizeWindow.x - lx;
		pos = 'R';
	}
		
	y = y0;
	sprite.setPosition(x,y);
	backGShape.setFillColor(sf::Color::White);
	backGShape.setPosition(x,y);
	backGShape.setSize(sf::Vector2f(lx,max(ly,(int)sizeWindow.y)));
	selectRect.setFillColor(sf::Color::Transparent);
	selectRect.setOutlineColor(sf::Color::Blue);
	selectRect.setOutlineThickness(3.);
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
	if (posMouse.x > x && posMouse.x < x+lx && posMouse.y > y0 && posMouse.y < sizeWindow.y)
		isMouseHere = 1;
	else
		isMouseHere = 0;
	
	return isMouseHere;
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

void imagePNG::update()
{
	if (pos == 'R' && sizeWindow.x - lx != x)
	{
		sf::Vector2f v = selectRect.getPosition();
		int xNewRect = v.x - x + sizeWindow.x - lx;
		selectRect.setPosition(xNewRect,v.y);
		x = sizeWindow.x - lx;
		sprite.setPosition(x,y);
		backGShape.setFillColor(sf::Color::White);
		backGShape.setPosition(x,y);
	}
	sf::Vector2f fooSize = backGShape.getSize();
	if (fooSize.y < sizeWindow.y-y)
	{
		backGShape.setSize(sf::Vector2f(fooSize.x, sizeWindow.y-y));
	}
	if (isMouseHere || select)
	{
		if (delta != 0)
		{
			delta = delta*yScroll;
			int sizeY = sizeWindow.y;
			sf::Vector2f fooPos = selectRect.getPosition();
			if (y+ly+delta<sizeY)
			{
				y = -1*ly+sizeY;
				delta = -y -1*ly+sizeY;
			}
			if (y+delta>y0)
			{
				y = y0;
				delta = y0-y;
			}
			else
				y = y+delta;
			
			fooPos.y += delta;			
			selectRect.setPosition(fooPos);	
			backGShape.setPosition(x,y);
			sprite.setPosition(x,y);
			posClick = sf::Vector2i(posClick.x, posClick.y+delta);
			delta = 0;	
		}
		if (isMousePressed)
		{
			if (!takeMidX)
				posClick.x = floor((posClick.x*1.-x)/xSprites)*xSprites+x;
			else
				posClick.x = floor((posClick.x*1.-x-xSprites/2)/xSprites)*xSprites+x+xSprites/2;
			if (!takeMidY)
				posClick.y = floor((posClick.y*1.-y)/ySprites)*ySprites+y;
			else
				posClick.y = floor((posClick.y*1.-y-ySprites/2)/ySprites)*ySprites+y+ySprites/2;
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
					fooInt[0] = ceil((posMouse.x*1.-x)/xSprites)*xSprites+x;
				else
					fooInt[0] = ceil((posMouse.x*1.-x-xSprites/2)/xSprites)*xSprites+x+xSprites/2;
				if (!takeMidY)
					fooInt[1] = ceil((posMouse.y*1.-y)/ySprites)*ySprites+y;
				else
					fooInt[1] = ceil((posMouse.y*1.-y-ySprites/2)/ySprites)*ySprites+y+ySprites/2;
		
				int xmin = min(posClick.x, fooInt[0]);
				int xmax = max(posClick.x, fooInt[0]);
				int ymin = min(posClick.y, fooInt[1]);
				int ymax = max(posClick.y, fooInt[1]);
				if (!takeMidX)
				{
					xmin = max(xmin, x);
					xmax = min(xmax, x+lx);
				}
				else
				{
					xmin = max(xmin, x+xSprites/2);
					xmax = min(xmax, x+lx-xSprites/2);
				}
				if (!takeMidY)			
				{
					ymin = max(ymin, y);
					ymax = min(ymax, y+ly);
				}
				else
				{
					ymin = max(ymin, y+ySprites/2);
					ymax = min(ymax, y+ly-ySprites/2);
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
				xmin = min(floor((foo.x-x)/xSprites)*xSprites+x,(float)1.*x+lx-xSprites);
				xmax = max(xmin+xSprites,round((foo.x+foo2.x-x)/xSprites)*xSprites+x);
				xmax = min(xmax, 1.*x+lx);
			}
			else
			{
				xmin = min(floor((foo.x-x)*2./xSprites)*xSprites/2.+x,x+lx-1.5*xSprites);
				xmax = max(xmin+xSprites,round((foo.x+foo2.x-x-xSprites/2)/xSprites)*xSprites+x+xSprites/2);	
				xmax = min(xmax, x+lx-xSprites/2.);
			}
			if (!takeMidY)
			{
				ymin = min(floor((foo.y-y)/ySprites)*ySprites+y,(float)1.*y+ly-ySprites);
				ymax = max(ymin+ySprites,round((foo.y+foo2.y-y)/ySprites)*ySprites+y);
				ymax = min(ymax, 1.*y+ly);
			}
			else
			{
				ymin = min(floor((foo.y-y)*2./ySprites)*ySprites/2.+y,y+ly-1.5*ySprites);
				ymax = max(ymin+ySprites,round((foo.y+foo2.y-y-ySprites/2)/ySprites)*ySprites+y+ySprites/2);	
				ymax = min(ymax, y+ly-ySprites/2.);		
			}
			selectRect.setPosition(xmin,ymin);
			selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
	
			nxSprites = round((xmax-xmin)/xSprites);
			nySprites = round((ymax-ymin)/ySprites);
			spriteToPull = new int[2];
			spriteToPull[0] = xmin-x;
			spriteToPull[1] = ymin-y;
		}
	}
}

void imagePNG::draw()
{
	window->draw(backGShape);
	window->draw(sprite);
	if (select || isMousePressed)
		window->draw(selectRect);
}
