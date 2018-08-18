#include "../include/supportWindow.h"
#include "../include/button.h"

int quitWindow()
{
    int confirmation = 0;
    
    sf::RenderWindow windowC(sf::VideoMode(640,70),"Are you sure you want to quit ?");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    signalButton SQ(&windowC,"Save and Quit",10,18,200,35,0);
    signalButton Q(&windowC,"Quit",220,18,200,35,0);
    signalButton C(&windowC,"Cancel",430,18,200,35,0);
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        SQ.testMouse(posMouse);
        Q.testMouse(posMouse);
        C.testMouse(posMouse);
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                confirmation = 2;
                windowC.close();
                break;
            }
            else if (event.type == sf::Event::Resized)
				windowC.setSize(sizeWindow);
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    	    {
        		SQ.mousePressed(posMouse);
        		Q.mousePressed(posMouse);
        		C.mousePressed(posMouse);
        	}
        	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mouseReleased();
                Q.mouseReleased();
                C.mouseReleased();
        	}
        }
        bool foo = 0;
        foo = SQ.updateSignal();
        if (foo)
        {
            confirmation = 1;
            windowC.close();
        }
        foo = Q.updateSignal();
        if (foo)
        {
            confirmation = 0;
            windowC.close();
        }
        foo = C.updateSignal();
        if (foo)
        {
            confirmation = 2;
            windowC.close();
        }
        windowC.clear(sf::Color(128,128,128,255));
        SQ.draw();
        Q.draw();
        C.draw();
        windowC.display();
    }
    return confirmation;
}

int saveWindow()
{
    int confirmation = 0;
    
    sf::RenderWindow windowC(sf::VideoMode(430,70),"Erase the existing map ?");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    signalButton SQ(&windowC,"Save and Erase",10,18,200,35,0);
    signalButton C(&windowC,"Cancel",220,18,200,35,0);
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        SQ.testMouse(posMouse);
        C.testMouse(posMouse);
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                confirmation = 1;
                windowC.close();
                break;
            }
            else if (event.type == sf::Event::Resized)
				windowC.setSize(sizeWindow);
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    	    {
        		SQ.mousePressed(posMouse);
        		C.mousePressed(posMouse);
        	}
        	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mouseReleased();
                C.mouseReleased();
        	}
        }
        bool foo = 0;
        foo = SQ.updateSignal();
        if (foo)
        {
            confirmation = 0;
            windowC.close();
        }
        foo = C.updateSignal();
        if (foo)
        {
            confirmation = 1;
            windowC.close();
        }
        windowC.clear(sf::Color(128,128,128,255));
        SQ.draw();
        C.draw();
        windowC.display();
    }
    return confirmation;
}

int loadWindow()
{
    int confirmation = 0;
    
    sf::RenderWindow windowC(sf::VideoMode(430,70),"Loading and erase current map ?");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    signalButton SQ(&windowC,"Loading and Erase",10,18,200,35,0);
    signalButton C(&windowC,"Cancel",220,18,200,35,0);
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        SQ.testMouse(posMouse);
        C.testMouse(posMouse);
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                confirmation = 1;
                windowC.close();
                break;
            }
            else if (event.type == sf::Event::Resized)
				windowC.setSize(sizeWindow);
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    	    {
        		SQ.mousePressed(posMouse);
        		C.mousePressed(posMouse);
        	}
        	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mouseReleased();
                C.mouseReleased();
        	}
        }
        bool foo = 0;
        foo = SQ.updateSignal();
        if (foo)
        {
            confirmation = 0;
            windowC.close();
        }
        foo = C.updateSignal();
        if (foo)
        {
            confirmation = 1;
            windowC.close();
        }
        windowC.clear(sf::Color(128,128,128,255));
        SQ.draw();
        C.draw();
        windowC.display();
    }
    return confirmation;
}


int reinitWindow()
{
    int confirmation = 0;
    
    sf::RenderWindow windowC(sf::VideoMode(640,70),"Are you sure you want to reinitialize the map ?");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    signalButton SQ(&windowC,"Save and Reinit",10,18,200,35,0);
    signalButton Q(&windowC,"Reinit",220,18,200,35,0);
    signalButton C(&windowC,"Cancel",430,18,200,35,0);
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        SQ.testMouse(posMouse);
        Q.testMouse(posMouse);
        C.testMouse(posMouse);
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                confirmation = 2;
                windowC.close();
                break;
            }
            else if (event.type == sf::Event::Resized)
				windowC.setSize(sizeWindow);
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    	    {
        		SQ.mousePressed(posMouse);
        		Q.mousePressed(posMouse);
        		C.mousePressed(posMouse);
        	}
        	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mouseReleased();
                Q.mouseReleased();
                C.mouseReleased();
        	}
        }
        bool foo = 0;
        foo = SQ.updateSignal();
        if (foo)
        {
            confirmation = 1;
            windowC.close();
        }
        foo = Q.updateSignal();
        if (foo)
        {
            confirmation = 0;
            windowC.close();
        }
        foo = C.updateSignal();
        if (foo)
        {
            confirmation = 2;
            windowC.close();
        }
        windowC.clear(sf::Color(128,128,128,255));
        SQ.draw();
        Q.draw();
        C.draw();
        windowC.display();
    }
    return confirmation;
}

