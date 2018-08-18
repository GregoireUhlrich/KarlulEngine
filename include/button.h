#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "drawable.h"
#include "map.h"
#include "support.h"
#include <string>
using namespace std;

class button: public drawable
{
	protected:
	
	double ratioPressed;
	unsigned int characterSize;
	bool isPressed;
	bool isRight;
	
	sf::RectangleShape rect;
	sf::Text text;
	sf::Font font;
	
	public:
	
	button(sf::RenderTarget *w, string t, double x,double y,double lx,double ly,bool isRighti);
	virtual ~button();
	
	sf::RectangleShape getRect() const;
	sf::Text getText() const;
	sf::Font getFont() const;
	double getRatioPressed() const;
	unsigned int getCharacterSize() const;
	bool getIsRight() const;
	
	void pressButton();
	void releaseButton();
	void windowResized(sf::Vector2u newSizeWindow);
	
	virtual void update();	
	virtual void draw();
};

class signalButton: public button
{
    public:
    signalButton(sf::RenderTarget *w, string t, double x,double y,double lx,double ly,bool isRighti);
    ~signalButton();
    bool updateSignal();
};

class buttonIm: public button
{
	private:
	
	mapi* M;
	char chirality;
	
	public:
	
	buttonIm(sf::RenderTarget *w, mapi* M, char c, string t, double x, double y, double lx, double ly0,bool isRighti);
	~buttonIm();
	
	virtual void update();
};
	
	
class buttonMapp: public button
{
	private:
	
	mapi* M;
	StateMap sM; 
	
	public:
	
	buttonMapp(sf::RenderTarget *w, mapi* M, StateMap sMi, string t, double x, double y, double lx, double ly0,bool isRighti);
	~buttonMapp();
	
	virtual void update();
};
	
	
	
class buttonSave: public button
{
	private:
	
	mapi* M;
	char mode;
	
	public:
	
	buttonSave(sf::RenderTarget *w, mapi* M, char m, string t, double x, double y, double lx, double ly0,bool isRighti);
	~buttonSave();
	
	virtual void update();
};
	

class pushButton: public drawable
{
	protected:
	
	int isPressed;
	bool isRight;
	unsigned int characterSize;
	mapi* M;
	
	sf::RectangleShape rect;
	sf::Text text;
	sf::Font font;
	
	public:
	
	pushButton(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
	pushButton(const pushButton& b);
	virtual ~pushButton();
	
	sf::RectangleShape getRect() const;
	sf::Text getText() const;
	void setPressed(bool s);
	bool getPressed() const;
	
	bool buttonPressed(sf::Vector2i p);
	virtual void windowResized(sf::Vector2u newSizeWindow);
	
	virtual void update();	
	virtual void draw();
	
};
	

class buttonGrid: public pushButton
{
	private:
	
	int grid;
	
	public:
	
	buttonGrid(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
	virtual ~buttonGrid();
	
	void update();
};
	
class buttonMap: public pushButton
{
	private:
	
	bool active;
	StateMap s;
	
	public:
	
	buttonMap(sf::RenderTarget *w, mapi* Mi, StateMap si, string t, double x,double y,double lx,double ly, bool isRighti);
	virtual ~buttonMap();
	
	void setPressed(bool s);	
	void update();
};

class buttonPrio: public pushButton
{
	private:
	
	bool active;
	int prio;
	
	public:
	
	buttonPrio(sf::RenderTarget *w, mapi* Mi, int p, string t, double x,double y,double lx,double ly, bool isRighti);
	virtual ~buttonPrio();
	
	void setPressed(bool s);	
	void update();
};


class buttonAllPrio: public pushButton
{
	private:
	
	int allPrio;
	
	public:
	
	buttonAllPrio(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
	virtual ~buttonAllPrio();
	
	void update();
};


class buttonShowPass: public pushButton
{
	private:
	
	bool showPass;
	
	public:
	
	buttonShowPass(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
	virtual ~buttonShowPass();
	
	void update();
};


class textBox: public pushButton
{
	private:
	
	bool active;
	char chirality;
	std::size_t len;
	std::size_t pos;
	sf::Text textit;
	sf::String stringText;
	sf::RenderTexture texture;
	sf::View view;
	sf::Sprite sprite;
	sf::RectangleShape underline;
	
	public:
	
	textBox(sf::RenderTarget *w, mapi* Mi, char c, sf::String t, double x,double y,double lx,double ly, bool isRighti);
	~textBox();
	
	void setString(string s);
	void windowResized(sf::Vector2u newSizeWindow);
	void mousePressed(sf::Vector2i p);
	
	void textEntered(sf::String f);
	void backSpace();
	void enter();
	void moveLeft();
	void moveRight();
	
	void update();
	void draw();
};

class wrapMenu: public drawable
{
    protected:
    
    int yMenu;
    int lyMenu;
    int ly0;
    int yScroll;
    int sizeWrapInWindow;
    int offsetTexture;
    
    bool isWrapped;
    bool isPressed;
	bool isRight;
	unsigned int characterSize;
	mapi* M;
	
	int nChoice;
	sf::Text* choice;
	int heightChoice;
	
	sf::RectangleShape rect;
	sf::Text text;
	sf::Font font;
	
	sf::RenderTexture* texture;
	sf::View view;
	sf::Sprite* sprite;
	
	sf::RectangleShape backGShape;
	sf::RectangleShape contourShape;
	sf::RectangleShape selectShape;
	sf::RectangleShape scrollBar;
	sf::Vector2f sizeScrollBar;
	
	public:
    wrapMenu(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenu();
    
    void setWrapped(bool s);
    void addChoice(string c);
    void deleteChoice(int i);
    void windowResized(sf::Vector2u newSizeWindow);
        
    virtual void update();
    void draw();
};

class wrapMenuLoad: public wrapMenu
{
    public:
    
    wrapMenuLoad(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuLoad();
    
    void update();
};

class wrapMenuTexture: public wrapMenu
{
    private:
    
    string dirPNG;
    
    public:
    
    wrapMenuTexture(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuTexture();
    
    void update();
};

#endif
