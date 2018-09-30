#ifndef INTERACTIVEWINDOW_H_INCLUDED
#define INTERACTIVEWINDOW_H_INCLUDED

#include "../include/drawable.h"
#include "../include/button.h"
#include "../include/map.h"
#include "../include/support.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string> 

class interactiveWindow
{
    private:
    
    int x, y;
    int lx, ly;
    mapi* M;
    sf::Vector2u sizeMap;
    sf::RenderWindow* window;
    sf::Vector2u sizeWindow;
    sf::Vector2i posMouse;
    
    int nD;
    int nStaticText;
    int nStaticShape;
    int nModeMap;
    int nPrio;
    int iAddButton;
    int mode;
    int iTexture;
    bool newTexture;
    int prio;
    bool takeMidX;
    bool takeMidY;
    int ctrl;
    drawable** D;
    int characterSize;
    sf::Font font;
    sf::Text* staticText;
    sf::RectangleShape* staticShape;
    sf::Text nameMap;
    sf::Text nameTexture;
    sf::RectangleShape limitNames;
    
    bool isWrapped;
    int iWrap, nWrap;
    bool testJoueur;
    int iButtonGame;
    
    
    int sizeLimit;
    sf::RectangleShape backGShape;
    sf::RectangleShape limitShape;
    
    sf::SoundBuffer buffer;
    sf::Sound son1;
    
    public:
    
    interactiveWindow(sf::RenderWindow* wi, mapi* Mi, int lyi);
    ~interactiveWindow();
    
    bool getTestJoueur();
    
    void closeWindow();
    
    bool hasFocus();
    void lossFocus();
    void testMouse(sf::Vector2i p);
    void windowEvent(sf::Event event);
    void windowResized();
    void update();
    void draw();
};

#endif
