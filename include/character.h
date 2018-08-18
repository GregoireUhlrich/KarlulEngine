#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
using namespace std;


///*** Class for the heros ***///
class character{

	private:
	
	string name;
	string file;
	int direction;
	bool onGrid;
	int keyPressed;
	int sx, sy, dx, dy;
	int xSprites, ySprites;
	double x,y;
	double speed;
	double typeSprite;
	double walk;
	double timeKeyPressed;
	double thresholdMove;
	
	sf::Texture texture;
	sf::Sprite sprite;
	
	public: 
	
	character(string name, string file, double x, double y);
	~character(){};
	
	string getName() const;
	string getFile() const;
	double getX() const;
	double getY() const;
	int getDir() const;
	bool isOnGrid() const;
	double getTypeSprite() const;
	sf::Sprite getSprite() const;
	
	void setName(string n);
	void setFile(string f);
	void setX(double xNew);
	void setY(double yNew);
	void setDir(int d);
	void setKeyPressed(int key);
	
	void move(double elapsedTime);
	void moveCarefully(double elapsedTime);
	void update(double elpasedTime);
	void updateSprite(double elapsedTime);
	
	void print() const;
	
};

#endif
