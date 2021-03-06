#include "../include/character.h"
#include <iostream>
#include <cmath>
using namespace std;

string path_sprites = "";
///*** Constructor ***///
character::character(string n, string f, double x, double y)
{
	file = f; // file of the texture
	name = n; // name of the carachter
	xSprites = ySprites = 32;
	x = round(x/xSprites)*xSprites;
	y = round(y/ySprites)*ySprites;
	direction = 0; // int between 0 and 3, direction in which the sprite looks
	typeSprite = 0.; // int between 0 and 3, which sprite for the walk animation
	onGrid = 1; // bool: is the character on a square or not ?
	keyPressed = -1; // int between 0 and 3, or -1 if no key is pressed
	speed = 6; // speed of the character in square/sec
	walk = 0.5; // number of times the total animation of walk is played in one square distance 
	walk *= 3*speed; // normalize to the right unit
	
	timeKeyPressed = 0; // time during which a key is pressed continuously
	thresholdMove = 0.15; // time from which the character starts to move (in seconds)
	
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
	sprite.setTextureRect(sf::IntRect(0,0,sx,sy));
	sprite.setPosition(x,y);
	
}

///*** get functions for private attributes ***///
string character::getName() const{ return name;}
string character::getFile() const{ return file;}
double character::getX() const{ return x;}
double character::getY() const{ return y;}
int character::getDir() const{ return direction;}
bool character::isOnGrid() const{ return onGrid;}
double character::getTypeSprite() const{ return typeSprite;}
sf::Sprite character::getSprite() const{ return sprite;}

///*** set fucntions for private variables ***///
void character::setName(string n){ name = n;}
void character::setFile(string f){ file = f;}
void character::setX(double xNew){ x = xNew;}
void character::setY(double yNew){ y = yNew;}
void character::setDir(int d){ if (onGrid) direction = d;}
	
///*** set the key interpreted by the game ***///
void character::setKeyPressed(int key)
{
	if (key >= 0) // if the key is valid
	{
		if (key == keyPressed+5) 
		{
			keyPressed = -1; // key+5 = the key has been released
		}
		else if (key == direction && key!= keyPressed) timeKeyPressed = thresholdMove + 1; 
		if (key < 4 && keyPressed == -1 && onGrid) // else if no key was already pressed
		{									  // we take the new key 
			keyPressed = key;
			direction = key;
		}
	}
}

///*** Update position and sprite for a given elapsed time ***///
void character::update(double elapsedTime)
{
	if (keyPressed >= 0) //we move
	{
		if (timeKeyPressed <= thresholdMove) timeKeyPressed += elapsedTime; // to make sure the variable timeKeyPressed don't overflow if we don't stop moving ! 
		else
		{
			if (onGrid) onGrid = 0;
			move(elapsedTime);
			direction = keyPressed;
		}
	}
	else if (!onGrid)  // we finish/continue the move the arrive the next square
	{
		moveCarefully(elapsedTime);
	}
	else timeKeyPressed = 0; // if we don't move at all we 
	updateSprite(elapsedTime); //update of the sprite with the new position
}

///*** Move the caracter, d = v*t ***///
void character::move(double elapsedTime)
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
void character::moveCarefully(double elapsedTime)
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
void character::updateSprite(double elapsedTime)
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

///*** Useless function to print some of the attributes ***///
void character::print() const
{
	cout<<"Name: "<<name<<endl;
	cout<<"File: "<<file<<endl;
	cout<<"Position = ["<<x<<" , "<<y<<"]"<<endl;
	cout<<"Direction: "<<direction<<endl;
}
