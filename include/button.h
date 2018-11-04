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
    protected:
    
    sf::Color c;
    bool enabled;
    
    public:
    signalButton(sf::RenderTarget *w, string t, sf::Color c, double x,double y,double lx,double ly,bool isRighti);
    ~signalButton();
    bool updateSignal();
    
    void enable();
    void disable();
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
    
    bool isPressed;
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

class pushButtonUX: public drawable
{
    protected:
    
    int isPressed;
    bool isRight;
    unsigned int characterSize;
    mapi* M;
    
    sf::Texture textureOutline;
    sf::Sprite outline;
    
    public:
    
    pushButtonUX(sf::RenderTarget *w, mapi* Mi, double x,double y,double lx,double ly, bool isRighti);
    pushButtonUX(const pushButtonUX& b);
    virtual ~pushButtonUX();
    
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

class buttonGridUX: public pushButtonUX
{
    private:
    
    int grid;
    double xScaleIcon, yScaleIcon;
    sf::Texture textureA;
    sf::Texture textureB;
    sf::Sprite spriteA;
    sf::Sprite spriteB;
    
    public:
    
    buttonGridUX(sf::RenderTarget *w, mapi* Mi, double x,double y,double lx,double ly, bool isRighti);
    virtual ~buttonGridUX();
    
    void update();
    void draw();
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

class buttonMapUX: public pushButtonUX
{
    private:
    
    bool active;
    StateMap s;
    double xScaleIcon, yScaleIcon;
    sf::Texture textureA;
    sf::Texture textureB;
    sf::Sprite spriteA;
    sf::Sprite spriteB;
    
    public:
    
    buttonMapUX(sf::RenderTarget *w, mapi* Mi, StateMap si, double x,double y,double lx,double ly, bool isRighti);
    virtual ~buttonMapUX();
    
    void setPressed(bool s);    
    void update();
    void draw();
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


class buttonPrioUX: public pushButtonUX
{
    private:
    
    bool active;
    int prio;
    int characterSize;
    sf::Font font;
    sf::Text text;
    
    public:
    
    buttonPrioUX(sf::RenderTarget *w, mapi* Mi, int p, double x,double y,double lx,double ly, bool isRighti);
    virtual ~buttonPrioUX();
    
    void setPressed(bool s);    
    void update();
    void draw();
};


class buttonAllPrioUX: public pushButtonUX
{
    private:
    
    int allPrio;
    double xScaleIcon, yScaleIcon;
    sf::Texture textureA;
    sf::Texture textureB;
    sf::Sprite spriteA;
    sf::Sprite spriteB;
    
    public:
    
    buttonAllPrioUX(sf::RenderTarget *w, mapi* Mi, double x,double y,double lx,double ly, bool isRighti);
    virtual ~buttonAllPrioUX();
    
    void update();
    void draw();
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


class buttonShowPassUX: public pushButtonUX
{
    private:
    
    bool showPass;
    double xScaleIcon, yScaleIcon;
    sf::Texture textureA;
    sf::Texture textureB;
    sf::Sprite spriteA;
    sf::Sprite spriteB;
    
    public:
    
    buttonShowPassUX(sf::RenderTarget *w, mapi* Mi, double x,double y,double lx,double ly, bool isRighti);
    virtual ~buttonShowPassUX();
    
    void update();
    void draw();
};

class interactiveButtonUX: public drawable
{
    protected:
    
    bool isRight;
    bool isPressed;
    double ratioPressed;
    unsigned int characterSize;
    mapi* M;
    sf::Font font;
    sf::Text text;
    
    sf::RenderTexture renderTexture;
    sf::Sprite sprite;
    
    sf::Texture textureIcon1;
    sf::Sprite icon1;
    
    public:
    
    interactiveButtonUX(sf::RenderTarget *w, mapi* Mi, double x,double y,double lx,double ly, bool isRighti);
    interactiveButtonUX(const interactiveButtonUX& b);
    ~interactiveButtonUX();
    
    bool getSignal();
    void windowResized(sf::Vector2u newSizeWindow);
    
    void update();    
    void draw();
    
};

class textBox: public pushButton
{
    private:
    
    bool active;
    bool enabled;
    char chirality;
    std::size_t len;
    std::size_t pos;
    sf::Text textit;
    sf::String stringText;
    sf::RenderTexture texture;
    sf::View view;
    sf::Sprite sprite;
    sf::RectangleShape underline;
    
    double elapsedTime;
    double threshold;
    bool stateUnderline;
    
    public:
    
    textBox(sf::RenderTarget *w, mapi* Mi, char c, sf::String t, double x,double y,double lx,double ly, bool isRighti);
    ~textBox();
    
    void enable();
    void disable();
    
    void setString(string s);
    string getString() const;
    void windowResized(sf::Vector2u newSizeWindow);
    void mousePressed(sf::Vector2i p);
    void setActive(bool s);
    bool getEnabled();
    
    void textEntered(sf::String f);
    void backSpace();
    void enter();
    void moveLeft();
    void moveRight();
    
    void update();
    void draw(double elapsedTime);
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

class wrapMenuUX: public drawable
{
    protected:
    
    int yMenu;
    int lyMenu;
    int ly0;
    int yScroll;
    int sizeWrapInWindow;
    int offsetTexture;
    double maxSizeChoice;
    
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
    
    sf::RectangleShape contourShape;
    sf::RectangleShape selectShape;
    sf::RectangleShape scrollBar;
    sf::Vector2f sizeScrollBar;
    
    public:
    wrapMenuUX(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuUX();
    
    string getName();
    virtual int testMouse(sf::Vector2i v);
    virtual bool getIsMouseHere() const;
    void setWrapped(bool s);
    bool getWrapped();
    virtual void addChoice(string c);
    void deleteChoice(int i);
    virtual void windowResized(sf::Vector2u newSizeWindow);
        
    virtual void update();
    virtual void draw();
};

class wrapMenuSideLoad;

class wrapMenuFile: public wrapMenuUX
{
    private:
    
    wrapMenuSideLoad* mLoad;
    
    public:
    
    wrapMenuFile(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuFile();
    
    int testMouse(sf::Vector2i v);
    virtual bool getIsMouseHere() const;
    void mousePressed(sf::Vector2i posMouse);
    void mouseReleased();
    
    void update();
    void draw();
};

class wrapMenuSideEvent;
class wrapMenuSideScenario;

class wrapMenuEdit: public wrapMenuUX
{
    protected:
    
    sf::RenderWindow* windowFIX;
    wrapMenuSideEvent* mLoad;
    wrapMenuSideScenario* mScenario;
    
    public:
    
    wrapMenuEdit(sf::RenderWindow *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuEdit();
    
    int testMouse(sf::Vector2i v);
    virtual bool getIsMouseHere() const;
    void mousePressed(sf::Vector2i posMouse);
    void mouseReleased();
    
    void update();
    void draw();
};

class wrapMenuSide: public wrapMenuUX
{
    protected:
    
    int lxMenu;
    double ratio; 
    sf::Texture textureArrow;
    sf::Sprite spriteArrow;
    
    public:

    wrapMenuSide(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuSide();
    
    void setWrapped(bool s);
    int testMouse(sf::Vector2i v);
    void addChoice(string c);
    virtual void windowResized(sf::Vector2u newSizeWindow);
        
    virtual void update();
    virtual void draw();
};

class wrapMenuTextureUX: public wrapMenuSide
{
    protected:
    
    string dirPNG;
    int mode;
    int nTexture;
    string fileMap;
    
    public:
    
    wrapMenuTextureUX(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti, int mode);
    ~wrapMenuTextureUX();
    
    void addChoice(string c);
    
    void update();
    void draw();
};

class wrapMenuSideLoad: public wrapMenuSide
{
    private:
    
    sf::Texture textureArrow2;
    sf::Sprite spriteArrow2;
    
    public: 
    
    wrapMenuSideLoad(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuSideLoad();
    
    void draw();
};

class wrapMenuSideEvent: public wrapMenuSide
{
    private:
    
    sf::Texture textureArrow2;
    sf::Sprite spriteArrow2;
    
    public: 
    
    wrapMenuSideEvent(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuSideEvent();
    
    void update();
    void draw();
};

class wrapMenuSideScenario: public wrapMenuSide
{
    private:
    
    sf::Texture textureArrow2;
    sf::Sprite spriteArrow2;
    
    public: 
    
    wrapMenuSideScenario(sf::RenderTarget *w, mapi* Mi, string t, double x,double y,double lx,double ly, bool isRighti);
    ~wrapMenuSideScenario();
    
    void update();
    void draw();
};

#endif
