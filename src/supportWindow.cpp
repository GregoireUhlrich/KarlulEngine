#include "../include/supportWindow.h"
#include "../include/button.h"
#include <dirent.h>
#include <fstream>

int quitWindow(sf::RenderWindow* w)
{
    int confirmation = 0;
    int sizeX = 650;
    int sizeY = 250;
    sf::RenderWindow windowC(sf::VideoMode(sizeX,sizeY),"Confirmation");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    int lQ = 80;
    int lQS = 160;
    int lC = 100;
    int sizeB = 40;
    int posX = sizeX-2.75*sizeB-lQ-lQS-lC;
    int posY = sizeY-1.75*sizeB;
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(sizeX,2));
    line.setPosition(0,sizeY-2.5*sizeB);
    line.setFillColor(sf::Color(217,217,217));
    signalButton Q(&windowC,"Quit",sf::Color(215,90,74),posX,posY,lQ,sizeB,0);
    signalButton SQ(&windowC,"Save and Quit",sf::Color(97,184,114),posX+lQ+sizeB,posY,lQS,sizeB,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),posX+lQ+lQS+2*sizeB,posY,lC,sizeB,0);
    
    sf::Text text;
    text.setString(sf::String("Are you sure you want to quit? Any unsaved change will be lost."));
    int characterSize = 20;
    text.setCharacterSize(characterSize);
    sf::Font font;
    font.loadFromFile(sf::String("Fonts/ubuntu-font-family/Ubuntu-L.ttf"));
    text.setFont(font);
    text.setColor(sf::Color::Black);
    sf::FloatRect foo = text.getLocalBounds();
    float sizeString = foo.width;
    float xText = (sizeX-sizeString)/2;
    float yText = (sizeY-2.5*sizeB-1.15*characterSize)/2;
    text.setPosition(round(xText), round(yText));
    
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
        windowC.clear(sf::Color::White);
        SQ.draw();
        Q.draw();
        C.draw();
        windowC.draw(line);
        windowC.draw(text);
        
        windowC.display();
    }
    return confirmation;
}

int saveWindow(sf::RenderWindow* w)
{
    int confirmation = 0;
    
    sf::RenderWindow windowC(sf::VideoMode(430,70),"Confir ?");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    signalButton SQ(&windowC,"Save and Erase",sf::Color(97,184,114),10,18,200,35,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),220,18,200,35,0);
    
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
    int sizeX = 730;
    int sizeY = 250;
    sf::RenderWindow windowC(sf::VideoMode(sizeX,sizeY),"Confirmation");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    int lQ = 80;
    int lQS = 160;
    int lC = 100;
    int sizeB = 40;
    int posX = sizeX-2.75*sizeB-lQ-lQS-lC;
    int posY = sizeY-1.75*sizeB;
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(sizeX,2));
    line.setPosition(0,sizeY-2.5*sizeB);
    line.setFillColor(sf::Color(217,217,217));
    signalButton Q(&windowC,"Load",sf::Color(215,90,74),posX,posY,lQ,sizeB,0);
    signalButton SQ(&windowC,"Save and Load",sf::Color(97,184,114),posX+lQ+sizeB,posY,lQS,sizeB,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),posX+lQ+lQS+2*sizeB,posY,lC,sizeB,0);
    
    sf::Text text;
    text.setString(sf::String("Are you sure you want to load the map? Any unsaved change will be lost."));
    int characterSize = 20;
    text.setCharacterSize(characterSize);
    sf::Font font;
    font.loadFromFile(sf::String("Fonts/ubuntu-font-family/Ubuntu-L.ttf"));
    text.setFont(font);
    text.setColor(sf::Color::Black);
    sf::FloatRect foo = text.getLocalBounds();
    float sizeString = foo.width;
    float xText = (sizeX-sizeString)/2;
    float yText = (sizeY-2.5*sizeB-1.15*characterSize)/2;
    text.setPosition(round(xText), round(yText));
    
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
        windowC.clear(sf::Color::White);
        SQ.draw();
        Q.draw();
        C.draw();
        windowC.draw(line);
        windowC.draw(text);
        
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
    signalButton SQ(&windowC,"Save and Reinit",sf::Color(97,184,114),10,18,200,35,0);
    signalButton Q(&windowC,"Reinit",sf::Color(215,90,74),220,18,200,35,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),430,18,200,35,0);
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

void newMapWindow(sf::RenderWindow* w, mapi* M)
{
    vector<int> sizeMapNew(2);
    sf::Vector2u sizeMap = M->getSizeMap();
    sizeMapNew[0] = sizeMap.x;
    sizeMapNew[1] = sizeMap.y;
    
    int sizeX = 550;
    int sizeY = 350;
    sf::RenderWindow windowC(sf::VideoMode(sizeX,sizeY),"Map dimensions");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    int lQS = 125;
    int lQ = 110;
    int lC = 100;
    int sizeB = 40;
    int posX = sizeX-2.75*sizeB-lQS-lC-lQ;
    int posY = sizeY-1.75*sizeB;
    int lenBox = 150;
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(sizeX,2));
    line.setPosition(0,sizeY-2.5*sizeB);
    line.setFillColor(sf::Color(217,217,217));
    signalButton Q(&windowC,"Replace",sf::Color(215,90,74),posX,posY,lQ,sizeB,0);
    signalButton SQ(&windowC,"Validate",sf::Color(97,184,114),posX+lQ+sizeB,posY,lQS,sizeB,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),posX+lQS+lQ+2*sizeB,posY,lC,sizeB,0);
    Q.disable();
    int leftOffset = 20;
    sf::Text text;
    text.setString(sf::String("Width (x axis):"));
    int characterSize = 20;
    text.setCharacterSize(characterSize);
    sf::Font font;
    font.loadFromFile(sf::String("Fonts/ubuntu-font-family/Ubuntu-L.ttf"));
    text.setFont(font);
    text.setColor(sf::Color::Black);
    sf::FloatRect foo = text.getLocalBounds();
    float sizeString = foo.width;
    float xText = (sizeX-(sizeString+lenBox))/2-leftOffset;
    float yText = (sizeY-2.5*sizeB)/2-(1.15*characterSize)/2;
    text.setPosition(round(xText), round(yText));
    
    sf::Text textMap;
    textMap.setString("Name:");
    textMap.setFont(font);
    textMap.setColor(sf::Color::Black);
    textMap.setCharacterSize(characterSize);
    foo = textMap.getLocalBounds();
    sizeString = foo.width;
    xText = (sizeX-(sizeString+lenBox))/2-leftOffset;
    yText = (sizeY-2.5*sizeB)/4-(1.15*characterSize)/2;
    textMap.setPosition(round(xText), round(yText));
    
    
    sf::Text text2;
    text2.setString(sf::String("Height (y axis):"));
    text2.setCharacterSize(characterSize);
    text2.setFont(font);
    text2.setColor(sf::Color::Black);
    foo = text2.getLocalBounds();
    sizeString = foo.width;
    xText = (sizeX-(sizeString+lenBox))/2-leftOffset;
    yText = 3*(sizeY-2.5*sizeB)/4-1.15*characterSize/2;
    text2.setPosition(round(xText), round(yText));
    
    textBox width(&windowC, M, 'X', unsignedIntToString(sizeMap.x), round(xText+150),round(yText-characterSize/2-(sizeY-2.5*sizeB)/4),lenBox,40,1);
    textBox height(&windowC, M, 'Y', unsignedIntToString(sizeMap.y), round(xText+150),round(yText-characterSize/2),lenBox,40,1);
    textBox name(&windowC, M, 'N', "", round(xText+150),round(yText-characterSize/2-(sizeY-2.5*sizeB)/2),lenBox,40,1);
    

    sf::Text text3;
    text3.setString(sf::String("Must be integers between 1 and 500."));
    text3.setCharacterSize(characterSize*2./3);
    text3.setFont(font);
    text3.setColor(sf::Color(146,161,176));
    foo = text3.getLocalBounds();
    sizeString = foo.width;
    text3.setPosition(round(xText+150+(lenBox-sizeString)/2),round(yText-characterSize/2)+50);
    
    int vOffsetTxt = 10;
    sf::Text textTxt;
    textTxt.setString(".txt");
    textTxt.setFont(font);
    textTxt.setColor(sf::Color::Black);
    textTxt.setCharacterSize(characterSize);
    foo = textTxt.getLocalBounds();
    sizeString = foo.width;
    xText = round(xText+150)+lenBox+10;
    yText = round(yText-characterSize/2-(sizeY-2.5*sizeB)/2+vOffsetTxt);
    textTxt.setPosition(round(xText), round(yText));
    
    
    sf::Text textWarning;
    textWarning.setString(sf::String("Warning: the map \"")+sf::String("test.txt")+sf::String("\" already exists! Replace it ?"));
    textWarning.setCharacterSize(characterSize*2./3);
    textWarning.setFont(font);
    textWarning.setColor(sf::Color(215,90,74));
    foo = textWarning.getLocalBounds();
    xText = (sizeX-foo.width)/2;
    yText = sizeY-2.5*sizeB + characterSize*1./3;
    textWarning.setPosition(round(xText),round(yText));
    
    sf::Clock clock;
    sf::Time elapsedTime = clock.restart();
    
    bool enableValidation = 1;
    bool mapAlreadyExists = 0;
    int foo1, foo2;
    string foo3;
    while (windowC.isOpen())
    {
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        
        Q.testMouse(posMouse);
        SQ.testMouse(posMouse);
        C.testMouse(posMouse);
        width.testMouse(posMouse);
        height.testMouse(posMouse);
        name.testMouse(posMouse);
        if (w->hasFocus()) windowC.requestFocus();
        while(w->pollEvent(event));
        
        bool textBoxEnabled = (width.getPressed() or height.getPressed() or name.getPressed());
        bool fooBool = 0;
        DIR * rep = opendir("./Maps/"); 
        if (rep != NULL) 
        { 
            struct dirent * ent; 
      
            while ((ent = readdir(rep)) != NULL) 
            { 
                if (string(ent->d_name) != "." && string(ent->d_name) != ".." && string(ent->d_name) != "dataMap.txt")
                {
                    foo3 = string(ent->d_name);
                    if (foo3 == name.getString()+".txt")
                    {
                        fooBool = 1;
                        break;
                    }
                }
            } 
            closedir(rep); 
        }
        if (fooBool)
        {
            SQ.disable();
            Q.enable();
            mapAlreadyExists = 1;
        }
        else
        {
            SQ.enable();
            Q.disable();
            mapAlreadyExists = 0;
        }
        foo1 = stringToUnsignedInt(width.getString());
        foo2 = stringToUnsignedInt(height.getString());
        if (foo1 > 0 and foo1 < 501 and foo2 > 0 and foo2 < 501 and name.getString().length() > 0)
        {
            if (!mapAlreadyExists) SQ.enable();
            enableValidation = 1;
        }
        else
        {
            Q.disable();
            SQ.disable();
            enableValidation = 0;
        }
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                windowC.close();
                sizeMapNew[0] = sizeMap.x;
                sizeMapNew[1] = sizeMap.y;
            }
            else if (event.type == sf::Event::Resized)
                windowC.setSize(sizeWindow);
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                Q.mousePressed(posMouse);
                SQ.mousePressed(posMouse);
                C.mousePressed(posMouse);
                width.mousePressed(posMouse);
                height.mousePressed(posMouse);
                width.buttonPressed(posMouse);
                height.buttonPressed(posMouse);
                name.mousePressed(posMouse);
                name.buttonPressed(posMouse);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                Q.mouseReleased();
                SQ.mouseReleased();
                C.mouseReleased();
                width.mouseReleased();
                height.mouseReleased();
                name.mouseReleased();
            }
            else if (event.type == sf::Event::KeyPressed && !textBoxEnabled)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    width.moveLeft();
                    height.moveLeft();
                    name.moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    width.moveRight();
                    height.moveRight();
                    name.moveRight();
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    width.moveLeft();
                    height.moveLeft();
                    name.moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    width.moveRight();
                    height.moveRight();
                    name.moveRight();
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                sf::String foo = event.text.unicode;
                if (event.text.unicode == 8)
                {
                    width.backSpace();
                    height.backSpace();
                    name.backSpace();
                }
                else if (event.text.unicode == 13)
                {
                    width.enter();
                    height.enter();
                    name.enter();
                }
                else    
                {
                    width.textEntered(foo);
                    height.textEntered(foo);
                    name.textEntered(foo);
                }
            }
        }
        bool foo = 0;
        foo = Q.updateSignal();
        if (foo)
        {
            windowC.close();
            sizeMapNew[0] = stringToUnsignedInt(width.getString());
            sizeMapNew[1] = stringToUnsignedInt(height.getString());
            M->reinitMap();
            M->setFile(name.getString()+".txt");
            M->loadEmpty((int)stringToUnsignedInt(width.getString()),(int)stringToUnsignedInt(height.getString()));
            M->saveMap();
        }
        foo = SQ.updateSignal();
        if (foo)
        {
            windowC.close();
            sizeMapNew[0] = stringToUnsignedInt(width.getString());
            sizeMapNew[1] = stringToUnsignedInt(height.getString());
            std::ofstream fooFile(("Maps/"+name.getString()+".txt").c_str(), std::ios::out);
            fooFile.close();
            M->reinitMap();
            M->setFile(name.getString()+".txt");
            M->loadEmpty((int)stringToUnsignedInt(width.getString()),(int)stringToUnsignedInt(height.getString()));
            M->saveMap();
        }
        foo = C.updateSignal();
        if (foo)
        {
            windowC.close();
        }
        height.update();
        width.update();
        name.update();
        windowC.clear(sf::Color::White);
        Q.draw();
        SQ.draw();
        C.draw();
        windowC.draw(line);
        windowC.draw(text);
        windowC.draw(text2);
        windowC.draw(textMap);
        windowC.draw(textTxt);
        if (mapAlreadyExists)
        {
            textWarning.setString(sf::String("Warning: the map \"")+sf::String(name.getString())+sf::String(".txt\" already exists! Replace it ?"));
            sf::FloatRect fooRect = textWarning.getLocalBounds();
            xText = (sizeX-fooRect.width)/2;
            yText = sizeY-2.5*sizeB + characterSize*1./3;
            textWarning.setPosition(round(xText),round(yText));
            windowC.draw(textWarning);
        }
        if (enableValidation == 0) text3.setColor(sf::Color(215,90,74));
        else text3.setColor(sf::Color(146,161,176));
        windowC.draw(text3);
        elapsedTime = clock.restart();
        height.draw(elapsedTime.asSeconds());
        width.draw(elapsedTime.asSeconds());
        name.draw(elapsedTime.asSeconds());
        windowC.display();
    }
}

vector<int> changeSizeMapWindow(sf::RenderWindow* w, mapi* M)
{
    vector<int> sizeMapNew(2);
    sf::Vector2u sizeMap = M->getSizeMap();
    sizeMapNew[0] = sizeMap.x;
    sizeMapNew[1] = sizeMap.y;
    
    int sizeX = 550;
    int sizeY = 300;
    sf::RenderWindow windowC(sf::VideoMode(sizeX,sizeY),"Map dimensions");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    int lQS = 125;
    int lC = 100;
    int sizeB = 40;
    int posX = sizeX-2.25*sizeB-lQS-lC;
    int posY = sizeY-1.75*sizeB;
    int lenBox = 150;
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(sizeX,2));
    line.setPosition(0,sizeY-2.5*sizeB);
    line.setFillColor(sf::Color(217,217,217));
    signalButton SQ(&windowC,"Validate",sf::Color(97,184,114),posX,posY,lQS,sizeB,0);
    signalButton C(&windowC,"Cancel",sf::Color(84,106,121),posX+lQS+sizeB,posY,lC,sizeB,0);
    
    int leftOffset = 20;
    sf::Text text;
    text.setString(sf::String("Width (x axis):"));
    int characterSize = 20;
    text.setCharacterSize(characterSize);
    sf::Font font;
    font.loadFromFile(sf::String("Fonts/ubuntu-font-family/Ubuntu-L.ttf"));
    text.setFont(font);
    text.setColor(sf::Color::Black);
    sf::FloatRect foo = text.getLocalBounds();
    float sizeString = foo.width;
    float xText = (sizeX-(sizeString+lenBox))/2-leftOffset;
    float yText = (sizeY-2.5*sizeB)/3-(1.15*characterSize)/2;
    text.setPosition(round(xText), round(yText));
    
    
    sf::Text text2;
    text2.setString(sf::String("Height (y axis):"));
    text2.setCharacterSize(characterSize);
    text2.setFont(font);
    text2.setColor(sf::Color::Black);
    foo = text2.getLocalBounds();
    sizeString = foo.width;
    xText = (sizeX-(sizeString+lenBox))/2-leftOffset;
    yText = 2*(sizeY-2.5*sizeB)/3-1.15*characterSize/2;
    text2.setPosition(round(xText), round(yText));
    
    textBox width(&windowC, M, 'X', unsignedIntToString(sizeMap.x), round(xText+150),round(yText-characterSize/2-(sizeY-2.5*sizeB)/3),lenBox,40,1);
    textBox height(&windowC, M, 'Y', unsignedIntToString(sizeMap.y), round(xText+150),round(yText-characterSize/2),lenBox,40,1);
    
    sf::Text text3;
    text3.setString(sf::String("Must be integers between 1 and 500."));
    text3.setCharacterSize(characterSize*2./3);
    text3.setFont(font);
    text3.setColor(sf::Color(146,161,176));
    foo = text3.getLocalBounds();
    sizeString = foo.width;
    text3.setPosition(round(xText+150+(lenBox-sizeString)/2),round(yText-characterSize/2)+50);
    
    sf::Clock clock;
    sf::Time elapsedTime = clock.restart();
    
    bool enableValidation = 1;
    int foo1, foo2;
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
        
        bool textBoxEnabled = (width.getPressed() or height.getPressed());
        foo1 = stringToUnsignedInt(width.getString());
        foo2 = stringToUnsignedInt(height.getString());
        if (foo1 > 0 and foo1 < 501 and foo2 > 0 and foo2 < 501)
        {
            SQ.enable();
            enableValidation = 1;
        }
        else
        {
            SQ.disable();
            enableValidation = 0;
        }
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
        windowC.clear(sf::Color::White);
        SQ.draw();
        C.draw();
        windowC.draw(line);
        windowC.draw(text);
        windowC.draw(text2);
        if (enableValidation == 0) text3.setColor(sf::Color(215,90,74));
        else text3.setColor(sf::Color(146,161,176));
        windowC.draw(text3);
        elapsedTime = clock.restart();
        height.draw(elapsedTime.asSeconds());
        width.draw(elapsedTime.asSeconds());
        windowC.display();
    }
    return vector<int>(2);
}

