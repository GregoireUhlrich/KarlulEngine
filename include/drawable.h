#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

using namespace std;

class drawable
{
	protected:
	
	int x,y;
	int lx,ly;
	sf::RenderTarget* window;
	sf::Vector2u sizeWindow;
	sf::Vector2i posClick;
	sf::Vector2i posMouse;
	int delta;
	
	bool isMouseHere;
	bool isMousePressed;	
	bool click;
	
	public:
	
	drawable(sf::RenderTarget *w);
	virtual ~drawable();
	
	void setPosition(double xNew, double yNew);
	void setPosition (sf::Vector2f vNew);
	sf::Vector2i getPosition() const;
	sf::Vector2i getSize() const;
	sf::RenderTarget* getWindow() const;
	sf::Vector2u getSizeWindow() const;
	sf::Vector2i getPosClick() const;
	sf::Vector2i getPosMouse() const;
	int getDelta() const;
	bool getIsMouseHere() const;
	bool getIsMousePressed() const;
	bool getClick() const;
	
	virtual int testMouse (sf::Vector2i posMouse);
	virtual void mousePressed (sf::Vector2i posMouse);
	virtual bool buttonPressed (sf::Vector2i posMouse);
	virtual void setPressed(bool s){};
	virtual bool getPressed() const{};
	virtual void textEntered(sf::String s){};
	virtual void setString(string s){};
	virtual void backSpace(){};
	virtual void enter(){};
	virtual void moveLeft(){};
	virtual void moveRight(){};
	void mouseReleased();
	void mouseWheel(int d);
	virtual void windowResized (sf::Vector2u newSizeWindow);
	
	virtual void setWrapped(bool s){};
	virtual void addChoice(string c){};
	virtual void deleteChoice(int i){};
	
	virtual void setColor(const sf::Color& c);
	virtual void draw() = 0;
	virtual void update() = 0;
	
	virtual void closeWindow();

};

class movingCircle: public drawable
{
	private:
	
	sf::Vector2i posMouse;
	sf::CircleShape shape;
	sf::Color color;
	double r;
	
	public:
	
	movingCircle(sf::RenderTarget* w, const sf::Color& c, double xi, double yi, double ri);
	virtual ~movingCircle();
	
	sf::CircleShape getShape() const;
	void setShape(sf::CircleShape& s);
	void setRadius(double ri);
	virtual void setColor(const sf::Color& c);
	
	virtual int testMouse (sf::Vector2i v);
	virtual void update();
	virtual void draw();
};

class imagePNG: public drawable
{
	private:
	
	char pos;
	int y0;
	int xSprites,ySprites;
	bool select, loadedSprites;
	bool takeMidX, takeMidY;
	
	int yScroll;
	int nxSprites;
	int nySprites;
	int* spriteToPull;
	
	std::string file;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape backGShape;
	sf::RectangleShape selectRect;
	
	public:
	
	imagePNG(sf::RenderTarget* w, std::string f, char p, int y0i);
	virtual ~imagePNG();
	
	virtual int testMouse (sf::Vector2i posMouse);
	
	std::string getFile() const;
	sf::Vector2u getNSprites() const;
	int* pullSprites();
	void freeSprites();
	sf::Sprite getSprite() const;
	sf::RectangleShape getShapeBG() const;
	void setTakeMidX(bool t);
	void setTakeMidY(bool t);
	
	virtual void update();
	virtual void draw();
};


#endif

