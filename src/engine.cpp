#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/drawable.h"
#include "../include/map.h"
#include "../include/button.h"
#include "../include/character.h"
#include "../include/support.h"
#include "../include/supportWindow.h"
#include "../include/interactiveWindow.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
	int xWindow = sf::VideoMode::getDesktopMode().width-100;
	int yWindow = sf::VideoMode::getDesktopMode().height-100;
	int yMap = yWindow-170;
	sf::RenderWindow window(sf::VideoMode(xWindow,yWindow), "\'Map haie\', dit Thor");
	
	sf::SoundBuffer buffer1;
	buffer1.loadFromFile("Audio/Projet1Arch.ogg");
	sf::Sound son1;
	son1.setLoop(1);
	son1.setBuffer(buffer1);
	
    sf::View view;
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    sf::Vector2u sizeWindow = window.getSize();
    
   
    string map;
    ifstream file("Maps/dataMap.txt", ios::in);
    if (file) file>>map;
    else map = "map1.txt";
    file.close();
	character Heros("Paul", "Graphics/AC001.png", 0, 0);
    mapi* M = new mapi(&window, &Heros, map, yMap);
    int tmp = M->loadMap();
    sf::Vector2u sizeMap = M->getSizeMap();
    M->initMap();
	interactiveWindow IWindow(&window, M, 170);
    
    bool testJoueur = 0;
	sf::Clock clock;
	sf::Clock clockMap;
	sf::Clock clockTest;
	sf::Time elapsedTime = clock.restart();
	sf::Time mapElapsedTime = clockMap.restart();
	sf::Time timeTest = clockTest.restart();
	bool verbose = 0;
    while (window.isOpen())
    {
    	posMouse = sf::Mouse::getPosition(window);
    	M->testMouse(posMouse);
    	IWindow.testMouse(posMouse);
        sf::Event event;
        while (window.pollEvent(event))
        {
            IWindow.windowEvent(event);
            if (event.type == sf::Event::Closed)
            {	
                SaveStateMap s = M->getSaveState();
                if (s == edited || s == loaded)
                {
                    window.setActive(0);
                    int foo = quitWindow();
                    window.setActive(1);
                    switch(foo)
                    {
                        case 0:
                        window.close();
                        IWindow.closeWindow();
                        break;
                        
                        case 1:
                        M->saveMap();
                        window.close();
                        IWindow.closeWindow();
                        break;
                        
                        case 2: break;
                    }
                }
                else 
                {
                    window.close();
                    IWindow.closeWindow();                
                }
            }
            else if (event.type == sf::Event::Resized)
			{
				view.reset(sf::FloatRect(0,0,event.size.width,event.size.height));
				window.setView(view);
			}
			else if (event.type == sf::Event::KeyPressed)
	        {
		        if (event.key.code == sf::Keyboard::Escape)
		        {
			        testJoueur = !testJoueur;
                }
            }
			else if (event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::Down:
						Heros.setKeyPressed(0+5);
						break;
					case sf::Keyboard::Left:
					    Heros.setKeyPressed(1+5);
						break;
				
					case sf::Keyboard::Right:
						Heros.setKeyPressed(2+5);
						break;
					
					case sf::Keyboard::Up:
						Heros.setKeyPressed(3+5);
						break;        
				}
			}
        }
		window.clear();
		if (window.isOpen())
		{
			if (window.getSize() != sizeWindow)
			{
            	sizeWindow = window.getSize();
				view.reset(sf::FloatRect(0,0,sizeWindow.x,sizeWindow.y));
    		    window.setView(view);
            	M->windowResized(sf::Vector2u(sizeWindow.x,sizeWindow.y));
            	IWindow.windowResized();
            }
            mapElapsedTime = clockMap.restart();
			IWindow.update();
			M->update(mapElapsedTime.asSeconds());
			M->draw();
			IWindow.draw();
			if (testJoueur)
			{
			    IWindow.lossFocus();
				if (!M->downOK()) Heros.setKeyPressed(0+5);
				if (!M->leftOK()) Heros.setKeyPressed(1+5);
				if (!M->rightOK()) Heros.setKeyPressed(2+5);
				if (!M->upOK()) Heros.setKeyPressed(3+5);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					if (M->testDir(0)) Heros.setKeyPressed(0);
					else Heros.setDir(0);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					if (M->testDir(1)) Heros.setKeyPressed(1);
					else Heros.setDir(1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					if (M->testDir(2)) Heros.setKeyPressed(2);
					else Heros.setDir(2);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					if (M->testDir(3)) Heros.setKeyPressed(3);
					else Heros.setDir(3);
				}
				elapsedTime = clock.restart();
				Heros.update(elapsedTime.asSeconds());
			}
		}
		
		window.display();
    }
    delete M;
	
    return 0;
}
