#include "../include/managerTriggerEvent.h"

using namespace std;

string fontManager = "Fonts/ubuntu-font-family/Ubuntu-L.ttf";
Manager::Manager(mapi* Mi, hero* hi, sf::RenderWindow* w)
{
    M = Mi,
    h = hi;
    window = w;
    nEvents = 0;
    events = vector<Event* >(0);
    triggers = vector<Trigger* >(0);
    listCharacter.setWindow(M->getMap());
}

Manager::Manager(const Manager& m)
{
    M = m.M,
    h = m.h;
    window = m.window;
    nEvents = m.nEvents;
    events = m.events;
    triggers = m.triggers;
    listCharacter = m.listCharacter;
}

Manager::~Manager()
{
    for (int i=0; i<nEvents; i++)
    {
        delete events[i];
        delete triggers[i];
    }
    events.clear();
    triggers.clear();
}

void Manager::addEvent()
{
    addEventWindow();
}

void Manager::deleteEvent()
{
    cout<<"Delete Event\n";
}

void Manager::addCharacter(character* c)
{
    listCharacter.addCharacter(c);
}

void Manager::deleteCharacter(character* c)
{
    listCharacter.deleteCharacter(c);
}

bool Manager::testPNJ(int ix, int iy, int diri)
{
    return listCharacter.testPNJ(ix,iy,diri);
}

void Manager::save(ofstream& f)
{
    f<<nEvents<<" #nEvents\n\n";
    for (int i=0; i<nEvents; i++)
    {
        events[i]->saveEvent(f);
        triggers[i]->saveTrigger(f);
    }
}

void Manager::createEvents(ifstream& f)
{
    string foo;
    f>>nEvents>>foo;
    for (int i=0; i<nEvents; i++)
    {
        f>>foo;
        if (foo == "ChangeMap:")
        {
            events.push_back(new ChangeMap(M,h,window,f));
        }
        else if (foo == "TextInteraction:")
        {
            events.push_back(new TextInteraction(M,h,window,f));
        }
        else if (foo == "StaticPNJ:")
        {
            events.push_back(new StaticPNJ(M,h,window,f));
        }
        else
        {
            events.push_back(NULL);
            triggers.push_back(NULL);
            continue;
        }
        f>>foo;
        if (foo == "Cross:")
        {
            triggers.push_back(new Cross(M,h,window,f));
        }
        else if (foo == "Action:")
        {
            triggers.push_back(new Action(M,h,window,f));
        }
        else if (foo == "Gate:")
        {
            triggers.push_back(new Gate(M,h,window,f));
        }
        else if (foo == "TurningAround:")
        {
            triggers.push_back(new TurningAround(M,h,window,f));
        }
        else
        {
            delete events[i];
            events[i] = NULL;
            triggers.push_back(NULL);
            continue;
        }
    }
}

void Manager::test(double eT)
{
    for (int i=0; i<nEvents; i++)
    {
        if (triggers[i]->test(eT))
        {
            events[i]->activate();
        }
    }
}

void Manager::update(double eT)
{
    test(eT);
    for (int i=0; i<nEvents; i++)
        events[i]->update(eT);
    h->setAction(0);
}

void Manager::draw(int p)
{
    if (p == 2) listCharacter.draw();
    for (int i=0; i<nEvents; i++)
        events[i]->draw(p);
}

void Manager::addEventWindow()
{
    int sizeX = 700;
    int sizeY = 500;
    sf::RenderWindow windowC(sf::VideoMode(sizeX,sizeY),"Event creator");
    windowC.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2,sf::VideoMode::getDesktopMode().height/2));
    sf::Vector2i posMouse = sf::Mouse::getPosition(windowC);
    sf::Vector2u sizeWindow = windowC.getSize();
    
    int sizeButtons = 40;
    wrapMenuUX wrapMenuEvent(&windowC,M,"Select event", (sizeX/2-sizeX/3)/2,sizeButtons/2,sizeX/3,sizeButtons, 0);
    wrapMenuEvent.addChoice("Change Map");
    wrapMenuEvent.addChoice("Text Interaction");
    wrapMenuEvent.addChoice("Static PNJ");
    wrapMenuEvent.setWrapped(1);
    wrapMenuUX wrapMenuTrigger(&windowC,M,"Select trigger",sizeX/2+ (sizeX/2-sizeX/3)/2,sizeButtons/2,sizeX/3,sizeButtons, 0);
    wrapMenuTrigger.addChoice("Cross");
    wrapMenuTrigger.addChoice("Action");
    wrapMenuTrigger.addChoice("Gate");
    wrapMenuTrigger.addChoice("Turning Around");
    wrapMenuTrigger.setWrapped(1);
    
    int lQS = 125;
    int lC = 100;
    int sizeB = sizeButtons;
    int posX = sizeX-2.25*sizeB-lQS-lC;
    int posY = sizeY-1.75*sizeB;
    int lenBox = 150;
    double xText, yText, sizeString;
    int characterSize = 20;
    
    int nTexts = 8;
    vector<sf::Text> text(nTexts);
    int nTextBoxes = 8;
    int nTextBoxesEvent = 5;
    vector <textBox*> textBoxes(nTextBoxes);
    text[0].setString(sf::String("x = "));
    text[1].setString(sf::String("y = "));
    text[2].setString(sf::String("dir = "));
    text[3].setString(sf::String("File : "));
    text[4].setString(sf::String("File picture: "));
    text[5].setString(sf::String("x = "));
    text[6].setString(sf::String("y = "));
    text[7].setString(sf::String("dir = "));
    sf::FloatRect foo;
    sf::Font font;
    font.loadFromFile(fontManager);
    for (int i=0; i<nTexts; i++)
    {
        text[i].setCharacterSize(characterSize);
        text[i].setFont(font);
        text[i].setColor(sf::Color::Black);
    }
    
    int y0 = 2.5*sizeB;
    int x0 = sizeB;
    int dx = 110;
    foo = text[0].getLocalBounds();
    sizeString = foo.width;
    xText = x0;
    yText = y0;
    text[0].setPosition(round(xText), round(yText));
    textBoxes[0] = new textBox(&windowC, M, 'X', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[1].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[1].setPosition(round(xText), round(yText));
    textBoxes[1] = new textBox(&windowC, M, 'Y', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[2].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[2].setPosition(round(xText), round(yText));
    textBoxes[2] = new textBox(&windowC, M, 'Y', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[3].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[3].setPosition(round(xText), round(yText));
    textBoxes[3] = new textBox(&windowC, M, 'N', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[4].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[4].setPosition(round(xText), round(yText));
    textBoxes[4] = new textBox(&windowC, M, 'N', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[5].getLocalBounds();
    sizeString = foo.width;
    xText += sizeX/2;
    yText = y0;
    text[5].setPosition(round(xText), round(yText));
    textBoxes[5] = new textBox(&windowC, M, 'X', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[6].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[6].setPosition(round(xText), round(yText));
    textBoxes[6] = new textBox(&windowC, M, 'Y', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    foo = text[7].getLocalBounds();
    sizeString = foo.width;
    yText += 1.5*sizeB;
    text[7].setPosition(round(xText), round(yText));
    textBoxes[7] = new textBox(&windowC, M, 'Y', "", round(xText+dx),round(yText-characterSize/2),lenBox,sizeB,1);
    
    int nSignalButtons = 2;
    vector<signalButton*> signal(nSignalButtons);
    signal[0] = new signalButton(&windowC,"Validate",sf::Color(97,184,114),posX,posY,lQS,sizeB,0);
    signal[0]->disable();
    signal[1] = new signalButton(&windowC,"Cancel",sf::Color(84,106,121),posX+lQS+sizeB,posY,lC,sizeB,0);
    
    int nShapes = 2;
    vector<sf::RectangleShape> shape(nShapes);
    int offsetShape0 = 0;
    int shapeWidth = 2;
    shape[0] = sf::RectangleShape(sf::Vector2f(shapeWidth,sizeY-2.5*sizeB - 2*offsetShape0));
    shape[0].setPosition(sizeX/2-shapeWidth*1./2,offsetShape0);
    shape[0].setFillColor(sf::Color(217,217,217));
    shape[1].setSize(sf::Vector2f(sizeX,shapeWidth));
    shape[1].setPosition(0,sizeY-2.5*sizeB);
    shape[1].setFillColor(sf::Color(217,217,217));
    sf::Clock clock;
    sf::Time elapsedTime = clock.restart();
    while (windowC.isOpen())
    {   
        bool enableValidation = 1;
        vector<int> fooInt1, fooInt2;
        if (wrapMenuEvent.getName() == "Change Map") fooInt1 = ChangeMap::getParams();
        else if (wrapMenuEvent.getName() == "Text Interaction") fooInt1 = TextInteraction::getParams();
        else if (wrapMenuEvent.getName() == "Static PNJ") fooInt1 = StaticPNJ::getParams();
        else
        {
            fooInt1 = vector<int>(nTextBoxesEvent);
            for (int i=0; i<nTextBoxesEvent; i++) fooInt1[i] = 0;
            enableValidation = 0;
        }
        for (int i=0; i<nTextBoxesEvent; i++)
        {
            if (fooInt1[i])
            {
                textBoxes[i]->enable();
                text[i].setColor(sf::Color::Black);
            }
            else
            {
                textBoxes[i]->disable();
                text[i].setColor(sf::Color(217,217,217));
            }
        }
        
        if (wrapMenuTrigger.getName() == "Cross") fooInt2 = Cross::getParams();
        else if (wrapMenuTrigger.getName() == "Action") fooInt2 = Action::getParams();
        else if (wrapMenuTrigger.getName() == "Gate") fooInt2 = Gate::getParams();
        else if (wrapMenuTrigger.getName() == "Turning Around") fooInt2 = TurningAround::getParams();
        else
        {
            fooInt2 = vector<int>(nTextBoxes-nTextBoxesEvent);
            for (int i=0; i<nTextBoxes-nTextBoxesEvent; i++) fooInt2[i] = 0;
            enableValidation = 0;
        }
        for (int i=nTextBoxesEvent; i<nTextBoxes; i++)
        {
            if (fooInt2[i-nTextBoxesEvent])
            {
                textBoxes[i]->enable();
                text[i].setColor(sf::Color::Black);
            }
            else
            {
                textBoxes[i]->disable();
                text[i].setColor(sf::Color(217,217,217));
            }
        }
        for (int i=0; i<nTextBoxesEvent; i++)
            enableValidation = enableValidation && (!fooInt1[i] || textBoxes[i]->getString().length() > 0);
        for (int i=nTextBoxesEvent; i<nTextBoxes; i++)
            enableValidation = enableValidation && (!fooInt2[i-nTextBoxesEvent] || textBoxes[i]->getString().length() > 0);
            
        if (!enableValidation) signal[0]->disable();
        else signal[0]->enable();
        
        bool textBoxEnabled = 0;
        for (int i=0; i<nTextBoxes; i++)
            textBoxEnabled = textBoxEnabled or textBoxes[i]->getPressed();
        if (windowC.getSize() != sizeWindow) windowC.setSize(sizeWindow);
        
        bool hasFocus = wrapMenuEvent.getIsMouseHere() or wrapMenuTrigger.getIsMouseHere();
        if (hasFocus)
        {
            wrapMenuEvent.testMouse(posMouse);
            wrapMenuTrigger.testMouse(posMouse);
        }
        else
        {
            if (!wrapMenuEvent.testMouse(posMouse) and !wrapMenuTrigger.testMouse(posMouse))
            {
                for (int i=0; i<nSignalButtons; i++)
                    signal[i]->testMouse(posMouse);
                for (int i=0; i<nTextBoxes; i++)
                    textBoxes[i]->testMouse(posMouse);
            }
        }
        
        
        posMouse = sf::Mouse::getPosition(windowC);
        sf::Event event;
        if (window->hasFocus()) windowC.requestFocus();
        while(window->pollEvent(event));
        while (windowC.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                windowC.close();
                break;
            }
            else if (event.type == sf::Event::Resized)
            {
                windowC.setSize(sizeWindow);
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                wrapMenuEvent.mousePressed(posMouse);
                wrapMenuEvent.buttonPressed(posMouse);
                wrapMenuTrigger.mousePressed(posMouse);
                wrapMenuTrigger.buttonPressed(posMouse);
                for (int i = 0; i<nSignalButtons; i++)
                {
                    signal[i]->mousePressed(posMouse);
                }
                for (int i=0; i<nTextBoxes; i++)
                {
                    textBoxes[i]->mousePressed(posMouse);
                    textBoxes[i]->buttonPressed(posMouse);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                wrapMenuEvent.mouseReleased();
                wrapMenuTrigger.mouseReleased();
                for (int i=0;i <nSignalButtons; i++)
                    signal[i]->mouseReleased();
                for (int i=0; i<nTextBoxes; i++)
                    textBoxes[i]->mouseReleased();
            }
            else if (event.type == sf::Event::KeyPressed && !textBoxEnabled)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->moveRight();
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->moveRight();
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                sf::String foo = event.text.unicode;
                if (event.text.unicode == 8)
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->backSpace();
                }
                else if (event.text.unicode == 13)
                {
                    int i0;
                    for (int i=0; i<nTextBoxes; i++)
                    {
                        if (textBoxes[i]->getPressed())
                        {
                            textBoxes[i]->enter();
                            i0 = i;
                            do{
                                textBoxes[i]->setActive(0);
                                textBoxes[(i+1)%nTextBoxes]->setActive(1);
                                i += 1;
                            }while(textBoxes[i%nTextBoxes]->getEnabled() == 0 && (i%nTextBoxes) != i0);
                            break;
                        }
                    }
                }
                else
                {
                    for (int i=0; i<nTextBoxes; i++)
                        textBoxes[i]->textEntered(foo);
                }
            }
        }
        windowC.clear(sf::Color::White);
        for (int i=0; i<nTextBoxes; i++)
            textBoxes[i]->update();
        if (signal[0]->updateSignal())
        {
            vector<string> fooString1(nTextBoxesEvent);
            vector<string> fooString2(nTextBoxes-nTextBoxesEvent);
            for (int i=0; i<nTextBoxesEvent; i++)
                fooString1[i] = textBoxes[i]->getString();
            for (int i=nTextBoxesEvent; i<nTextBoxes; i++)
                fooString2[i-nTextBoxesEvent] = textBoxes[i]->getString();
                
            if (wrapMenuEvent.getName() == "Change Map")
            {
                events.push_back(new ChangeMap(M,h,window,fooString1));
            }
            else if (wrapMenuEvent.getName() == "Text Interaction")
            {
                events.push_back(new TextInteraction(M,h,window,fooString1));
            }
            else if (wrapMenuEvent.getName() == "Static PNJ")
            {
                events.push_back(new StaticPNJ(M,h,window,fooString1));            
            }
            
            if (wrapMenuTrigger.getName() == "Cross")
            {
                triggers.push_back(new Cross(M,h,window,fooString2));
            }
            else if (wrapMenuTrigger.getName() == "Action")
            {
                triggers.push_back(new Action(M,h,window,fooString2));
            }
            else if (wrapMenuTrigger.getName() == "Gate")
            {
                triggers.push_back(new Gate(M,h,window,fooString2));
            }
            else if (wrapMenuTrigger.getName() == "Turning Around")
            {
                triggers.push_back(new TurningAround(M,h,window,fooString2));
            }
            nEvents += 1;
            windowC.close();
        }
        if (signal[1]->updateSignal())
        {
            windowC.close();
        }
        for (int i=0; i<nSignalButtons; i++)
        {
            signal[i]->draw();
        }
        for (int i=0; i<nShapes; i++)
            windowC.draw(shape[i]);
        for (int i=0; i<nTexts; i++)
            windowC.draw(text[i]);
        elapsedTime = clock.restart();
        for (int i=0; i<nTextBoxes; i++)
            textBoxes[i]->draw(elapsedTime.asSeconds());
        wrapMenuEvent.update();
        wrapMenuTrigger.update();
        wrapMenuEvent.draw();
        wrapMenuTrigger.draw();
        windowC.display();
    }
}

