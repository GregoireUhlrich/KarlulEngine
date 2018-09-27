#include "../include/supportWindow.h"
#include "../include/button.h"

int quitWindow(sf::RenderWindow* w)
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
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
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

int saveWindow(sf::RenderWindow* w)
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
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
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

int loadWindow(sf::RenderWindow* w)
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
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
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


int reinitWindow(sf::RenderWindow* w)
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
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
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

vector<int> changeSizeMapWindow(sf::RenderWindow* w, mapi* M)
{
    vector<int> sizeMapNew(2);
    sf::Vector2u sizeMap = M->getSizeMap();
    sizeMapNew[0] = sizeMap.x;
    sizeMapNew[1] = sizeMap.y;
    
    sf::RenderWindow windowC(sf::VideoMode(640,170),"Choose map dimensions");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    textBox width(&windowC, M, 'X', unsignedIntToString(sizeMap.x), 10,18,100,40,1);
    textBox height(&windowC, M, 'Y', unsignedIntToString(sizeMap.y), 430,18,100,40,1);
    signalButton SQ(&windowC,"Go !",10,118,200,35,0);
    signalButton C(&windowC,"Cancel",430,118,200,35,0);
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        SQ.testMouse(posMouse);
        C.testMouse(posMouse);
        width.testMouse(posMouse);
        height.testMouse(posMouse);
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
        
        bool textBoxEnabled = width.getPressed() or height.getPressed();
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                windowC.close();
                sizeMapNew[0] = sizeMap.x;
                sizeMapNew[1] = sizeMap.y;
                return sizeMapNew;
            }
            else if (event.type == sf::Event::Resized)
                windowC.setSize(sizeWindow);
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mousePressed(posMouse);
                C.mousePressed(posMouse);
                width.mousePressed(posMouse);
                height.mousePressed(posMouse);
                width.buttonPressed(posMouse);
                height.buttonPressed(posMouse);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                SQ.mouseReleased();
                C.mouseReleased();
                width.mouseReleased();
                height.mouseReleased();
            }
            else if (event.type == sf::Event::KeyPressed && !textBoxEnabled)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    width.moveLeft();
                    height.moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    width.moveRight();
                    height.moveRight();
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    width.moveLeft();
                    height.moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    width.moveRight();
                    height.moveRight();
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                sf::String foo = event.text.unicode;
                if (event.text.unicode == 8)
                {
                    width.backSpace();
                    height.backSpace();
                }
                else if (event.text.unicode == 13)
                {
                    width.enter();
                    height.enter();
                }
                else    
                {
                    width.textEntered(foo);
                    height.textEntered(foo);
                }
            }
        }
        bool foo = 0;
        foo = SQ.updateSignal();
        if (foo)
        {
            windowC.close();
            sizeMapNew[0] = stringToUnsignedInt(width.getString());
            sizeMapNew[1] = stringToUnsignedInt(height.getString());
            //M->setSizeMap(sf::Vector2u(stringToUnsignedInt(width.getString()),stringToUnsignedInt(height.getString())));
            return sizeMapNew;
        }
        foo = C.updateSignal();
        if (foo)
        {
            windowC.close();
            sizeMapNew[0] = sizeMap.x;
            sizeMapNew[1] = sizeMap.y;
            return sizeMapNew;
        }
        height.update();
        width.update();
        windowC.clear(sf::Color(128,128,128,255));
        SQ.draw();
        C.draw();
        height.draw();
        width.draw();
        windowC.display();
    }
    return vector<int>(2);
}

