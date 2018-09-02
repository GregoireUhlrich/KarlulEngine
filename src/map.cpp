#include "../include/map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <sys/stat.h>
using namespace std;

string dirMaps = "Maps/";

mapi::mapi(sf::RenderWindow* w, character* H, string f, int height)
{
    stringDirPNG = "./Tileset/";
    window = w;
    Heros = H;
    sizeWindow = window->getSize();
    stringFile = f;
    state = moving;
    saveState = initialized;
    
    iZoom = 0.95;
    maxZoom = 5;
    minZoom = 0.3;
    isMouseHere = isMousePressed = 0;
    click = 0;
    select = select2 = 0;
    delta = 0;
    allPrio = 0;
    showPass = 1;
    currentPrio = 0;
    posMouse = sf::Vector2i(0,0);
    oldPosMouse = sf::Vector2i(0,0);
    posClick = sf::Vector2i(0,0);
    ghostSprite = 0;
    
    nxCtrlC = nyCtrlC = 0;
    xCtrlC = yCtrlC = 0;
    prioCtrlC = -1;
    
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color::Red);
    selectRect.setOutlineThickness(8.);
    
    grid = 0;
    
    lxMap = lyMap = 0;
    xSprites = ySprites = 32;
    nPrio = 4;
    nTotTexture = 0;
    indexSpriteVec = vector<vector<vector<vector<int> > > >(4);
    nTextureVec = vector<int>(4);
    nSpriteVec = vector<vector<int> >(4);
    passOrNotVec = vector<vector<vector<int> > >(4);
    spriteCtrlCVec = vector<vector<vector<sf::Sprite> > >(4);
    fileCtrlCVec = vector<vector<vector<string> > >(4);
    spriteVec = vector<vector<vector<sf::Sprite> > >(4);
    iTextureVec = vector<vector<int> >(4);
    nExceptions = 0;
    textureVec = vector<sf::Texture>(0);
    
    isImLeft = 0;
    isImRight = 0;
    sizeLim = 5;
    imL = 0;
    imR = 0;
    
    mapWindow.create(sizeWindow.x, height);
    x = 0;
    y = sizeWindow.y - height;
    lx = sizeWindow.x;
    ly = height;
    
    ctrlZObject = new mapCtrlZ(this);
    nEvents = 0;
}

mapi::~mapi()
{
    if (isImLeft)
    {
        delete imL;
        imL = 0;
    }
    delete ctrlZObject;
    freeSpritesCtrlC();
    
    for (int i=0; i<nEvents; i++)
    {
        if (events[i] != NULL) delete events[i];
    }
}

void mapi::initPNG(string f, char chirality)
{
    ifstream file(f.c_str());
    if (file)
    {
        if (chirality == 'L')
        {
            if (isImLeft)
            {
                delete imL;
                imL = new imagePNG(window, f, chirality, sizeWindow.y-ly);
            }
            else
            {
                isImLeft = 1;
                imL = new imagePNG(window, f, chirality, sizeWindow.y-ly);
            }
            sf::Vector2i v = imL->getSize();
            limL = sf::RectangleShape(sf::Vector2f(sizeLim,ly));
            limL.setFillColor(sf::Color::Black);
            limL.setPosition(v.x,y);
            
            sf::Vector2i foo = imL->imagePNG::getPosition();
            imL->imagePNG::setPosition(foo.x, sizeWindow.y-ly);
        }
        else if (chirality == 'R')
        {
            if (isImRight)
            {
                delete imR;
                imR = new imagePNG(window, f, chirality, sizeWindow.y-ly);
            }
            else
            {
                isImRight = 1;
                imR = new imagePNG(window, f, chirality, sizeWindow.y-ly);
            }
            sf::Vector2i v = imR->getSize();
            limR = sf::RectangleShape(sf::Vector2f(sizeLim,ly));
            limR.setFillColor(sf::Color::Black);
            limR.setPosition(sizeWindow.x-v.x,y);
            
            sf::Vector2i foo = imR->imagePNG::getPosition();
            imR->imagePNG::setPosition(foo.x, sizeWindow.y-ly);
            
            file.close();
        }
    }
    else cout<<"Invalid file!"<<endl;
}

void mapi::deletePNG(char chirality)
{
    if (isImLeft and chirality == 'L')
    {
        delete imL;
        isImLeft = 0;
    }
    else if (isImRight and chirality == 'R')
    {
        delete imR;
        isImRight = 0;
    }
}

sf::Vector2u mapi::getPosition() const { return sf::Vector2u(x,y);}

sf::Vector2u mapi::getSizeTexture() const { return sf::Vector2u(lx,ly);}

sf::Vector2u mapi::getSizeMap() const { return sf::Vector2u(lxMap,lyMap);}

sf::Vector2u mapi::getSizeSprites() const { return sf::Vector2u(xSprites, ySprites);}

SaveStateMap mapi::getSaveState() const {return saveState;}

bool mapi::isSaved() const { return saveState == saved;}

string mapi::getFile() const { return stringFile;}

string mapi::getImageFile() const { return imL->getFile();}

int mapi::getNTextures() const { return nTotTexture;}

string* mapi::getFileTextures() const
{
    string *toReturn = new string[nTotTexture];
    for (int i=0; i<nTotTexture; i++)
        toReturn[i] = fileTextureVec[i];
    return toReturn;
}

void mapi::setFile(string f)
{
    if (stringFile != f) stringFile = f;
}

void mapi::setDirPNG(string f)
{
    int foo = (int)f.length();
    if (f[foo-1] != '/')
        f = f+"/";
    stringDirPNG = f;
}

string mapi::getDirPNG() const
{
    return stringDirPNG;
}   

void mapi::setState(StateMap s)
{
    if (state == adding && s != adding) freeSpritesCtrlC();
    state = s;
    if (state == heros) 
    {
        float xView = Heros->getX()-lx/2.;
        float yView = Heros->getY()-ly/2.;
        if (xView < 0) xView = 0;
        if (xView > lxMap*xSprites-lx) xView = lxMap*xSprites-lx;    
        if (yView < 0) yView = 0;
        if (yView > lyMap*ySprites-ly) yView = lyMap*ySprites-ly;        
        if (lx > lxMap*xSprites) xView = -(lx-lxMap*xSprites)/2;
        if (ly > lyMap*ySprites) yView = -(ly-lyMap*ySprites)/2;
        viewMap.reset(sf::FloatRect(xView,yView,lx,ly));
        mapWindow.setView(viewMap);
    }
}

void mapi::setSaveState(SaveStateMap s) { saveState = s;}

void mapi::setGrid(bool g){ grid = g;}

void mapi::setPrio(int newPrio)
{
    if (newPrio<0 || newPrio>nPrio)
        cout<<"Invalid prio!\n";
    else
        currentPrio = newPrio;
}

void mapi::setAllPrio(bool a)
{
    allPrio = a;
}

void mapi::setShowPass(bool s)
{
    showPass = s;
}

void mapi::setPassOrNot(int pass, int prio, int ix, int iy)
{
    if (ix >= 0 && ix < lxMap && iy > 0 && iy < lyMap)
    {
        if (passOrNotVec[prio][ix][iy] == 0 && pass != 0)
        {
            nExceptions += 1;
            passOrNotVec[prio][ix][iy] = pass;
        }
        else if (passOrNotVec[prio][ix][iy] != 0 && pass == 0)
        {
            nExceptions -= 1;
            passOrNotVec[prio][ix][iy] = pass;
        }
        else
            passOrNotVec[prio][ix][iy] = pass;
    }
}

void mapi::pastePassOrNot(int dir, int prio, int xmin, int ymin, int lxS, int lyS)
{
    if (xmin >= 0 && xmin+lxS < lxMap && ymin > 0 && ymin+lyS < lyMap)
    {
        if (saveState != loaded) if (saveState != loaded) saveState = edited;
        for (int ix=xmin; ix<xmin+lxS; ix++)
        {
            for (int iy=ymin; iy<ymin+lyS; iy++)
            {
                if (passOrNotVec[prio][ix][iy] == 0) nExceptions += 1;
                if (passOrNotVec[prio][ix][iy] == 0 and dir != 4)
                    passOrNotVec[prio][ix][iy] = (int)pow(2,dir);
                else if(passOrNotVec[prio][ix][iy] == 0 and dir == 4)
                    passOrNotVec[prio][ix][iy] = 16-1;
                else
                {
                    if (dir == 4)
                        passOrNotVec[prio][ix][iy] = 15 - passOrNotVec[prio][ix][iy];
                    else
                    {
                        int foo = passOrNotVec[prio][ix][iy]%(int)pow(2,dir+1)-passOrNotVec[prio][ix][iy]%(int)pow(2,dir);
                        foo /= pow(2,dir);
                        passOrNotVec[prio][ix][iy] = passOrNotVec[prio][ix][iy]+(int)pow(2,dir)*pow(-1,foo);
                    }
                }
                if (passOrNotVec[prio][ix][iy] == 0) nExceptions -= 1;
            }
        }
    }
}

void mapi::setJoueur(bool s)
{
    if (s == 1)
        state = heros;
    else
        state = nothing;
}

void mapi::setSizeMap(sf::Vector2u s)
{
    if (saveState != loaded) saveState = edited;
    if (s.x < lxMap)
        for (int i=0; i<nPrio; i++)
            for (int ix=s.x; ix<lxMap; ix++)
                for (int iy=0; iy<lyMap; iy++)
                    removeSprite(i,ix,iy);
    
    if (s.y < lyMap)
        for (int i=0; i<nPrio; i++)
            for (int ix=0; ix<lxMap; ix++)
                for (int iy=s.y; iy<lyMap; iy++)
                    removeSprite(i,ix,iy);
                    
    if (s.x > lxMap)
    {
        if (s.y > lyMap)
        {
            for (int prio=0; prio<nPrio; prio++)
            {
                for (int i=0; i<lxMap; i++)
                {
                    for (int j=lyMap; j<s.y; j++)
                    {
                        indexSpriteVec[prio][i].push_back(vector<int>(2));
                        indexSpriteVec[prio][i][j][0] = -1;
                        indexSpriteVec[prio][i][j][1] = -1;
                    }
                }
                for (int i=lxMap; i<s.x; i++)
                {
                    indexSpriteVec[prio].push_back(vector<vector<int> >(s.y));
                    for (int j=0; j<s.y; j++)
                    {   
                        indexSpriteVec[prio][i][j] = vector<int>(2);
                        indexSpriteVec[prio][i][j][0] = -1;
                        indexSpriteVec[prio][i][j][1] = -1;
                    }
                }
            }
        }
        else
        {
            for (int prio=0; prio<nPrio; prio++)
            {
                for (int i=0; i<lxMap; i++)
                    indexSpriteVec[prio][i].erase(indexSpriteVec[prio][i].begin()+s.y,indexSpriteVec[prio][i].begin()+lyMap);
                for (int i=lxMap; i<s.x; i++)
                {
                    indexSpriteVec[prio].push_back(vector<vector<int> >(s.y));
                    for (int j=0; j<s.y; j++)
                    {   
                        indexSpriteVec[prio][i][j] = vector<int>(2);
                        indexSpriteVec[prio][i][j][0] = -1;
                        indexSpriteVec[prio][i][j][1] = -1;
                    }
                }
            }
        }
    }
    else
    {
        if (s.y > lyMap)
        {
            for (int prio=0; prio<nPrio; prio++)
            {
                for (int i=0; i<s.x; i++)
                {
                    for (int j=lyMap; j<s.y; j++)
                    {
                        indexSpriteVec[prio][i].push_back(vector<int>(2));
                        indexSpriteVec[prio][i][j][0] = -1;
                        indexSpriteVec[prio][i][j][1] = -1;
                    }
                }
                indexSpriteVec[prio].erase(indexSpriteVec[prio].begin()+s.x,indexSpriteVec[prio].begin()+lxMap);
            }
        }
        else
        {
            for (int prio=0; prio<nPrio; prio++)
            {
                for (int i=0; i<lxMap; i++)
                    indexSpriteVec[prio][i].erase(indexSpriteVec[prio][i].begin()+s.y,indexSpriteVec[prio][i].begin()+lyMap);
                indexSpriteVec[prio].erase(indexSpriteVec[prio].begin()+s.x,indexSpriteVec[prio].begin()+lxMap);
            }
        }
    }
                    
    
    for (int prio=0; prio<nPrio; prio++)
    {
        if (s.y > lyMap)
        {
            for (int i=0; i<lxMap; i++)
            {
                for (int j=lyMap; j<s.y; j++)
                {
                    passOrNotVec[prio][i].push_back(0);
                }
            }
        }
        else
        {
            for (int i=0; i<lxMap; i++)
                passOrNotVec[prio][i].erase(passOrNotVec[prio][i].begin()+s.y, passOrNotVec[prio][i].begin()+lyMap);
        }
        if (s.x > lxMap)
        {
            for (int i=lxMap; i<s.x; i++)
            {
                passOrNotVec[prio].push_back(vector<int>(s.y));
                for (int j=0; j<s.y; j++)
                    passOrNotVec[prio][i][j] = 0;
            }
        }
        else
        {
            passOrNotVec[prio].erase(passOrNotVec[prio].begin()+s.x, passOrNotVec[prio].begin()+lxMap);
        }
    }
    lxMap = s.x;
    lyMap = s.y;
    
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineColor(sf::Color::Green);
    
    double thickness = xSprites;
    boundary.setOutlineThickness(thickness);
    
    boundary.setPosition(0,0);
    boundary.setSize(sf::Vector2f(lxMap*xSprites, lyMap*ySprites));
    
    double sizeGrid = 4;
    gridX = vector<sf::RectangleShape>(lxMap-1);
    gridY = vector<sf::RectangleShape>(lyMap-1);
    
    for (int i=1; i<lxMap; i++)
    {
        gridX[i-1] = sf::RectangleShape(sf::Vector2f(sizeGrid, lyMap*ySprites));
        gridX[i-1].setPosition(i*xSprites-sizeGrid/2,0);
        gridX[i-1].setFillColor(sf::Color(128,128,128,100));
    }
    for (int i=1; i<lyMap; i++)
    {
        gridY[i-1] = sf::RectangleShape(sf::Vector2f(lxMap*xSprites,sizeGrid));
        gridY[i-1].setPosition(0,i*ySprites-sizeGrid/2);
        gridY[i-1].setFillColor(sf::Color(128,128,128,100));        
    }
}

void mapi::setLxMap(int lxNew)
{
    if (lxNew > 0){ if (saveState != loaded) saveState = edited; setSizeMap(sf::Vector2u(lxNew,lyMap));}
}

void mapi::setLyMap(int lyNew)
{
    if (lyNew > 0){ if (saveState != loaded) saveState = edited; setSizeMap(sf::Vector2u(lxMap,lyNew));}
}

void mapi::setTakeMidX(bool t)
{
    imL->setTakeMidX(t);
}

void mapi::setTakeMidY(bool t)
{
    imL->setTakeMidY(t);
}

void mapi::setPosHero(int xi, int yi)
{
    Heros->setX(xi);
    Heros->setY(yi);
}

void mapi::setDirHero(int dir)
{
    if (dir > -1 && dir < 4) Heros->setDir(dir);
    else Heros->setDir(0);
}

void mapi::setSelect(bool s, bool s2)
{
    select = s;
    select2 = s2;
}

void mapi::resetTextureSprite()
{
    for (int i=0; i<nPrio; i++)
        for (int j=0; j<nTextureVec[i]; j++)
            for (int k=0; k<nSpriteVec[i][j]; k++)
                spriteVec[i][j][k].setTexture(textureVec[iTextureVec[i][j]]);
}

int mapi::addTexture(string t)
{
    int fooInt = -1;
    for (int i=0; i<nTotTexture; i++)
        if (t == fileTextureVec[i] && fooInt == -1)
            fooInt = i;
            
    if (fooInt == -1)
    {
        textureVec.push_back(sf::Texture());
        textureVec[nTotTexture].loadFromFile(t);
        fileTextureVec.push_back(t);
        nTotTexture += 1;
        return (nTotTexture-1);
    }
    return fooInt;
}

void mapi::removeTexture(int prio, int iT, string t)
{
    if (nTextureVec[prio] > 0)
        iTextureVec[prio].erase(iTextureVec[prio].begin()+iT);
}

int mapi::loadMap()
{
    int conf = 0;
    bool fooLoading = 0;
    if (saveState == edited)
    {
        conf = loadWindow();
        if (conf == 0)
            fooLoading = 1;
        saveState = loaded;
    }
    else fooLoading = 1;
            
    if (fooLoading)
    {
        if (saveState != initialized) reinitMap();
        saveState = saved;
        ifstream file((dirMaps+stringFile).c_str(), ios::in);
        if (file)
        {
            string foo;
            file>>lxMap>>lyMap>>foo;        
            for (int i=0; i<nPrio; i++)
            {
                passOrNotVec[i] = vector<vector<int> >(lxMap);
                for (int j=0; j<lxMap; j++)
                {
                    passOrNotVec[i][j] = vector<int>(lyMap);
                    for (int k=0; k<lyMap; k++)
                        passOrNotVec[i][j][k] = 0;
                }
            }
        
            for (int i=0; i<nPrio; i++)
            {
                indexSpriteVec[i] = vector<vector<vector<int > > >(lxMap);
                for (int j=0; j<lxMap; j++)
                {
                    indexSpriteVec[i][j] = vector<vector<int> >(lyMap);
                    for (int k=0; k<lyMap; k++)
                    {
                        indexSpriteVec[i][j][k] = vector<int>(2);
                        indexSpriteVec[i][j][k][0] = -1;
                        indexSpriteVec[i][j][k][1] = -1;
                    }
                }
            }
            for (int i=0; i<nPrio; i++)
            {    
                file>>foo>>nTextureVec[i]>>foo;
                if (nTextureVec[i] > 0)
                {
                    nSpriteVec[i] = vector<int>(nTextureVec[i]);
                    iTextureVec[i] = vector<int>(nTextureVec[i]);
                    spriteVec[i] = vector<vector<sf::Sprite> >(nTextureVec[i]);
                
                    for (int j=0; j<nTextureVec[i]; j++)
                    {
                        file>>nSpriteVec[i][j];
                        spriteVec[i][j] = vector<sf::Sprite>(nSpriteVec[i][j]);
                    }
                    file>>foo;
                
                    for (int j=0; j<nTextureVec[i]; j++)
                    {
                        file>>foo;
                        iTextureVec[i][j] = addTexture(foo);
                        file>>foo;
                        if (foo[0] != '{')
                        {    
                            cout<<"Invalid syntax for texture "<<fileTextureVec[iTextureVec[i][j]]<<" at prio "<<i<<endl;
                            return 0;
                        }
                        sprite s;
                        for (int k=0; k<nSpriteVec[i][j]; k++)
                        {
                            file>>s.x>>s.y>>s.xPNG>>s.yPNG>>foo;
                            spriteVec[i][j][k].setTexture(textureVec[iTextureVec[i][j]]);
                            spriteVec[i][j][k].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
                            spriteVec[i][j][k].setPosition(s.x,s.y);        
                            if (s.x>=0 && s.x<lxMap*xSprites && s.y>=0 && s.y<lyMap*ySprites)
                            {
                                indexSpriteVec[i][s.x/xSprites][s.y/ySprites][0] = j;
                                indexSpriteVec[i][s.x/xSprites][s.y/ySprites][1] = k;
                            }
                        }
                    }
                }
            }
            resetTextureSprite();
        
            int fooPrio = 0, fooDir = 0, fooX = 0, fooY = 0;
            file>>foo>>nExceptions>>foo;
            for (int i=0; i<nExceptions; i++)
            {
                file>>fooPrio>>fooX>>fooY>>fooDir;
                passOrNotVec[fooPrio][fooX][fooY] = fooDir;            
            }
            if (nTotTexture > 0) initPNG(fileTextureVec[0],'L');
            else initPNG("Tileset/base.PNG",'L');
            file>>nEvents>>foo;
            if (nEvents > 0) events = vector<gameEvent*>(nEvents);
            for (int i=0; i<nEvents; i++)
            {
                file>>foo;
                if (foo == "changeMap:")
                {
                    int fooXNew, fooYNew, fooDirNew;
                    file>>fooX>>fooY>>fooDir>>foo>>fooXNew>>fooYNew>>fooDirNew;
                    events[i] = new changeMap(fooX, fooY, fooDir, this, window, foo, fooXNew, fooYNew, fooDirNew);
                    //cout<<events[i]->x<<" "<<events[i]->y<<" "<<events[i]->dir<<" "<<events[i]->nameMap<<" "<<events[i]->xNew<<" "<<events[i]->yNew<<" "<<events[i]->dirNew<<endl;
                }
                else if (foo == "textInteraction:")
                {
                    file>>fooX>>fooY>>fooDir>>foo;
                    events[i] = new textInteraction(fooX, fooY, fooDir, foo, this, window);
                }
                else
                    events[i] = NULL;
            }
            file.close();
            initMap();
            return 1;   
        }
        else cout<<"Unable to open file! "<<endl;
        return 0;
    }
    return 0;
}

void mapi::saveMap()
{
    ofstream file((dirMaps+stringFile).c_str(), ios::out);
    int confirmation = 0;
    if (saveState == loaded) confirmation = saveWindow();
    if (confirmation == 0)
    {    
        saveState = saved;
        file<<lxMap<<" "<<lyMap<<" "<<"#dimensions\n\n";
        sf::Vector2f fooPos;
        sf::Vector2f fooPNG;
        sf::IntRect fooRect;
        for (int i=0; i<nPrio; i++)
        {
            file<<"Prio"<<i<<endl<<endl;
            file<<"\t"<<nTextureVec[i]<<" #nTextureVec\n\t";
            for (int j=0; j<nTextureVec[i]; j++)
                file<<nSpriteVec[i][j]<<" ";
            if (nTextureVec[i] > 0) file<<"#nSpriteVec\n";
            for (int j=0; j<nTextureVec[i]; j++)
            {
                file<<"\t"<<fileTextureVec[iTextureVec[i][j]]<<" { ";
                for (int k=0; k<nSpriteVec[i][j]; k++)
                {
                    fooPos = spriteVec[i][j][k].getPosition();
                    fooRect = spriteVec[i][j][k].getTextureRect();
                    fooPNG = sf::Vector2f(fooRect.left, fooRect.top);
                    file<<fooPos.x<<" "<<fooPos.y<<" ";
                    file<<fooPNG.x<<" "<<fooPNG.y<<" ";
                    if (k == nSpriteVec[i][j]-1)
                        file<<"}\n";
                    else
                        file<<", ";
                }
            }
            file<<"\n\n";
        }
        file<<"passOrNotVec\n\n"<<nExceptions<<" #nExceptions\n\t";
        for (int i=0; i<nPrio; i++)
        {
            for (int j=0; j<lxMap; j++)
                for (int k=0; k<lyMap; k++)
                    if (passOrNotVec[i][j][k] != 0)
                        file<<i<<" "<<j<<" "<<k<<" "<<passOrNotVec[i][j][k]<<"  ";
                    
            if (i == nPrio-1)
                file<<"\n\n";
        }
        file<<nEvents<<" #nEvents\n\n";
        for (int i=0; i<nEvents; i++)
        {
            if (events[i]->type == "changeMap") file<<"changeMap: " <<events[i]->x<<" "<<events[i]->y<<" "<<events[i]->dir<<" "<<events[i]->nameMap<<" "<<events[i]->xNew<<" "<<events[i]->yNew<<" "<<events[i]->dirNew<<" ";
            else if (events[i]->type == "textInteraction") file<<"textInteraction: "<<events[i]->x<<" "<<events[i]->y<<" "<<events[i]->dir<<" "<<events[i]->stringFile<<" ";
            else file<<"None ";
        }
        file<<endl<<endl;
        file<<"endFile";
        file.close();
        
        ofstream file("Maps/dataMap.txt",ios::out);
        file<<stringFile<<"\n"<<stringDirPNG<<"\n";
        file.close();
    }
}

void mapi::reinitMap()
{
    int conf = 0;
    if (saveState == edited) conf = reinitWindow();
    if (conf != 2)
    {
        if (conf == 1) saveMap();
        if (saveState != loaded) saveState = edited;
        freeSpritesCtrlC();
        nTotTexture = 0;
        textureVec.clear();
        nTextureVec.clear();
        iTextureVec.clear();
        indexSpriteVec.clear();
        fileTextureVec.clear();
        nSpriteVec.clear();
        passOrNotVec.clear();
        spriteVec.clear();
        passOrNotVec = vector<vector<vector<int> > >(4);
        nSpriteVec = vector<vector<int> >(4);
        nTextureVec = vector<int>(4);
        for (int i=0; i<nPrio; i++)
            nTextureVec[i] = 0;
        iTextureVec = vector<vector<int> >(4);
        indexSpriteVec = vector<vector<vector<vector<int> > > >(4);
        spriteVec = vector<vector<vector<sf::Sprite> > >(4);
        boundary.setFillColor(sf::Color::Transparent);
        boundary.setOutlineColor(sf::Color::Green);
    
        double thickness = xSprites;
        boundary.setOutlineThickness(thickness);
    
        boundary.setPosition(0,0);
        boundary.setSize(sf::Vector2f(lxMap*xSprites, lyMap*ySprites));
    
        viewMap.reset(sf::FloatRect(0,0,lx,ly));
        mapWindow.setView(viewMap);
        
        double sizeGrid = 4;
        gridX.clear();
        gridY.clear();
        gridX = vector<sf::RectangleShape>(lxMap-1);
        gridY = vector<sf::RectangleShape>(lyMap-1);
        
        for (int i=1; i<lxMap; i++)
        {
            gridX[i-1] = sf::RectangleShape(sf::Vector2f(sizeGrid, lyMap*ySprites));
            gridX[i-1].setPosition(i*xSprites-sizeGrid/2,0);
            gridX[i-1].setFillColor(sf::Color(128,128,128,100));
        }
        for (int i=1; i<lyMap; i++)
        {
            gridY[i-1] = sf::RectangleShape(sf::Vector2f(lxMap*xSprites,sizeGrid));
            gridY[i-1].setPosition(0,i*ySprites-sizeGrid/2);
            gridY[i-1].setFillColor(sf::Color(128,128,128,100));
        }
        for (int i=0; i<nPrio; i++)
        {
            passOrNotVec[i] = vector<vector<int > >(lxMap);
            for (int j=0; j<lxMap; j++)
            {
                passOrNotVec[i][j] = vector<int>(lyMap);
                for (int k=0; k<lyMap; k++)
                    passOrNotVec[i][j][k] = 0;
            }
        }
        for (int i=0; i<nPrio; i++)
        {
            indexSpriteVec[i] = vector<vector<vector<int > > >(lxMap);
            for (int j=0; j<lxMap; j++)
            {
                indexSpriteVec[i][j] = vector<vector<int> >(lyMap);
                for (int k=0; k<lyMap; k++)
                {
                    indexSpriteVec[i][j][k] = vector<int>(2);
                    indexSpriteVec[i][j][k][0] = -1;
                    indexSpriteVec[i][j][k][1] = -1;
                }
            }
        }
    }
}

sf::Vector2f mapi::convertPos(sf::Vector2i p)
{
    sf::Vector2f foo = viewMap.getCenter();
    sf::Vector2f foo2 = viewMap.getSize();
    
    double xTranslate = foo.x - sizeWindow.x/2;
    double yTranslate = foo.y - (sizeWindow.y - ly/2);
    
    double ratioX = foo2.x/lx;
    double ratioY = foo2.y/ly;
    
    foo2.x = p.x + xTranslate;
    foo2.y = p.y + yTranslate;
    
    foo2.x = (foo2.x-foo.x)*ratioX + foo.x;
    foo2.y = (foo2.y-foo.y)*ratioY + foo.y;
    
    return foo2;
}

sf::Vector2i mapi::invConvertPos(sf::Vector2f p)
{
    sf::Vector2f foo = viewMap.getCenter();
    sf::Vector2f foo2 = viewMap.getSize();
    sf::Vector2i foo3;
    
    double ratioX = lx/foo2.x;
    double ratioY = ly/foo2.y;
    
    foo3.x = p.x - foo.x;
    foo3.y = p.y - foo.y;
    
    foo3.x = foo3.x*ratioX + sizeWindow.x/2;
    foo3.y = foo3.y*ratioY + (sizeWindow.y-ly/2);
    
    return foo3;
}

void mapi::initMap()
{
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineColor(sf::Color::Green);
    
    double thickness = xSprites;
    boundary.setOutlineThickness(thickness);
    
    boundary.setPosition(0,0);
    boundary.setSize(sf::Vector2f(lxMap*xSprites, lyMap*ySprites));
    
    viewMap.reset(sf::FloatRect(0,0,lx,ly));
    mapWindow.setView(viewMap);
    
    double sizeGrid = 4;
    gridX.clear();
    gridY.clear();
    gridX = vector<sf::RectangleShape>(lxMap-1);
    gridY = vector<sf::RectangleShape>(lyMap-1);
    
    for (int i=1; i<lxMap; i++)
    {
        gridX[i-1] = sf::RectangleShape(sf::Vector2f(sizeGrid, lyMap*ySprites));
        gridX[i-1].setPosition(i*xSprites-sizeGrid/2,0);
        gridX[i-1].setFillColor(sf::Color(128,128,128,100));
    }
    for (int i=1; i<lyMap; i++)
    {
        gridY[i-1] = sf::RectangleShape(sf::Vector2f(lxMap*xSprites,sizeGrid));
        gridY[i-1].setPosition(0,i*ySprites-sizeGrid/2);
        gridY[i-1].setFillColor(sf::Color(128,128,128,100));
    }
}

void mapi::addSprite(sprite s, int prio, string t)
{
    if (prio < 0 || prio > 3)        
        cout<<"Invalid priority !\n";
    else
    {
        if (saveState != loaded) saveState = edited;
        bool toErase = 0;
        for (int iPrio=prio; iPrio<4; iPrio++)
        {
            toErase = (toErase || indexSpriteVec[iPrio][s.x/xSprites][s.y/ySprites][0]!=-1);
            toErase = (toErase || indexSpriteVec[iPrio][s.x/xSprites][s.y/ySprites][0]!=-1);
            if (toErase)
                removeSprite(prio, s.x/xSprites, s.y/ySprites);
            toErase = 0;
        }
        int indexTexture = -1;
        for (int j=0; j<nTextureVec[prio]; j++)
        {
            if (t == fileTextureVec[iTextureVec[prio][j]])
                indexTexture = j;
        }
        if (indexTexture == -1)
        {
            iTextureVec[prio].push_back(addTexture(t));
            nSpriteVec[prio].push_back(1);
            spriteVec[prio].push_back(vector<sf::Sprite>(1));
            spriteVec[prio][nTextureVec[prio]] = vector<sf::Sprite>(1);
            spriteVec[prio][nTextureVec[prio]][0].setTexture(textureVec[iTextureVec[prio][nTextureVec[prio]]]);
            spriteVec[prio][nTextureVec[prio]][0].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
            spriteVec[prio][nTextureVec[prio]][0].setPosition(s.x,s.y);
            
            
            indexSpriteVec[prio][s.x/xSprites][s.y/ySprites][0] = nTextureVec[prio];
            indexSpriteVec[prio][s.x/xSprites][s.y/ySprites][1] = 0;
            nTextureVec[prio] += 1;
        }
        else
        {
            sf::Sprite* foo = new sf::Sprite[nSpriteVec[prio][indexTexture]+1];
            for (int k=0; k<nSpriteVec[prio][indexTexture]; k++)
                foo[k] = spriteVec[prio][indexTexture][k];
            
            foo[nSpriteVec[prio][indexTexture]].setTexture(textureVec[iTextureVec[prio][indexTexture]]);
            foo[nSpriteVec[prio][indexTexture]].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
            foo[nSpriteVec[prio][indexTexture]].setPosition(s.x,s.y);
            spriteVec[prio][indexTexture].push_back(foo[nSpriteVec[prio][indexTexture]]);
            
            indexSpriteVec[prio][s.x/xSprites][s.y/ySprites][0] = indexTexture;
            indexSpriteVec[prio][s.x/xSprites][s.y/ySprites][1] = nSpriteVec[prio][indexTexture];
            nSpriteVec[prio][indexTexture] += 1;
            
            delete[] foo;
            foo = 0;
        }
    }
    resetTextureSprite();
}

void mapi::addSprite2(sprite s, int prio, string t, int nxS, int nyS, int lxS, int lyS)
{
    if (prio < 0 || prio > 3)
        cout<<"Invalid priority !\n";
    else if (s.x < 0 or s.x/xSprites >= lxMap or s.y < 0 or s.y/ySprites >= lyMap)
        cout<<"Outside the map!\n";
    else
    {
        if (saveState != loaded) saveState = edited;
        sprite fooSprite;
        sf::IntRect fooRect;
        string fooTexture;
        for (int ix=s.x/xSprites; ix<s.x/xSprites+lxS; ix++)
        {
            for (int iy=s.y/ySprites; iy<s.y/ySprites+lyS; iy++)
            {
                if (indexSpriteVec[prio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = spriteVec[prio][indexSpriteVec[prio][ix][iy][0]][indexSpriteVec[prio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextureVec[iTextureVec[prio][indexSpriteVec[prio][ix][iy][0]]];
                    ctrlZObject->saveErasing(fooSprite, prio, passOrNotVec[prio][ix][iy], fooTexture);
                }
                removeSprite(prio, ix, iy);
            }
        }
        ctrlZObject->saveAdding(s, prio, lxS, lyS);
        int indexTexture = -1;
        for (int j=0; j<nTextureVec[prio]; j++)
        {
            if (t == fileTextureVec[iTextureVec[prio][j]])
                indexTexture = j;
        }
        if (indexTexture == -1)
        {
            sf::Sprite** foo3 = new sf::Sprite*[nTextureVec[prio]+1];
            for (int j=0; j<nTextureVec[prio]; j++)
            {
                foo3[j] = new sf::Sprite[nSpriteVec[prio][j]];
                for (int k=0; k<nSpriteVec[prio][j]; k++)
                    foo3[j][k] = spriteVec[prio][j][k];
            }
            iTextureVec[prio].push_back(addTexture(t));
            nSpriteVec[prio].push_back(lxS*lyS);
            foo3[nTextureVec[prio]] = new sf::Sprite[lxS*lyS];
            sprite fooS = s;
            spriteVec[prio].push_back(vector<sf::Sprite>(0));
            for (int ix=0; ix<lxS; ix++)
            {
                for (int iy=0; iy<lyS; iy++)
                {
                    fooS.x = s.x + ix*xSprites;
                    fooS.y = s.y + iy*ySprites;
                    fooS.xPNG = s.xPNG + (ix%nxS)*xSprites;
                    fooS.yPNG = s.yPNG + (iy%nyS)*ySprites;
                    foo3[nTextureVec[prio]][ix*lyS+iy].setTextureRect(sf::IntRect(fooS.xPNG,fooS.yPNG,xSprites,ySprites));
                    foo3[nTextureVec[prio]][ix*lyS+iy].setPosition(fooS.x,fooS.y);
                    spriteVec[prio][nTextureVec[prio]].push_back(foo3[nTextureVec[prio]][ix*lyS+iy]);
                    indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = nTextureVec[prio];
                    indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = ix*lyS+iy;
                }
            }
            
            nTextureVec[prio] += 1;
            
            for (int j=0; j<nTextureVec[prio]; j++)
            {
                delete[] foo3[j];
                foo3[j] = 0;
            }
            delete[] foo3;
            foo3 = 0;
        }
        else
        {
            sf::Sprite* foo = new sf::Sprite[nSpriteVec[prio][indexTexture]+lxS*lyS];
            for (int k=0; k<nSpriteVec[prio][indexTexture]; k++)
                foo[k] = spriteVec[prio][indexTexture][k];
            
            for (int ix=0; ix<lxS; ix++)
            {
                for (int iy=0; iy<lyS; iy++)
                {
                    foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setTexture(textureVec[iTextureVec[prio][indexTexture]]);
                    foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setTextureRect(sf::IntRect(s.xPNG+ (ix%nxS)*xSprites,s.yPNG + (iy%nyS)*ySprites,xSprites,ySprites));
                    foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setPosition(s.x + ix*xSprites,s.y + iy*ySprites);
                    spriteVec[prio][indexTexture].push_back(foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy]);
                    indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = indexTexture;
                    indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = nSpriteVec[prio][indexTexture]+ix*lyS+iy;
                }
            }
            nSpriteVec[prio][indexTexture] += lxS*lyS;
                
            delete[] foo;
            foo = 0;
        }
        resetTextureSprite();
    }
}

void mapi::removeSprite(int prio, int ix, int iy)
{
    if (ix>=0 && ix<lxMap && iy>=0 && iy<lyMap)
    {
        if (saveState != loaded) saveState = edited;
        int iT = indexSpriteVec[prio][ix][iy][0];
        int iS = indexSpriteVec[prio][ix][iy][1];
        if (iT!= -1 && iS != -1)
        {
            int nT = nTextureVec[prio];
            int nS = nSpriteVec[prio][iT];
            if (nS == 1)
            {
                removeTexture(prio, iT, fileTextureVec[iTextureVec[prio][iT]]);
                if (nT == 1)
                {
                    spriteVec[prio].clear();
                    iTextureVec[prio].clear();
                    nTextureVec[prio] = 0;
                    nSpriteVec[prio].clear();
                }
                else
                {
                    sf::Vector2f fooPos;
                    for (int i=iT+1; i<nT; i++)
                    {
                        for (int j=0; j<nSpriteVec[prio][i]; j++)
                        {
                            fooPos = spriteVec[prio][i][j].getPosition();
                            indexSpriteVec[prio][(int)fooPos.x/xSprites][(int)fooPos.y/ySprites][0] -= 1;
                        }
                    }
                    spriteVec[prio][iT].clear();
                    spriteVec[prio].erase(spriteVec[prio].begin()+iT);
                    nSpriteVec[prio].erase(nSpriteVec[prio].begin()+iT);
                    nTextureVec[prio] -= 1;
                    nT = nTextureVec[prio];
                }
            }
            else
            {
                sf::Vector2f fooPos;
                for (int i=iS+1; i<nS; i++)
                {
                    fooPos = spriteVec[prio][iT][i].getPosition();
                    indexSpriteVec[prio][(int)fooPos.x/xSprites][(int)fooPos.y/ySprites][1] -= 1;
                }
                spriteVec[prio][iT].erase(spriteVec[prio][iT].begin()+iS);
                
                
                nSpriteVec[prio][iT] -= 1;
                nS = nSpriteVec[prio][iT];
            }
        }
        indexSpriteVec[prio][ix][iy][0] = -1;
        indexSpriteVec[prio][ix][iy][1] = -1;    
        if (passOrNotVec[prio][ix][iy] != 0)
        {
            nExceptions -= 1;
            passOrNotVec[prio][ix][iy] = 0;    
        }
    }
    resetTextureSprite();
}

void mapi::freeSpritesCtrlC()
{
    spriteCtrlCVec.clear();
    fileCtrlCVec.clear();
    nxCtrlC = nyCtrlC = 0;
    ghostSpriteCtrlC = 0;
}

void mapi::ctrlC()
{
    if (select && select2 && state==selecting)
    {
        spriteCtrlCVec.clear();
        fileCtrlCVec.clear();
        spriteCtrlCVec = vector<vector<vector<sf::Sprite > > >(4);
        fileCtrlCVec = vector<vector<vector<string> > >(4);
        
        sf::Vector2f fooPos = selectRect.getPosition();
        sf::Vector2f fooSize = selectRect.getSize();
        
        xCtrlC = fooPos.x;
        yCtrlC = fooPos.y;
        nxCtrlC = round(fooSize.x/xSprites);
        nyCtrlC = round(fooSize.y/ySprites);
        
        if (isImLeft) imL->freeSprites();
        
        if (!allPrio)
        {
            prioCtrlC = 4;
            for (int prio=0; prio<nPrio; prio++)
            {
                spriteCtrlCVec[prio] = vector<vector<sf::Sprite> >(nxCtrlC);
                fileCtrlCVec[prio] = vector<vector<string> >(nxCtrlC);
                sf::Sprite s;
                int iT, iS;
                for (int i=0; i<nxCtrlC; i++)
                {
                    spriteCtrlCVec[prio][i] = vector<sf::Sprite>(nyCtrlC);
                    fileCtrlCVec[prio][i] = vector<string>(nyCtrlC);
                    for (int j=0; j<nyCtrlC; j++)
                    {
                        iT = indexSpriteVec[prio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][0];
                        iS = indexSpriteVec[prio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][1];
                        if (iT != -1 && iS != -1)
                        {
                            s = spriteVec[prio][iT][iS];
                            spriteCtrlCVec[prio][i][j].setTexture(textureVec[iTextureVec[prio][iT]]);
                            spriteCtrlCVec[prio][i][j].setTextureRect(s.getTextureRect());
                            spriteCtrlCVec[prio][i][j].setColor(sf::Color(255,255,255,128));
                            fileCtrlCVec[prio][i][j] = fileTextureVec[iTextureVec[prio][iT]];
                        }
                        else fileCtrlCVec[prio][i][j] = "No File";
                    }
                }
            }
        }
        else
        {
            prioCtrlC = currentPrio;
            spriteCtrlCVec[currentPrio] = vector<vector<sf::Sprite> >(nxCtrlC);
            fileCtrlCVec[currentPrio] = vector<vector<string> >(nxCtrlC);
            sf::Sprite s;
            int iT, iS;
            for (int i=0; i<nxCtrlC; i++)
            {
                spriteCtrlCVec[currentPrio][i] = vector<sf::Sprite>(nyCtrlC);
                fileCtrlCVec[currentPrio][i] = vector<string>(nyCtrlC);
                for (int j=0; j<nyCtrlC; j++)
                {
                    iT = indexSpriteVec[currentPrio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][0];
                    iS = indexSpriteVec[currentPrio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][1];
                    if (iT != -1 && iS != -1)
                    {
                        s = spriteVec[currentPrio][iT][iS];
                        spriteCtrlCVec[currentPrio][i][j].setTexture(textureVec[iTextureVec[currentPrio][iT]]);
                        spriteCtrlCVec[currentPrio][i][j].setTextureRect(s.getTextureRect());
                        spriteCtrlCVec[currentPrio][i][j].setColor(sf::Color(255,255,255,128));
                        fileCtrlCVec[currentPrio][i][j] = fileTextureVec[iTextureVec[currentPrio][iT]];
                    }
                    else fileCtrlCVec[currentPrio][i][j] = "No File";
                }
            }
        }
        
        ghostSpriteCtrlC = 1;
        ghostSprite = 0;
    }
}

void mapi::ctrlV()
{
    if (saveState != loaded) saveState = edited;
    sprite s;
    sf::Vector2f fooPos = selectRect.getPosition();
    sf::Vector2f fooSize = selectRect.getSize();
    int nX = round(fooSize.x/xSprites);
    int nY = round(fooSize.y/ySprites);
    int xmin = round(fooPos.x/xSprites);
    int ymin = round(fooPos.y/ySprites);
    sf::IntRect fooRect;
    sprite fooSprite;
    string fooTexture;
    int ix,iy;
    if (!allPrio || prioCtrlC == 4)
    {
        for (int prio=0; prio<nPrio; prio++)
        {
            for (int i=0; i<nX; i++)
            {
                for (int j=0; j<nY; j++)
                {
                    ix = xmin+i;
                    iy = ymin+j;
                    if (indexSpriteVec[prio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = spriteVec[prio][indexSpriteVec[prio][ix][iy][0]][indexSpriteVec[prio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextureVec[iTextureVec[prio][indexSpriteVec[prio][ix][iy][0]]];
        
                        ctrlZObject->saveErasing(fooSprite, prio, passOrNotVec[prio][ix][iy], fooTexture);
                    }
                    removeSprite(prio, ix, iy);
                    removeSprite(prio, xmin+i, ymin+j);
                }
            }
        }
    }
    else
    {
        for (int i=0; i<nX; i++)
        {
            for (int j=0; j<nY; j++)
            {
                ix = xmin+i;
                iy = ymin+j;
                if (indexSpriteVec[currentPrio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = spriteVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]][indexSpriteVec[currentPrio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextureVec[iTextureVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]]];
    
                    ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNotVec[currentPrio][ix][iy], fooTexture);
                }
                removeSprite(currentPrio, ix, iy);
                removeSprite(currentPrio, xmin+i, ymin+j);
            }
        }
    }
    if (prioCtrlC == 4)
    {
        for (int prio=0; prio<nPrio; prio++)
        {
            s.x = fooPos.x;
            s.y = fooPos.y;
            ctrlZObject->saveAdding(s, prio, nX, nY);
            for (int i=0; i<nX; i++)
            {
                for (int j=0; j<nY; j++)
                {
                    if (fileCtrlCVec[prio][i%nxCtrlC][j%nyCtrlC] != "No File")
                    {
                        s.x = fooPos.x + i*xSprites;
                        s.y = fooPos.y + j*ySprites;
                        fooRect = spriteCtrlCVec[prio][i%nxCtrlC][j%nyCtrlC].getTextureRect();
                        s.xPNG = fooRect.left;
                        s.yPNG = fooRect.top;
                        addSprite(s, prio, fileCtrlCVec[prio][i%nxCtrlC][j%nyCtrlC]);
                    }
                }
            }
        }
    }
    else
    {
        ctrlZObject->saveAdding(s, currentPrio, nX, nY);
        for (int i=0; i<nX; i++)
        {
            for (int j=0; j<nY; j++)
            {
                if (fileCtrlCVec[prioCtrlC][i%nxCtrlC][j%nyCtrlC] != "No File")
                {
                    s.x = fooPos.x + i*xSprites;
                    s.y = fooPos.y + j*ySprites;
                    fooRect = spriteCtrlCVec[prioCtrlC][i%nxCtrlC][j%nyCtrlC].getTextureRect();
                    s.xPNG = fooRect.left;
                    s.yPNG = fooRect.top;
                    addSprite(s, currentPrio, fileCtrlCVec[prioCtrlC][i%nxCtrlC][j%nyCtrlC]);
                }
            }
        }
    }
}

void mapi::ctrlX()
{
    ctrlC();
    suppr();
}

void mapi::ctrlZ()
{
    ctrlZObject->CtrlZ();
}

void mapi::suppr()
{
    if (state == selecting)
    {
        if (saveState != loaded) saveState = edited;
        sf::Vector2f foo = selectRect.getPosition();
        sf::Vector2f foo2 = selectRect.getSize();
    
        int nxFoo = floor(foo2.x/xSprites);
        int nyFoo = floor(foo2.y/ySprites);
    
        int xFoo = floor(foo.x/xSprites);
        int yFoo = floor(foo.y/ySprites);
        
        sprite fooSprite;
        string fooTexture;
        sf::IntRect fooRect;
        int ix, iy;
        if (!allPrio)
        {    
            for (int prio=0; prio<nPrio; prio++)
            {
                for (int i=0; i<nxFoo; i++)
                {
                    for (int j=0; j<nyFoo; j++)
                    {
                        ix = xFoo+i;
                        iy = yFoo+j;
                        if (indexSpriteVec[prio][ix][iy][0] != -1)
                        {
                            fooSprite.x = ix*xSprites;
                            fooSprite.y = iy*ySprites;
                            fooRect = spriteVec[prio][indexSpriteVec[prio][ix][iy][0]][indexSpriteVec[prio][ix][iy][1]].getTextureRect();
                            fooSprite.xPNG = fooRect.left;
                            fooSprite.yPNG = fooRect.top;
                            fooTexture = fileTextureVec[iTextureVec[prio][indexSpriteVec[prio][ix][iy][0]]];
            
                            ctrlZObject->saveErasing(fooSprite, prio, passOrNotVec[prio][ix][iy], fooTexture);
                        }
                        removeSprite(prio, ix, iy);
                    }
                }
            }
        }
        else
        {
            for (int i=0; i<nxFoo; i++)
            {
                for (int j=0; j<nyFoo; j++)
                {
                    ix = xFoo+i;
                    iy = yFoo+j;                
                    if (indexSpriteVec[currentPrio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = spriteVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]][indexSpriteVec[currentPrio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextureVec[iTextureVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]]];
                
                        ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNotVec[currentPrio][ix][iy], fooTexture);
                    }
                    removeSprite(currentPrio, ix, iy);
                }
            }                    
        }
    }
}
    
int mapi::testMouse(sf::Vector2i p)
{
    int foo1 = 0;
    int foo2 = 0;
    posMouse = p;
    posMouse.y = max(y,posMouse.y);
    if (isImLeft)
        foo1 = imL->imagePNG::testMouse(posMouse);
    isMouseHere = (!foo1 && !foo2 && (posMouse.y > sizeWindow.y-ly));
    
    if (!isMouseHere)
    {
        select = 0;
        select2 = 0;
    }
    
    return isMouseHere;
}

void mapi::mousePressed(sf::Vector2i p)
{
    posMouse = p;
    posMouse.y = max(y,posMouse.y);
    if (isImLeft)
        imL->imagePNG::mousePressed(posMouse);
    
    if (testMouse(posMouse))
    {
        isMousePressed = 1;
        if (!click)
        {
            click = 1;
            posClick = posMouse;
            sf::Vector2f foo = convertPos(posClick);
            foo.x = floor(foo.x/xSprites)*xSprites;
            foo.y = floor(foo.y/ySprites)*ySprites;
            posClick = invConvertPos(foo);
            oldPosMouse = posMouse;
        }
    }
    else ghostSprite = 0;
}

void mapi::mouseReleased()
{
    if (isImLeft)
        imL->imagePNG::mouseReleased();
    isMousePressed = 0;
    click = 0;
}

void mapi::mouseWheel(int d)
{
    if (isImLeft)
        imL->imagePNG::mouseWheel(d);
    if (isMouseHere) delta += d;
    else delta = 0;
}

void mapi::windowResized(sf::Vector2u newSizeWindow)
{
    lx = (int)newSizeWindow.x;
    ly = (int)newSizeWindow.y-y;
    
    if (isImLeft)
    {
        imL->imagePNG::windowResized(newSizeWindow);
        limL.setSize(sf::Vector2f(sizeLim,ly));
    }
        
    double ratioX = newSizeWindow.x*1./sizeWindow.x;
    double ratioY = (newSizeWindow.y*1.-y)/((int)sizeWindow.y-y);
    
    
    sf::Vector2f foo = viewMap.getSize();
    foo.x *= ratioX;
    foo.y *= ratioY;
    viewMap.setSize(foo);
    mapWindow.create(lx,ly);
    mapWindow.setView(viewMap);
    toDraw.setTextureRect(sf::IntRect(0,0,lx,ly));
    sizeWindow = newSizeWindow;
}

void mapi::keyPressed(sf::Keyboard::Key k)
{
    if (state == selecting && select2)
    {
        sf::Vector2f fooPos = selectRect.getPosition();
        sf::Vector2f fooSize = selectRect.getSize();
        
        int xmin = fooPos.x/xSprites;
        int ymin = fooPos.y/ySprites;
        int xmax = fooSize.x/xSprites + xmin;
        int ymax = fooSize.y/ySprites + ymin;
        int dir = 0;
        if (k == sf::Keyboard::Down) dir = 0;
        else if (k == sf::Keyboard::Left) dir = 1;
        else if (k == sf::Keyboard::Right) dir = 2;
        else if (k == sf::Keyboard::Up) dir = 3;
        else if (k == sf::Keyboard::A) dir = 4;
        else dir = -1;
        
        if (dir != -1)
        {
            if (saveState != loaded) saveState = edited;
            ctrlZObject->saveChangingPass(currentPrio, dir, xmin, ymin, xmax-xmin, ymax-ymin);
            for (int ix=xmin; ix<xmax; ix++)
            {
                for (int iy=ymin; iy<ymax; iy++)
                {
                    if (passOrNotVec[currentPrio][ix][iy] == 0) nExceptions += 1;
                    if (passOrNotVec[currentPrio][ix][iy] == 0 and dir != 4)
                        passOrNotVec[currentPrio][ix][iy] = (int)pow(2,dir);
                    else if(passOrNotVec[currentPrio][ix][iy] == 0 and dir == 4)
                        passOrNotVec[currentPrio][ix][iy] = 16-1;
                    else
                    {
                        if (dir == 4)
                            passOrNotVec[currentPrio][ix][iy] = 15 - passOrNotVec[currentPrio][ix][iy];
                        else
                        {
                            int foo = passOrNotVec[currentPrio][ix][iy]%(int)pow(2,dir+1)-passOrNotVec[currentPrio][ix][iy]%(int)pow(2,dir);
                            foo /= pow(2,dir);
                            passOrNotVec[currentPrio][ix][iy] = passOrNotVec[currentPrio][ix][iy]+(int)pow(2,dir)*pow(-1,foo);
                        }
                    }
                    if (passOrNotVec[currentPrio][ix][iy] == 0) nExceptions -= 1;
                }
            }
        }
    }
}

void mapi::setHerosPos(sf::Vector2i p)
{
    sf::Vector2f fooPos = convertPos(p);
    fooPos.x = floor(fooPos.x/xSprites)*xSprites;
    fooPos.y = floor(fooPos.y/ySprites)*ySprites;
    
    fooPos.x = max(0,(int)fooPos.x);
    fooPos.x = min((lxMap-1)*xSprites, (int)fooPos.x);
    fooPos.y = max(0,(int)fooPos.y);
    fooPos.y = min((lyMap-1)*ySprites, (int)fooPos.y);
    
    Heros->setX(fooPos.x);
    Heros->setY(fooPos.y);
}

//Down Left Right Up
bool mapi::testDir(int dir)
{
    double xHeros = Heros->getX()/xSprites;
    double yHeros = Heros->getY()/ySprites;
    if (!Heros->isOnGrid())
    {
        int dirHeros = Heros->getDir();
        switch(dirHeros)
        {
            case 0:
            xHeros = round(xHeros);
            yHeros = floor(yHeros);
            break;
            
            case 1:
            xHeros = floor(xHeros);
            yHeros = round(yHeros);
            break;
            
            case 2:
            xHeros = ceil(xHeros);
            yHeros = round(yHeros);
            break; 
            
            case 3:
            xHeros = round(xHeros);
            yHeros = ceil(yHeros);
            break;
        }
    }
    bool foo = 1;
    int ix = (int)xHeros;
    int iy = (int)yHeros;
    switch(dir)
    {
        case 0:
        if (iy+1 >= lyMap) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNotVec[prio][ix][iy]%2 == 0);
            foo = foo && (passOrNotVec[prio][ix][iy+1]-passOrNotVec[prio][ix][iy+1]%8 == 0);
        }
        break;
        
        case 1:
        if (ix-1 < 0) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNotVec[prio][ix][iy]%4-passOrNotVec[prio][ix][iy]%2 == 0);
            foo = foo && (passOrNotVec[prio][ix-1][iy]%8-passOrNotVec[prio][ix-1][iy]%4 == 0);
        }
        break;
        
        case 2:
        if (ix+1 >= lxMap) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNotVec[prio][ix][iy]%8-passOrNotVec[prio][ix][iy]%4 == 0);
            foo = foo && (passOrNotVec[prio][ix+1][iy]%4-passOrNotVec[prio][ix+1][iy]%2 == 0);
        }
        break;
        
        case 3:
        if (iy-1 < 0) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNotVec[prio][ix][iy]-passOrNotVec[prio][ix][iy]%8 == 0);
            foo = foo && (passOrNotVec[prio][ix][iy-1]%2 == 0);
        }
        break;    
    }
    
    return foo;
}
bool mapi::downOK()
{
    int ix, iy;
    double xHeros = Heros->getX();
    double yHeros = Heros->getY();
    int dirHeros = Heros->getDir();
    
    if (dirHeros == 0)
    {
        iy = ceil(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    else if (dirHeros == 1)
    {
        iy = round(yHeros/ySprites);
        ix = floor(xHeros/xSprites);
    }
    else if (dirHeros == 2)
    {
        iy = round(yHeros/ySprites);
        ix = ceil(xHeros/xSprites);
    }
    else if (dirHeros == 3)
    {
        iy = floor(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    int dirMiror = 0;
    if (dirHeros == 0) dirMiror = 3;
    else if (dirHeros == 1) dirMiror = 2;
    else if (dirHeros == 2) dirMiror = 1;
    else if (dirHeros == 3) dirMiror = 0;
    if (iy+1 >= lyMap)
        return 0;
    bool returnBool = 1;
    for (int prio=0; prio<nPrio; prio++)
    {
        returnBool = returnBool && (passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirMiror+1)-passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNotVec[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
    }
    
    return returnBool;
}

bool mapi::upOK()
{
    int ix, iy;
    double xHeros = Heros->getX();
    double yHeros = Heros->getY();
    int dirHeros = Heros->getDir();
    
    if (dirHeros == 0)
    {
        iy = ceil(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    else if (dirHeros == 1)
    {
        iy = round(yHeros/ySprites);
        ix = floor(xHeros/xSprites);
    }
    else if (dirHeros == 2)
    {
        iy = round(yHeros/ySprites);
        ix = ceil(xHeros/xSprites);
    }
    else if (dirHeros == 3)
    {
        iy = floor(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    int dirMiror = 0;
    if (dirHeros == 0) dirMiror = 3;
    else if (dirHeros == 1) dirMiror = 2;
    else if (dirHeros == 2) dirMiror = 1;
    else if (dirHeros == 3) dirMiror = 0;
    if (iy-1 < 0)
        return 0;
    bool returnBool = 1;
    for (int prio=0; prio<nPrio; prio++)
    {
        returnBool = returnBool && (passOrNotVec[prio][ix][iy-1]%(int)pow(2,dirMiror+1)-passOrNotVec[prio][ix][iy-1]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNotVec[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
    }
    
    return returnBool;
}

bool mapi::leftOK()
{
    int ix, iy;
    double xHeros = Heros->getX();
    double yHeros = Heros->getY();
    int dirHeros = Heros->getDir();
    
    if (dirHeros == 0)
    {
        iy = ceil(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    else if (dirHeros == 1)
    {
        iy = round(yHeros/ySprites);
        ix = floor(xHeros/xSprites);
    }
    else if (dirHeros == 2)
    {
        iy = round(yHeros/ySprites);
        ix = ceil(xHeros/xSprites);
    }
    else if (dirHeros == 3)
    {
        iy = floor(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    int dirMiror = 0;
    if (dirHeros == 0) dirMiror = 3;
    else if (dirHeros == 1) dirMiror = 2;
    else if (dirHeros == 2) dirMiror = 1;
    else if (dirHeros == 3) dirMiror = 0;
    if (ix - 1 < 0)
        return 0;
    bool returnBool = 1;
    for (int prio=0; prio<nPrio; prio++)
    {
        returnBool = returnBool && (passOrNotVec[prio][ix-1][iy]%(int)pow(2,dirMiror+1)-passOrNotVec[prio][ix-1][iy]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNotVec[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
    }
    
    return returnBool;
}

bool mapi::rightOK()
{
    int ix, iy;
    double xHeros = Heros->getX();
    double yHeros = Heros->getY();
    int dirHeros = Heros->getDir();
    
    if (dirHeros == 0)
    {
        iy = ceil(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    else if (dirHeros == 1)
    {
        iy = round(yHeros/ySprites);
        ix = floor(xHeros/xSprites);
    }
    else if (dirHeros == 2)
    {
        iy = round(yHeros/ySprites);
        ix = ceil(xHeros/xSprites);
    }
    else if (dirHeros == 3)
    {
        iy = floor(yHeros/ySprites);
        ix = round(xHeros/xSprites);
    }
    int dirMiror = 0;
    if (dirHeros == 0) dirMiror = 3;
    else if (dirHeros == 1) dirMiror = 2;
    else if (dirHeros == 2) dirMiror = 1;
    else if (dirHeros == 3) dirMiror = 0;
    if (ix + 1 >= lxMap)
        return 0;
    bool returnBool = 1;
    for (int prio=0; prio<nPrio; prio++)
    {
        returnBool = returnBool && (passOrNotVec[prio][ix+1][iy]%(int)pow(2,dirMiror+1)-passOrNotVec[prio][ix+1][iy]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNotVec[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNotVec[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
    }
    
    return returnBool;
}


void mapi::update(double eT)
{
    ctrlZObject->addElapsedTime(eT);
    if (isImLeft)
        imL->imagePNG::update();
    sf::Vector2f effectivePosMouse = convertPos(posMouse);
    if (isMouseHere && state == adding)
    {    
        sf::Vector2f effectivePosMouse = convertPos(posMouse);
        if (!select || (select && select2))
        {
            sf::Vector2u foo(0,0);
            if (isImLeft)
            {
                foo = imL->getNSprites();
                if (foo.x != 0)
                {
                    int* foo3 = imL->pullSprites();
                    spriteToAdd = imL->getSprite();
                    spriteToAdd.setTextureRect(sf::IntRect(foo3[0], foo3[1], foo.x*xSprites, foo.y*ySprites));
                    spriteToAdd.setColor(sf::Color(255,255,255,128));
                    effectivePosMouse.y = round(effectivePosMouse.y/ySprites)*ySprites-ySprites*foo.y;
                    effectivePosMouse.x = round(effectivePosMouse.x/xSprites)*xSprites-xSprites*foo.x;
                    spriteToAdd.setPosition(effectivePosMouse);    
                    ghostSprite = 1;
                    freeSpritesCtrlC();
                }
            }
            if (foo.x == 0)
            {
                ghostSprite = 0;
                if (nxCtrlC != 0) ghostSpriteCtrlC = 1;
                int xmin, ymin;
                for(int i=0; i<nxCtrlC; i++)
                {
                    for (int j=0; j<nyCtrlC; j++)
                    {
                        xmin = (round(effectivePosMouse.x/xSprites)-nxCtrlC)*xSprites;
                        ymin = (round(effectivePosMouse.y/ySprites)-nyCtrlC)*ySprites;
                        if (prioCtrlC == 4)
                            for (int k=0; k<nPrio; k++)
                                spriteCtrlCVec[k][i][j].setPosition(xmin+i*xSprites, ymin+j*ySprites);
                        else
                            spriteCtrlCVec[prioCtrlC][i][j].setPosition(xmin+i*xSprites, ymin+j*ySprites);
                    }
                }
            }
        }
        else
        {
            int* foo3;
            sf::Vector2u foo4;
            if (isImLeft)
            {
                foo4 = imL->getNSprites();
                foo3 = imL->pullSprites();
            }
            if (foo4.x != 0)
            {
                spriteToAdd.setTextureRect(sf::IntRect(foo3[0], foo3[1], foo4.x*xSprites, foo4.y*ySprites));
                spriteToAdd.setColor(sf::Color(255,255,255,128));
                spriteToAdd.setPosition(convertPos(posClick));        
                ghostSprite = 1;
                ghostSpriteCtrlC = 0;
            }
            else ghostSprite = 0;
        }
    }
    else if (isMouseHere && isMousePressed && state == erasing)
    {
        sf::Vector2f effectivePosMouse = convertPos(posMouse);
        int ix = floor(effectivePosMouse.x/xSprites);
        int iy = floor(effectivePosMouse.y/ySprites);
        if (ix>=0 && ix<lxMap && iy>=0 && iy<lyMap)
        {
            if (!allPrio)
            {
                sprite fooSprite;
                string fooTexture;
                sf::IntRect fooRect;
                for (int prio=0; prio<4; prio++)
                {            
                    if (indexSpriteVec[prio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = spriteVec[prio][indexSpriteVec[prio][ix][iy][0]][indexSpriteVec[prio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextureVec[iTextureVec[prio][indexSpriteVec[prio][ix][iy][0]]];
                    
                        ctrlZObject->saveErasing(fooSprite, prio, passOrNotVec[prio][ix][iy], fooTexture);
                    }
                    removeSprite(prio, ix, iy);
                }
            }
            else
            {
                sprite fooSprite;
                string fooTexture;
                sf::IntRect fooRect;
                if (indexSpriteVec[currentPrio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = spriteVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]][indexSpriteVec[currentPrio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextureVec[iTextureVec[currentPrio][indexSpriteVec[currentPrio][ix][iy][0]]];
                
                    ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNotVec[currentPrio][ix][iy], fooTexture);
                }
                removeSprite(currentPrio, ix, iy);
            }
        }
    }
    if (!isMouseHere || state != adding)
    {
        ghostSprite = 0;
        ghostSpriteCtrlC = 0;
    }
    if (isMousePressed && state == moving)
    {
        sf::Vector2f foo = viewMap.getSize();
        
        viewMap.move(sf::Vector2f((oldPosMouse.x-posMouse.x)*foo.x/lx,(oldPosMouse.y-posMouse.y)*foo.y/ly));
        oldPosMouse = posMouse;
    }
    if (isMouseHere && delta!=0)
    {
        sf::Vector2f center = viewMap.getCenter();
        sf::Vector2f size = viewMap.getSize();
        
        double ratioZoom = pow(iZoom,delta);
        if (size.x/lx*ratioZoom > maxZoom)
            ratioZoom = maxZoom*lx/size.x;
        else if (size.x/lx*ratioZoom < minZoom)
            ratioZoom = minZoom*lx/size.x;
        
        center.x = (center.x - effectivePosMouse.x)*ratioZoom;
        center.y = (center.y - effectivePosMouse.y)*ratioZoom;
        center = center + effectivePosMouse;
        size.x *= ratioZoom;
        size.y *= ratioZoom;
        viewMap.setCenter(center);
        viewMap.setSize(size);
        
        delta = 0;
    }
    if (isMousePressed && (state == adding || state == selecting))
    {
        if (!select || (select && select2)) 
        {
            select = 1;
            select2 = 0;
            if (state == adding)
            {
                sf::Vector2f foo2 = viewMap.getSize();
                sf::Vector2u foo;
                if (isImLeft)
                    foo = imL->getNSprites();
                if (foo.x != 0)
                {
                    posClick = invConvertPos(spriteToAdd.getPosition())+sf::Vector2i(1,1);
                }
                else if (ghostSpriteCtrlC)
                {
                    int foo;
                    if (prioCtrlC == 4) foo = 0;
                    else foo = prioCtrlC;
                    posClick = invConvertPos(spriteCtrlCVec[foo][0][0].getPosition())+sf::Vector2i(1,1);
                }
            }
        }
        if (select)
        {
            sf::Vector2f clickEff = convertPos(posClick);
            effectivePosMouse.x = round(effectivePosMouse.x/xSprites)*xSprites;
            effectivePosMouse.y = round(effectivePosMouse.y/ySprites)*ySprites;
            int xmin = min(clickEff.x, effectivePosMouse.x);
            int xmax = max(clickEff.x, effectivePosMouse.x);
            int ymin = min(clickEff.y, effectivePosMouse.y);
            int ymax = max(clickEff.y, effectivePosMouse.y);
            xmin = max(xmin, 0);
            xmax = min(xmax, lxMap*xSprites);
            ymin = max(ymin, 0);
            ymax = min(ymax, lyMap*ySprites);
    
            selectRect.setPosition(xmin, ymin);
            selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
        }
    }
    else if (select)
    {
        select2 = 1;
        sf::Vector2f foo;
        sf::Vector2f foo2;
        foo = selectRect.getPosition();
        foo2 = selectRect.getSize();
        double xmin = round(foo.x/xSprites)*xSprites;
        double ymin = round(foo.y/ySprites)*ySprites;
        double xmax = max(xmin+xSprites,round((foo.x+foo2.x)/xSprites)*xSprites);
        double ymax = max(ymin+ySprites,round((foo.y+foo2.y)/ySprites)*ySprites);
        if (xmin < 0) xmin = 0;
        if (xmax > lxMap*xSprites)  xmax = lxMap*xSprites;
        if (ymin < 0) ymin = 0;
        if (ymax > lyMap*ySprites)  ymax = lyMap*ySprites;
        selectRect.setPosition(xmin,ymin);
        selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
        
        if (state == adding)
        {
            if (ghostSprite)
            {
                if (isImLeft)
                {
                    sf::Vector2u foo5 = imL->getNSprites();
                    if (foo5.x * foo5.y > 0)
                    {
                        string foo4 = imL->getFile();
                        int* foo3 = imL->pullSprites();
                        sprite s;
                        s.xPNG = foo3[0];
                        s.yPNG = foo3[1];
                        s.x = xmin;
                        s.y = ymin;
                        int lxS = (xmax-xmin)/xSprites;
                        int lyS = (ymax-ymin)/ySprites;
                        addSprite2(s, currentPrio, foo4, foo5.x, foo5.y, lxS, lyS);
                        select = select2 = 0;
                        /*
                        for (int i=0; i<(xmax-xmin)/xSprites; i++)
                        {
                            for (int j=0; j<(ymax-ymin)/ySprites; j++)
                            {
                                s.xPNG = foo3[i%foo5.x][j%foo5.y][0];
                                s.yPNG = foo3[i%foo5.x][j%foo5.y][1];
                                s.x = xmin + i*xSprites;
                                s.y = ymin + j*ySprites;
                                if (s.x>=0 && s.x<(lxMap)*xSprites && s.y>=0 && s.y<(lyMap)*ySprites)
                                {
                                    removeSprite(currentPrio, s.x/xSprites, s.y/ySprites);
                                    addSprite(s, currentPrio, foo4);
                                }
                            }
                        }*/
                        //imL->freeSprites();
                    }    
                }
            }
            else if (ghostSpriteCtrlC && state == adding)
            {
                ctrlV();
                select = select2 = 0;
            }
        }
    }
}

void mapi::draw()
{
    mapWindow.clear(sf::Color::Black);
    sf::Sprite foo;
    if (!allPrio || state == heros)
    {
        sprite s;
        for (int i=0; i<2; i++)
        {
            for (int j=0; j<nTextureVec[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(fileTextureVec[iTextureVec[i][j]]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSpriteVec[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    //mapWindow.draw(Sprite[i][j][k]);
                    mapWindow.draw(spriteVec[i][j][k]);
                }
            }
        }
        if (state == heros)
        {
            mapWindow.draw(Heros->getSprite()); 
            float xView = Heros->getX()-lx/2.;
            float yView = Heros->getY()-ly/2.;
            if (xView < 0) xView = 0;
            if (xView > lxMap*xSprites-lx) xView = lxMap*xSprites-lx;    
            if (yView < 0) yView = 0;
            if (yView > lyMap*ySprites-ly) yView = lyMap*ySprites-ly;        
            if (lx > lxMap*xSprites) xView = -(lx-lxMap*xSprites)/2;
            if (ly > lyMap*ySprites) yView = -(ly-lyMap*ySprites)/2;
            viewMap.reset(sf::FloatRect(xView,yView,lx,ly));
        }
        for (int i=2; i<nPrio; i++)
        {
            for (int j=0; j<nTextureVec[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(fileTextureVec[iTextureVec[i][j]]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSpriteVec[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    //mapWindow.draw(Sprite[i][j][k]);
                    mapWindow.draw(spriteVec[i][j][k]);
                }
            }
        }
    }
    else
    {
        bool toHide = 0;
        sprite s;
        for (int i=0; i<nPrio; i++)
        {
            if (i == currentPrio) toHide = 1;
            else toHide = 0;
            for (int j=0; j<nTextureVec[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(fileTextureVec[iTextureVec[i][j]]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSpriteVec[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    //if (toHide)    foo.setColor(sf::Color(255, 255, 255, 255));
                    //else foo.setColor(sf::Color(255,255,255,64));
                    if (toHide)    spriteVec[i][j][k].setColor(sf::Color(255, 255, 255, 255));
                    else spriteVec[i][j][k].setColor(sf::Color(255,255,255,64));
                    mapWindow.draw(spriteVec[i][j][k]);
                    spriteVec[i][j][k].setColor(sf::Color(255, 255, 255, 255));
                }
            }
        }
    }
    mapWindow.draw(boundary);
    if (grid)
    {
        for (int i=0; i<lxMap-1; i++)
            mapWindow.draw(gridX[i]);
        for (int i=0; i<lyMap-1; i++)
            mapWindow.draw(gridY[i]);
    }
    if (select && state != heros)
        mapWindow.draw(selectRect);
    sf::RectangleShape foo2;
    sf::RectangleShape foo3;
    foo2.setSize(sf::Vector2f(xSprites,ySprites));
    foo2.setFillColor(sf::Color(255,0,0,64));
    foo3.setFillColor(sf::Color(0,0,255,64));
    if (!allPrio && showPass)
    {
        for (int prio=0; prio<nPrio; prio++)
        {
            for (int i=0; i<lxMap; i++)
            {
                for (int j=0; j<lyMap; j++)
                {
                    if (passOrNotVec[prio][i][j] != 0)
                    {    
                        int fooInt = passOrNotVec[prio][i][j];
                        foo2.setPosition(i*xSprites,j*ySprites);
                        mapWindow.draw(foo2);
                        if (fooInt/8 == 0)
                        {
                            foo3.setPosition(xSprites*(i+0.35),ySprites*j);
                            foo3.setSize(sf::Vector2f(0.3*xSprites,ySprites*0.65));
                            mapWindow.draw(foo3);
                        }
                        else
                            fooInt -= 8;
                        
                        if (fooInt/4 == 0)
                        {
                            foo3.setPosition(xSprites*(i+0.5-0.15),ySprites*(j+0.35));
                            foo3.setSize(sf::Vector2f(xSprites*(0.65),ySprites*0.3));
                            mapWindow.draw(foo3);
                        }
                        else
                            fooInt -= 4;
                        
                        if (fooInt/2 == 0)
                        {
                            foo3.setPosition(xSprites*i,ySprites*(j+0.35));
                            foo3.setSize(sf::Vector2f(xSprites*0.65,ySprites*0.3));
                            mapWindow.draw(foo3);
                        }
                        else
                            fooInt -= 2;
                        
                        if (fooInt == 0)
                        {
                            foo3.setPosition(xSprites*(i+0.35),ySprites*(j+0.35));
                            foo3.setSize(sf::Vector2f(0.3*xSprites,ySprites*0.65));
                            mapWindow.draw(foo3);
                        }
                    }
                }
            }
        }
    }
    else if (showPass)
    {
        for (int i=0; i<lxMap; i++)
        {
            for (int j=0; j<lyMap; j++)
            {
                if (passOrNotVec[currentPrio][i][j] != 0)
                {
                    int fooInt = passOrNotVec[currentPrio][i][j];
                    foo2.setPosition(i*xSprites,j*ySprites);
                    mapWindow.draw(foo2);
                    if (fooInt/8 == 0)
                    {
                        foo3.setPosition(xSprites*(i+0.35),ySprites*j);
                        foo3.setSize(sf::Vector2f(0.3*xSprites,ySprites*0.65));
                        mapWindow.draw(foo3);
                    }
                    else
                        fooInt -= 8;
                    
                    if (fooInt/4 == 0)
                    {
                        foo3.setPosition(xSprites*(i+0.5-0.15),ySprites*(j+0.35));
                        foo3.setSize(sf::Vector2f(xSprites*(0.65),ySprites*0.3));
                        mapWindow.draw(foo3);
                    }
                    else
                        fooInt -= 4;
                    
                    if (fooInt/2 == 0)
                    {
                        foo3.setPosition(xSprites*i,ySprites*(j+0.35));
                        foo3.setSize(sf::Vector2f(xSprites*0.65,ySprites*0.3));
                        mapWindow.draw(foo3);
                    }
                    else
                        fooInt -= 2;
                    
                    if (fooInt == 0)
                    {
                        foo3.setPosition(xSprites*(i+0.35),ySprites*(j+0.35));
                        foo3.setSize(sf::Vector2f(0.3*xSprites,ySprites*0.65));
                        mapWindow.draw(foo3);
                    }
                }
            }
        }
    }
    if (ghostSprite && state != heros)
    {
        mapWindow.draw(spriteToAdd);
    }
    if (ghostSpriteCtrlC)
    {
        for (int i=0; i<nxCtrlC; i++)
        {
            for (int j=0; j<nyCtrlC; j++)
            {
                if (prioCtrlC == 4)
                {
                    for (int prio=0; prio<nPrio; prio++)
                        mapWindow.draw(spriteCtrlCVec[prio][i][j]);
                }
                else
                    mapWindow.draw(spriteCtrlCVec[prioCtrlC][i][j]);
            }
        }
    }
    if (state != heros)
    {
        sf::CircleShape fooCircle;
        fooCircle.setFillColor(sf::Color::Blue);
        fooCircle.setPosition(Heros->getX(),Heros->getY());
        fooCircle.setRadius(xSprites/2);
        mapWindow.draw(fooCircle);
    }
    /*
    cout<<"map displayed : "<<stringFile<<endl;
    cout<<"x = "<<x<<", y = "<<y<<endl;
    cout<<"lx = "<<lx<<", ly = "<<ly<<endl;
    sf::Vector2f foop = viewMap.getCenter();
    cout<<"x view = "<<foop.x<<", y view = "<<foop.y<<endl;
    foop = viewMap.getSize();
    cout<<"sizeX view = "<<foop.x<<", sizeY view = "<<foop.y<<endl;
    foop = spriteVec[0][indexSpriteVec[0][0][0][0]][indexSpriteVec[0][0][0][1]].getPosition();
    cout<<"PosX sprite0 = "<<foop.x<<", posY sprite0 = "<<foop.y<<endl<<endl;
    */
    mapWindow.display();
    mapWindow.setView(viewMap);
    toDraw.setTexture(mapWindow.getTexture());
    toDraw.setPosition(x,y);
    window->draw(toDraw);
    if (state != heros)
    {
        if (isImLeft)
        {
            imL->imagePNG::draw();
            window->draw(limL);
        }
        if (isImRight)
        {
            imR->imagePNG::draw();
            window->draw(limR);
        }
    }  
    else
    {
        for (int i=0; i<nEvents; i++)
        {
            events[i]->testHero(Heros);
        }
    }
}

void mapi::drawClearWindow()
{
    mapWindow.clear(sf::Color::Black);
    mapWindow.display();
    toDraw.setTexture(mapWindow.getTexture());
    toDraw.setPosition(x,y);
    window->draw(toDraw);
    window->display();
}

void mapi::draw(sf::Drawable* d)
{
    window->draw(*d);
}

void mapi::closeWindow()
{
    window = 0;
}
