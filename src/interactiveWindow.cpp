#include "../include/interactiveWindow.h"
#include <iostream>
using namespace std;

std::string fileFonts = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";

interactiveWindow::interactiveWindow(sf::RenderWindow* wi, mapi* Mi, int lyi)
{
    x = y = 0;
    ly = lyi;
    M = Mi;
    window = wi;
    sizeWindow = window->getSize();
    sizeMap = M->getSizeMap();
    lx = sizeWindow.x;
    posMouse = sf::Vector2i(0,0);
    
    buffer.loadFromFile("Audio/Projet1Arch.ogg");
    son1.setLoop(1);
    son1.setBuffer(buffer);
    
    nD = 16;
    nModeMap = 4;
    nPrio = 4;
    iButtonGame = 15;
    iAddButton = 0;
    mode = 1;
    iTexture = 0;
    newTexture = 0;
    prio = 0;
    takeMidX = 0;
    takeMidY = 0;
    isWrapped = 1;
    iWrap = 11;
    nWrap = 4;
    
    ctrl = 0;
    testJoueur = 0;
    string im1 = M->getImageFile();
    string map = "Maps/map1.txt";
    
    D = new drawable*[nD];
    int posX = 100;
    int posY = 50;
    D[0] = new buttonMapUX(window, M, adding, posX,posY,40,40,0);
    D[1] = new buttonMapUX(window, M, moving, posX+50,posY,40,40,0);
    D[2] = new buttonMapUX(window, M, selecting, posX+100,posY,40,40,0);
    D[3] = new buttonMapUX(window, M, erasing, posX+150,posY,40,40,0);
    
    posX = 665;
    D[4] = new buttonPrioUX(window, M, 0, posX+50,posY,40,40,0);
    D[5] = new buttonPrioUX(window, M, 1, posX+100,posY,40,40,0);
    D[6] = new buttonPrioUX(window, M, 2, posX+150,posY,40,40,0);
    D[7] = new buttonPrioUX(window, M, 3, posX+200,posY,40,40,0);
    D[8] = new buttonAllPrioUX(window, M, posX,posY,40,40,0);
    
    posX = 445;
    D[9] = new buttonShowPassUX(window, M, posX,posY,40,40,0);
    D[10] = new buttonGridUX(window, M, posX+50,posY,40,40,0);
    
    
    sf::Vector2i fooSizeIm = M->getSizeIm();
    int fooThickness = M->getThicknessIm();
    D[11] = new wrapMenuTextureUX(window,M,"Texture",0,ly+1,fooSizeIm.x+2*fooThickness,40,0,0);
    D[12] = new wrapMenuTextureUX(window,M,"Texture",0,ly+40+1,fooSizeIm.x+2*fooThickness,40,0,1);
    posX = 5;
    posY = 5;
    D[13] = new wrapMenuFile(window,M,"File",posX, posY,90,40,0);
    D[14] = new wrapMenuEdit(window,M,"Edit",posX+90,posY,90,40,0);
    D[15] = new interactiveButtonUX(window,M,960,50,140,40,0);
    
    M->setState(moving);
    D[1]->setPressed(1);
    D[4]->setPressed(1);
    D[9]->setPressed(1);
    
    backGShape.setPosition(0,0);
    backGShape.setSize(sf::Vector2f(lx,ly));
    backGShape.setFillColor(sf::Color::White);
    limitShape.setPosition(x, y+ly);
    sizeLimit = 1;
    limitShape.setSize(sf::Vector2f(lx,sizeLimit));
    limitShape.setFillColor(sf::Color(217,217,217));
    
    font.loadFromFile(fileFonts);
    characterSize = 20;
    nStaticText = 3;
    posX = 30;
    posY = 58;
    staticText = new sf::Text[nStaticText];
    staticText[0].setString("Action");
    staticText[0].setCharacterSize(characterSize);
    staticText[0].setFont(font);
    staticText[0].setPosition(posX,posY+40/2-characterSize);
    staticText[0].setColor(sf::Color(84,106,139));
    staticText[1].setString("Display");
    staticText[1].setCharacterSize(characterSize);
    staticText[1].setFont(font);
    staticText[1].setPosition(posX+320,posY+40/2-characterSize);
    staticText[1].setColor(sf::Color(84,106,139));
    staticText[2].setString("Layers");
    staticText[2].setCharacterSize(characterSize);
    staticText[2].setFont(font);
    staticText[2].setPosition(posX+565,posY+40/2-characterSize);
    staticText[2].setColor(sf::Color(84,106,139));
    
    nStaticShape = 3;
    staticShape = new sf::RectangleShape[nStaticShape];
    int l = 35;
    posX = 320;
    posY = 70;
    staticShape[0].setSize(sf::Vector2f(2, l));
    staticShape[0].setPosition(posX,posY-l/2);
    staticShape[0].setFillColor(sf::Color(232,232,232));
    staticShape[1].setSize(sf::Vector2f(2, l));
    staticShape[1].setPosition(posX+245,posY-l/2);
    staticShape[1].setFillColor(sf::Color(232,232,232));
    staticShape[2].setSize(sf::Vector2f(2,l));
    staticShape[2].setPosition(posX+610,posY-l/2);
    staticShape[2].setFillColor(sf::Color(232,232,232));
    
    posY = 18;
    posX = 350;
    nameMap.setString("Map: "+M->getFile());
    nameMap.setCharacterSize(characterSize);
    nameMap.setFont(font);
    nameMap.setStyle(sf::Text::Bold);
    nameMap.setPosition(round(sizeWindow.x-posX),round(posY+40/2-characterSize));
    nameMap.setColor(sf::Color(84,106,139));
    
    string fooString = M->getImageFile();
    int fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);
    fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);
    nameTexture.setString("Texture: "+fooString);
    nameTexture.setCharacterSize(characterSize);
    nameTexture.setFont(font);
    nameTexture.setStyle(sf::Text::Bold);
    nameTexture.setPosition(round(sizeWindow.x-posX),round(posY+40+40/2-characterSize));
    nameTexture.setColor(sf::Color(84,106,139));
    
    limitNames.setSize(sf::Vector2f(2,ly));
    limitNames.setPosition(sizeWindow.x-posX-15,0);
    limitNames.setFillColor(sf::Color(232,232,232));
}

interactiveWindow::~interactiveWindow()
{
    for (int i=0; i<nD; i++)
        delete D[i];
    delete[] D;
}

bool interactiveWindow::getTestJoueur() { return testJoueur;}

bool interactiveWindow::hasFocus()
{     
    bool foo = 0;
    for (int i=iWrap; i<iWrap+nWrap; i++)
        foo = foo || D[i]->getIsMouseHere();
    return foo;
}   

void interactiveWindow::lossFocus()
{
    for (int i=iWrap; i<iWrap+nWrap; i++)
        D[i]->setWrapped(1);
}

void interactiveWindow::testMouse(sf::Vector2i p)
{
    posMouse = p;
    if (!hasFocus())
    {
        for (int i=0; i<nD; i++)
            D[i]->testMouse(posMouse);
    }
    else
    {
        for (int i=iWrap; i<iWrap+nWrap; i++)
            if (D[i]->getIsMouseHere()) D[i]->testMouse(posMouse);
    }
}

void interactiveWindow::windowEvent(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed && !testJoueur)
    {
        if (event.mouseButton.button == sf::Mouse::Left && isWrapped)
        {
            posMouse = sf::Mouse::getPosition(*window);
            for (int i=0; i<nModeMap; i++)
            {
                if (D[i]->buttonPressed(posMouse))
                    mode = i;
            }
            for (int i=0; i<nModeMap; i++)
            {
                if (i != mode)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (D[i]->buttonPressed(posMouse))
                    prio = i-nModeMap;
            }
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (i-nModeMap != prio)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
                    
            for (int i=nModeMap+nPrio; i<nD; i++)
            {
                D[i]->mousePressed(posMouse);
                D[i]->buttonPressed(posMouse);
            }
            if (isWrapped) M->mousePressed(posMouse);
            sf::Vector2u foo = M->getSizeMap();
        }
        else if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (int i=iWrap; i<iWrap+nWrap; i++)
            {
                D[i]->mousePressed(posMouse);
                D[i]->buttonPressed(posMouse);
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right and isWrapped)
            M->setHerosPos(posMouse);
    }
    else if (event.type == sf::Event::MouseButtonReleased && !testJoueur)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {    
            posMouse = sf::Mouse::getPosition(*window);
            for (int i=0; i<nD; i++)
                D[i]->mouseReleased();
            if (isWrapped) M->mouseReleased();
        }
    }
    else if (event.type == sf::Event::MouseWheelMoved && !testJoueur)
    {
        for (int i=0; i<nD; i++)
            D[i]->mouseWheel(event.mouseWheel.delta);
        if (isWrapped) M->mouseWheel(event.mouseWheel.delta);
    }
    else if (event.type == sf::Event::Resized)
    {
        windowResized();
        for (int i=0; i<nD; i++)
            D[i]->windowResized(sf::Vector2u(event.size.width,event.size.height));
        M->windowResized(sf::Vector2u(event.size.width,event.size.height));
        
        sizeWindow = window->getSize();
        backGShape.setSize(sf::Vector2f(event.size.width, ly));
        limitShape.setSize(sf::Vector2f(event.size.width, sizeLimit));
    }    
    else if (event.type == sf::Event::KeyPressed)
    {
        M->keyPressed(event.key.code);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !testJoueur)
        {
            if (event.key.code == sf::Keyboard::C)
            {
                M->ctrlC();
                M->setSelect(0,0);
                mode=iAddButton;
                for (int i=0; i<nModeMap; i++)
                {
                    if (i != mode)
                        D[i]->setPressed(0);
                    else
                        D[i]->setPressed(1);
                }
            }
            if (event.key.code == sf::Keyboard::X)
            {
                M->ctrlX();
                M->setSelect(0,0);
                D[iAddButton]->setPressed(1);
                mode=iAddButton;
                for (int i=0; i<nModeMap; i++)
                {
                    if (i != mode)
                        D[i]->setPressed(0);
                    else
                        D[i]->setPressed(1);
                }
            }
            if (event.key.code == sf::Keyboard::Z) M->ctrlZ();
            if (event.key.code == sf::Keyboard::S) M->saveMap();
        }
        if (event.key.code == sf::Keyboard::Delete && !testJoueur) M->suppr();
        else if (event.key.code == sf::Keyboard::Escape)
        {
            testJoueur = !testJoueur;
            M->setJoueur(testJoueur);
            if (testJoueur)
            {
                for (int i=0; i<nModeMap; i++)
                {
                    D[i]->setPressed(0);
                }
                son1.play();
            }
            else son1.stop();
        }
        else if (event.key.code == sf::Keyboard::L)
        {
            takeMidX = !takeMidX;
            M->setTakeMidX(takeMidX);
        }
        else if (event.key.code == sf::Keyboard::M)
        {
            takeMidY = !takeMidY;
            M->setTakeMidY(takeMidY);
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            M->setSelect(0,0);
            D[0]->setPressed(1);
            mode=0;
            for (int i=0; i<nModeMap; i++)
            {
                if (i != mode)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::F)
        {
            M->setSelect(0,0);
            D[1]->setPressed(1);
            mode=1;
            for (int i=0; i<nModeMap; i++)
            {
                if (i != mode)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::G)
        {
            M->setSelect(0,0);
            D[2]->setPressed(1);
            mode=2;
            for (int i=0; i<nModeMap; i++)
            {
                if (i != mode)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::H)
        {
            M->setSelect(0,0);
            D[3]->setPressed(1);
            mode=3;
            for (int i=0; i<nModeMap; i++)
            {
                if (i != mode)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::C && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            prio = 0;
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (i-nModeMap != prio)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::V)
        {
            prio = 1;
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (i-nModeMap != prio)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::B)
        {
            prio = 2;
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (i-nModeMap != prio)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::N)
        {
            prio = 3;
            for (int i=nModeMap; i<nPrio+nModeMap; i++)
            {
                if (i-nModeMap != prio)
                    D[i]->setPressed(0);
                else
                    D[i]->setPressed(1);
            }
        }
        else if (event.key.code == sf::Keyboard::X && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            D[nModeMap+nPrio]->setPressed(!D[nModeMap+nPrio]->getPressed());
        else if (event.key.code == sf::Keyboard::J)
            D[nModeMap+nPrio+1]->setPressed(!D[nModeMap+nPrio+1]->getPressed());
        else if (event.key.code == sf::Keyboard::K)
            D[nModeMap+nPrio+2]->setPressed(!D[nModeMap+nPrio+2]->getPressed());
    }
}

void interactiveWindow::windowResized()
{
    sf::Vector2u foo = window->getSize();
    int deltaX = (int)foo.x - (int)sizeWindow.x;
    sizeWindow = foo;
    for (int i=0; i<nD; i++)
        D[i]->windowResized(foo);
    sf::Vector2f fooPos = nameMap.getPosition();
    fooPos.x = round(fooPos.x + deltaX);
    nameMap.setPosition(fooPos);
    fooPos = nameTexture.getPosition();
    fooPos.x = round(fooPos.x + deltaX);
    nameTexture.setPosition(fooPos);
    fooPos = limitNames.getPosition();
    fooPos.x = round(fooPos.x + deltaX);
    limitNames.setPosition(fooPos);
}

void interactiveWindow::update()
{
    if (D[iButtonGame]->getSignal())
    {
        testJoueur = 1;
        M->setJoueur(1);
    }
    for (int i=0; i<nD; i++)
        D[i]->update();
    isWrapped = !hasFocus();
    
    if (M->getFile() != nameMap.getString().toAnsiString())
    {
        nameMap.setString("Map: "+sf::String(M->getFile()));
    }
    string fooString = M->getImageFile();
    int fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);
    fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);
    if (fooString != nameTexture.getString().toAnsiString())
    {
        nameTexture.setString(sf::String("Texture: "+fooString));
    }
}

void interactiveWindow::closeWindow()
{
    window = 0;
    for (int i=0; i<nD; i++)
        D[i]->closeWindow();
}

void interactiveWindow::draw()
{
    window->draw(backGShape);
    window->draw(limitShape);
    for (int i=0; i<nStaticShape; i++)
        window->draw(staticShape[i]);
    for (int i=0; i<nStaticText; i++)
        window->draw(staticText[i]);
    for (int i=0; i<nD; i++)
        D[i]->draw();
    window->draw(limitNames);
    window->draw(nameMap);
    window->draw(nameTexture);
}
