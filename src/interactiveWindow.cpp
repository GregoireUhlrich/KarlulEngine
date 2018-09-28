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
    
    nD = 15;
    nModeMap = 4;
    nPrio = 4;
    nTextBox = 1;
    iTextBox = new int[1];
    iBoxImage = 11;
    iTextBox[0] = 11;
    iAddButton = 0;
    mode = 1;
    iTexture = 0;
    newTexture = 0;
    prio = 0;
    takeMidX = 0;
    takeMidY = 0;
    isWrapped = 1;
    iWrap = 12;
    nWrap = 3;
    
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
    
    D[11] = new textBox(window, M, 'L', sf::String(im1), 10,110,270,40,0);
    D[12] = new wrapMenuTexture(window,M,"Load Texture",290,110,200,40,0);
    
    posX = 5;
    posY = 5;
    D[13] = new wrapMenuFile(window,M,"File",posX, posY,90,40,0);
    D[14] = new wrapMenuEdit(window,M,"Edit",posX+90,posY,90,40,0);
    
    M->setState(moving);
    D[1]->setPressed(1);
    D[4]->setPressed(1);
    D[9]->setPressed(1);
    
    nTexture = M->getNTextures();
    fileTextures = M->getFileTextures();
    P = new pushButton*[nTexture];
    for (unsigned int i=0; i<nTexture; i++)
        P[i] = new pushButton(window, M, unsignedIntToString(i),500+i*50,110,40,40,0);
    P[0]->setPressed(1);
    
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
    staticText = new sf::Text[nStaticText];
    staticText[0].setString("Action");
    staticText[0].setCharacterSize(characterSize);
    staticText[0].setFont(font);
    staticText[0].setPosition(30,67+40/2-characterSize);
    staticText[0].setColor(sf::Color(84,106,139));
    staticText[1].setString("Display");
    staticText[1].setCharacterSize(characterSize);
    staticText[1].setFont(font);
    staticText[1].setPosition(350,67+40/2-characterSize);
    staticText[1].setColor(sf::Color(84,106,139));
    staticText[2].setString("Layers");
    staticText[2].setCharacterSize(characterSize);
    staticText[2].setFont(font);
    staticText[2].setPosition(595,67+40/2-characterSize);
    staticText[2].setColor(sf::Color(84,106,139));
    
    nStaticShape = 2;
    staticShape = new sf::RectangleShape[nStaticShape];
    int l = 35;
    staticShape[0].setSize(sf::Vector2f(2, l));
    staticShape[0].setPosition(320,80-l/2);
    staticShape[0].setFillColor(sf::Color(232,232,232));
    staticShape[1].setSize(sf::Vector2f(2, l));
    staticShape[1].setPosition(565,80-l/2);
    staticShape[1].setFillColor(sf::Color(232,232,232));
    
}

interactiveWindow::~interactiveWindow()
{
    for (int i=0; i<nD; i++)
        delete D[i];
    delete[] D;
    delete[] iTextBox;
}

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
    bool textBoxEnabled = 0;
    for (int i=0; i<nTextBox; i++)
        textBoxEnabled = textBoxEnabled || D[iTextBox[i]]->getPressed();
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
            if (sizeMap != foo)
            {
                sizeMap = foo;
                D[iTextBox[nTextBox-2]]->setString(unsignedIntToString(foo.x));
                D[iTextBox[nTextBox-1]]->setString(unsignedIntToString(foo.y));
            }
            for (int i=0; i<nTexture; i++)
            {
                if (P[i]->buttonPressed(posMouse))
                    iTexture = i;
            }
            for (int i=0; i<nTexture; i++)
            {
                if (i != iTexture)
                    P[i]->setPressed(0);
                else
                    P[i]->setPressed(1);
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (int i=iWrap; i<iWrap+nWrap; i++)
            {
                D[i]->mousePressed(posMouse);
                D[i]->buttonPressed(posMouse);
            }
            if (D[iWrap+nWrap-1]->getPressed())
            {
                iTexture = -1;
                for (int i=0; i<nTexture; i++)
                    P[i]->setPressed(0);
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
        for (int i=0; i<nD; i++)
            D[i]->windowResized(sf::Vector2u(event.size.width,event.size.height));
        M->windowResized(sf::Vector2u(event.size.width,event.size.height));
        for (int i=0; i<nTexture; i++)
            P[i]->windowResized(sf::Vector2u(event.size.width,event.size.height));
        
        sizeWindow = window->getSize();
        backGShape.setSize(sf::Vector2f(event.size.width, ly));
        limitShape.setSize(sf::Vector2f(event.size.width, sizeLimit));
    }    
    else if (event.type == sf::Event::KeyPressed && !textBoxEnabled)
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
        else if (event.key.code == sf::Keyboard::Left && !testJoueur)
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->moveLeft();
        }
        else if (event.key.code == sf::Keyboard::Right && !testJoueur)
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->moveRight();
        }
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
        else if (event.key.code == sf::Keyboard::K)
            D[nModeMap+nPrio+1]->setPressed(!D[nModeMap+nPrio+1]->getPressed());
        else if (event.key.code == sf::Keyboard::J)
            D[nModeMap+nPrio+3]->setPressed(!D[nModeMap+nPrio+3]->getPressed());
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Left && !testJoueur)
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->moveLeft();
        }
        else if (event.key.code == sf::Keyboard::Right && !testJoueur)
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->moveRight();
        }
    }
    else if (event.type == sf::Event::TextEntered && !testJoueur)
    {
        sf::String foo = event.text.unicode;
        if (event.text.unicode == 8)
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->backSpace();
        }
        else if (event.text.unicode == 13)
        {
            if (D[iBoxImage]->getPressed())
            {
                iTexture = -1;
                for (int i=0; i<nTexture; i++)
                    P[i]->setPressed(0);
            }
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->enter();                
        }
        else    
        {
            for (int i=0; i<nTextBox; i++)
                D[iTextBox[i]]->textEntered(foo);
        }
    }
}

void interactiveWindow::windowResized()
{
    sf::Vector2u foo = window->getSize();
    sizeWindow = foo;
    for (int i=0; i<nD; i++)
        D[i]->windowResized(foo);
    for (int i=0; i<nTexture; i++)
        P[i]->windowResized(foo);
}

void interactiveWindow::update()
{
    if (nTexture != M->getNTextures())
    {
        for (int i=0; i<nTexture; i++)
            delete P[i];
        delete[] P;
        nTexture = M->getNTextures();
        delete[] fileTextures;
        if (nTexture > 0)
        {
            fileTextures = M->getFileTextures();
            P = new pushButton*[nTexture];
            for (unsigned int i=0; i<nTexture; i++)
                P[i] = new pushButton(window, M, unsignedIntToString(i),500+i*50,110,40,40,0);
            P[nTexture-1]->setPressed(1);
        }
        else
        {
            P = 0;
            fileTextures = 0;
        }
    }
    for (int i=0; i<nD; i++)
        D[i]->update();
    isWrapped = !hasFocus();
    for (int i=0; i<nTexture; i++)
    {
        P[i]->update();
        if (P[i]->getPressed() && iTexture >= 0 && M->getImageFile() != fileTextures[i])
            M->initPNG(fileTextures[i],'L');
    }
}

void interactiveWindow::closeWindow()
{
    window = 0;
    for (int i=0; i<nD; i++)
        D[i]->closeWindow();
    for (int i=0; i<nTexture; i++)
        P[i]->closeWindow();
}

void interactiveWindow::draw()
{
    window->draw(backGShape);
    window->draw(limitShape);
    for (int i=0; i<nTexture; i++)
        P[i]->draw();
    for (int i=0; i<nStaticShape; i++)
        window->draw(staticShape[i]);
    for (int i=0; i<nStaticText; i++)
        window->draw(staticText[i]);
    for (int i=0; i<nD; i++)
        D[i]->draw();
}
