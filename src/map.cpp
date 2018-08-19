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
    ghostSpriteCtrlC = 0;
    ghostSprite = 0;
    
    nxCtrlC = nyCtrlC = 0;
    xCtrlC = yCtrlC = 0;
    prioCtrlC = -1;
    
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color::Red);
    selectRect.setOutlineThickness(8.);
    
    grid = 0;
    gridX = gridY = 0;
    
    lxMap = lyMap = 0;
    xSprites = ySprites = 32;
    nPrio = 4;
    Sprite = new sf::Sprite**[nPrio];
    iTexture = new int*[nPrio];
    nTotTexture = 0;
    indexSprites = new int***[nPrio];
    nTexture = new int[nPrio];
    nameTexture = new string*[nPrio];
    nSprite = new int*[nPrio];
    passOrNot = new int**[nPrio];
    spriteCtrlC = new sf::Sprite**[nPrio];
    fileCtrlC = new string**[nPrio];
    nExceptions = 0;
    for (int i=0; i<nPrio; i++)
    {
        nTexture[i] = 0;
        Sprite[i] = 0;
        nameTexture[i] = 0;
        nSprite[i] = 0;
        indexSprites[i] = 0;
        passOrNot[i] = 0;
        iTexture[i] = 0;
        spriteCtrlC[i] = 0;
        fileCtrlC[i] = 0;
    }
    Texture = 0;
    fileTextures = 0;
    
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
}

mapi::~mapi()
{
    for (int i=0; i<nPrio; i++)
    {
        for (int j=0; j<lxMap; j++)
        {
            for (int k=0; k<lyMap; k++)
            {
                delete[] indexSprites[i][j][k];
                indexSprites[i][j][k] = 0;
            }
            delete[] indexSprites[i][j];
            indexSprites[i][j] = 0;
            delete[] passOrNot[i][j];
            passOrNot[i][j] = 0;
        }
        for (int j=0; j<nTexture[i]; j++)
        {
            delete[] Sprite[i][j];
            Sprite[i][j] = 0;
        }
        if (nTexture[i] > 0)
        {
            delete[] Sprite[i];
            delete[] nSprite[i];
            delete[] nameTexture[i];
            delete[] iTexture[i];
            Sprite[i] = 0;
            nSprite[i] = 0;
            nameTexture[i] = 0;
            iTexture[i] = 0;
        }
        delete[] indexSprites[i];
        delete[] passOrNot[i];
        indexSprites[i] = 0;
        passOrNot[i] = 0;
    }
    for (int i=0; i<nTotTexture; i++)
    {
        delete Texture[i];
        Texture[i] = 0;
    }
    if (Sprite != 0)
    {
        delete[] nTexture;
        delete[] Sprite;
        delete[] nSprite;
        delete[] nameTexture;
        delete[] indexSprites;
        delete[] iTexture;
        delete[] Texture;
        delete[] fileTextures;
        nTexture = 0;
        Sprite = 0;
        nSprite = 0;
        nameTexture = 0;
        indexSprites = 0;
        Texture  = 0;
        iTexture = 0;
        fileTextures = 0;
    }
    delete[] passOrNot;
    passOrNot = 0;
    if (isImLeft)
    {
        delete imL;
        imL = 0;
    }
    delete[] gridX;
    delete[] gridY;
    gridX = 0;
    gridY = 0;
    delete ctrlZObject;
    freeSpritesCtrlC();
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
        toReturn[i] = fileTextures[i];
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
        if (passOrNot[prio][ix][iy] == 0 && pass != 0)
        {
            nExceptions += 1;
            passOrNot[prio][ix][iy] = pass;
        }
        else if (passOrNot[prio][ix][iy] != 0 && pass == 0)
        {
            nExceptions -= 1;
            passOrNot[prio][ix][iy] = pass;
        }
        else
            passOrNot[prio][ix][iy] = pass;
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
                if (passOrNot[prio][ix][iy] == 0) nExceptions += 1;
                if (passOrNot[prio][ix][iy] == 0 and dir != 4)
                    passOrNot[prio][ix][iy] = (int)pow(2,dir);
                else if(passOrNot[prio][ix][iy] == 0 and dir == 4)
                    passOrNot[prio][ix][iy] = 16-1;
                else
                {
                    if (dir == 4)
                        passOrNot[prio][ix][iy] = 15 - passOrNot[prio][ix][iy];
                    else
                    {
                        int foo = passOrNot[prio][ix][iy]%(int)pow(2,dir+1)-passOrNot[prio][ix][iy]%(int)pow(2,dir);
                        foo /= pow(2,dir);
                        passOrNot[prio][ix][iy] = passOrNot[prio][ix][iy]+(int)pow(2,dir)*pow(-1,foo);
                    }
                }
                if (passOrNot[prio][ix][iy] == 0) nExceptions -= 1;
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
                    
    
    int ***fooPassOrNot = new int**[nPrio];
    int ****fooIndexSprites = new int***[nPrio];
    for (int i=0; i<nPrio; i++)
    {
        fooPassOrNot[i] = new int*[s.x];
        fooIndexSprites[i] = new int**[s.x];
        for (int j=0; j<s.x; j++)
        {
            fooPassOrNot[i][j] = new int[s.y];
            fooIndexSprites[i][j] = new int*[s.y];
            for (int k=0; k<s.y; k++)
            {
                if (j < lxMap && k < lyMap)
                {
                    fooPassOrNot[i][j][k] = passOrNot[i][j][k];
                    fooIndexSprites[i][j][k] = new int[2];
                    fooIndexSprites[i][j][k][0] = indexSprites[i][j][k][0];
                    fooIndexSprites[i][j][k][1] = indexSprites[i][j][k][1];
                    delete[] indexSprites[i][j][k];
                    indexSprites[i][j][k] = 0;
                }
                else
                {
                    fooPassOrNot[i][j][k] = 0;
                    fooIndexSprites[i][j][k] = new int[2];
                    fooIndexSprites[i][j][k][0] = fooIndexSprites[i][j][k][1] = -1;
                }
            }
            if (j < lxMap)
            {
                delete[] passOrNot[i][j];
                passOrNot[i][j] = 0;
                delete[] indexSprites[i][j];
                indexSprites[i][j] = 0;
            }
        }
        delete[] passOrNot[i];
        passOrNot[i] = 0;
        delete[] indexSprites[i];
        indexSprites[i] = 0;
    }
    
    lxMap = s.x;
    lyMap = s.y;
    
    for (int i=0; i<nPrio; i++)
    {
        passOrNot[i] = new int*[lxMap];
        indexSprites[i] = new int**[lxMap];
        for (int j=0; j<lxMap; j++)
        {
            passOrNot[i][j] = new int[lyMap];
            indexSprites[i][j] = new int*[lyMap];
            for (int k=0; k<lyMap; k++)
            {
                passOrNot[i][j][k] = fooPassOrNot[i][j][k];
                indexSprites[i][j][k] = new int[2];
                indexSprites[i][j][k][0] = fooIndexSprites[i][j][k][0];
                indexSprites[i][j][k][1] = fooIndexSprites[i][j][k][1];
                delete[] fooIndexSprites[i][j][k];
                fooIndexSprites[i][j][k] = 0;
            }
            delete[] fooPassOrNot[i][j];
            fooPassOrNot[i][j] = 0;
            delete[] fooIndexSprites[i][j];
            fooIndexSprites[i][j] = 0;
        }
        delete[] fooPassOrNot[i];
        fooPassOrNot[i] = 0;
        delete[] fooIndexSprites[i];
        fooIndexSprites[i] = 0;
    }
    delete[] fooPassOrNot;
    fooPassOrNot = 0;
    delete[] fooIndexSprites;
    fooIndexSprites = 0;
    
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineColor(sf::Color::Green);
    
    double thickness = xSprites;
    boundary.setOutlineThickness(thickness);
    
    boundary.setPosition(0,0);
    boundary.setSize(sf::Vector2f(lxMap*xSprites, lyMap*ySprites));
    
    double sizeGrid = 4;
    gridX = new sf::RectangleShape[lxMap-1];
    gridY = new sf::RectangleShape[lyMap-1];
    
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

void mapi::setPosHeros(int xi, int yi)
{
    Heros->setX(xi);
    Heros->setY(yi);
}

void mapi::setDirHeros(int dir)
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
    {
        for (int j=0; j<nTexture[i]; j++)
        {
            for (int k=0; k<nSprite[i][j]; k++)
            {
                Sprite[i][j][k].setTexture(*Texture[iTexture[i][j]]);
            }
        }
    }
}

int mapi::addTexture(string t)
{
    int fooInt = -1;
    for (int i=0; i<nTotTexture; i++)
        if (t == fileTextures[i] && fooInt == -1)
            fooInt = i;
            
    if (fooInt == -1)
    {
        sf::Texture** fooTexture = new sf::Texture*[nTotTexture+1];
        string* fooString = new string[nTotTexture+1];
        for (int i=0; i<nTotTexture; i++)
        {
            fooTexture[i] = new sf::Texture;
            *fooTexture[i] = *Texture[i];
            delete Texture[i];
            Texture[i] = 0;
            fooString[i] = fileTextures[i];
        }
        if (nTotTexture > 0)
        {
            delete[] Texture;
            Texture = 0;
            delete[] fileTextures;
            fileTextures = 0;
        }
        fooTexture[nTotTexture] = new sf::Texture;
        fooTexture[nTotTexture]->loadFromFile(t);
        fooString[nTotTexture] = t;
        nTotTexture += 1;
        Texture = new sf::Texture*[nTotTexture];
        fileTextures = new string[nTotTexture];
        for (int i=0; i<nTotTexture; i++)
        {
            Texture[i] = new sf::Texture;
            *Texture[i] = *fooTexture[i];
            delete fooTexture[i];
            fooTexture[i] = 0;
            fileTextures[i] = fooString[i];
        }
            
        delete[] fooTexture;
        fooTexture = 0;
        delete[] fooString;        
        fooString = 0;
        return (nTotTexture-1);
    }
    return fooInt;
}

void mapi::removeTexture(int prio, int iT, string t)
{
    if (nTexture[prio] > 0)
    {
        int* fooInt = new int[nTexture[prio]-1];
        for (int i=0; i<nTexture[prio]; i++)
        {
            if (i < iT)
                fooInt[i] = iTexture[prio][i];
            else if (i > iT)
                fooInt[i-1] = iTexture[prio][i];
        }
        delete[] iTexture[prio];
        iTexture[prio] = 0;
        iTexture[prio] = new int[nTexture[prio]-1];
        for (int i=0; i<nTexture[prio]-1; i++)
            iTexture[prio][i] = fooInt[i];
        delete[] fooInt;
        fooInt = 0;
    }
}

/*void mapi::removeTexture(int prio, int iT, string t)
{
    
    cout<<"Debut remove Texture\n";
    cout<<"Prio = "<<prio<<endl;
    cout<<"iT = "<<iT<<endl;
    cout<<"iTexture = "<<iTexture[prio][iT]<<endl<<endl;
    cout<<"File = "<<fileTextures[iTexture[prio][iT]]<<endl;
    for (int i=0; i<nPrio; i++)
    {
        cout<<"Prio = "<<i<<endl;
        cout<<"nTexture = "<<nTexture[i]<<endl;
        for (int j=0; j<nTexture[i]; j++)
        {
            cout<<"Indice = "<<iTexture[i][j]<<endl;
            cout<<fileTextures[iTexture[i][j]]<<"\t";
            cout<<nameTexture[i][j]<<endl;
        }
        cout<<endl;
    }
    cout<<"nTotTexture = "<<nTotTexture<<endl;
    for (int i=0; i<nTotTexture; i++)
        cout<<fileTextures[i]<<endl;
    cout<<endl;
    bool textureStillHere = 0;
    for (int i=0; i<nPrio; i++)
        for (int j=0; j<nTexture[i]; j++)
            if (i != prio || j != iT)
                textureStillHere = textureStillHere || t == fileTextures[iTexture[i][j]];
    if (!textureStillHere)
    {
        int index = iTexture[prio][iT];
        for (int iPrio=0; iPrio<nPrio; iPrio++)
        {
            if (nTexture[iPrio] > 0)
            {
                if (iPrio == prio)
                {
                    int* fooInt = new int[nTexture[iPrio]-1];
                    for (int i=0; i<nTexture[iPrio]; i++)
                    {
                        if (iTexture[iPrio][i] > index)
                            iTexture[iPrio][i] -= 1;
                        if (i < iT)
                            fooInt[i] = iTexture[iPrio][i];
                        else if (i > iT)
                            fooInt[i-1] = iTexture[iPrio][i];
                    }
                    delete[] iTexture[iPrio];
                    iTexture[iPrio] = new int[nTexture[iPrio]-1];
                    for (int i=0; i<nTexture[iPrio]-1; i++)
                        iTexture[iPrio][i] = fooInt[i];
                    delete[] fooInt;
                    fooInt = 0;
                }
                if (iPrio != prio)
                {
                    int* fooInt = new int[nTexture[iPrio]];
                    for (int i=0; i<nTexture[iPrio]; i++)
                    {
                        if (iTexture[iPrio][i] > index)
                            iTexture[iPrio][i] -= 1;
                        fooInt[i] = iTexture[iPrio][i];
                    }
                    delete[] iTexture[iPrio];
                    iTexture[iPrio] = new int[nTexture[iPrio]];
                    for (int i=0; i<nTexture[iPrio]; i++)
                        iTexture[iPrio][i] = fooInt[i];
                    delete[] fooInt;
                    fooInt = 0;
                }
            }
        }
    
        sf::Texture** fooText = new sf::Texture*[nTotTexture-1];
        string* fooString = new string[nTotTexture-1];
        for (int i=0; i<nTotTexture; i++)
        {
            if (i<index)
            {
                fooText[i] = new sf::Texture(*Texture[i]);
                fooString[i] = fileTextures[i];
            }
            else if (i>index)
            {
                fooText[i-1] = new sf::Texture(*Texture[i]);
                fooString[i-1] = fileTextures[i];
            }
            delete Texture[i];
            Texture[i] = 0;
        }
        delete[] Texture;
        delete[] fileTextures;
        Texture = 0;
        fileTextures = 0;
        nTotTexture -= 1;
        Texture = new sf::Texture*[nTotTexture];
        fileTextures = new string[nTotTexture];
        for (int i=0; i<nTotTexture; i++)
        {
            Texture[i] = new sf::Texture(*fooText[i]);
            delete fooText[i];
            fooText[i] = 0;
            fileTextures[i] = fooString[i];
        }
        delete[] fooText;
        fooText = 0;
        delete[] fooString;    
        fooString = 0;    
    }
    else
    {
        int* fooInt = new int[nTexture[prio]-1];
        for (int i=0; i<nTexture[prio]; i++)
        {
            if (i < iT)
                fooInt[i] = iTexture[prio][i];
            else if (i > iT)
                fooInt[i-1] = iTexture[prio][i];
        }
        delete[] iTexture[prio];
        iTexture[prio] = 0;
        iTexture[prio] = new int[nTexture[prio]-1];
        for (int i=0; i<nTexture[prio]-1; i++)
            iTexture[prio][i] = fooInt[i];
        delete[] fooInt;
        fooInt = 0;
    }
}*/

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
                passOrNot[i] = new int*[lxMap];
                for (int j=0; j<lxMap; j++)
                {
                    passOrNot[i][j] = new int[lyMap];
                    for (int k=0; k<lyMap; k++)
                        passOrNot[i][j][k] = 0;
                }
            }
        
            for (int i=0; i<nPrio; i++)
            {
                indexSprites[i] = new int**[lxMap];
                for (int j=0; j<lxMap; j++)
                {
                    indexSprites[i][j] = new int*[lyMap];
                    for (int k=0; k<lyMap; k++)
                    {
                        indexSprites[i][j][k] = new int[2];
                        indexSprites[i][j][k][0] = -1;
                        indexSprites[i][j][k][1] = -1;
                    }
                }        
            }
            for (int i=0; i<nPrio; i++)
            {    
                file>>foo>>nTexture[i]>>foo;
                if (nTexture[i] > 0)
                {
                    nameTexture[i] = new string[nTexture[i]];
                    nSprite[i] = new int[nTexture[i]];
                    Sprite[i] = new sf::Sprite*[nTexture[i]];
                    iTexture[i] = new int[nTexture[i]];
                
                    for (int j=0; j<nTexture[i]; j++)
                    {
                        file>>nSprite[i][j];
                        Sprite[i][j] = new sf::Sprite[nSprite[i][j]];
                    }
                    file>>foo;
                
                    for (int j=0; j<nTexture[i]; j++)
                    {
                        file>>nameTexture[i][j]>>foo;
                        iTexture[i][j] = addTexture(nameTexture[i][j]);
                        if (foo[0] != '{')
                        {    
                            cout<<"Invalid syntax for texture "<<nameTexture[i][j]<<" at prio "<<i<<endl;
                            return 0;
                        }
                        sprite s;
                        for (int k=0; k<nSprite[i][j]; k++)
                        {
                            file>>s.x>>s.y>>s.xPNG>>s.yPNG>>foo;
                            Sprite[i][j][k].setTexture(*Texture[iTexture[i][j]]);
                            Sprite[i][j][k].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
                            Sprite[i][j][k].setPosition(s.x,s.y);        
                            if (s.x>=0 && s.x<lxMap*xSprites && s.y>=0 && s.y<lyMap*ySprites)
                            {
                                indexSprites[i][s.x/xSprites][s.y/ySprites][0] = j;
                                indexSprites[i][s.x/xSprites][s.y/ySprites][1] = k;
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
                passOrNot[fooPrio][fooX][fooY] = fooDir;            
            }
            file.close();
            initMap();
            if (nTexture > 0) initPNG(fileTextures[0],'L');
            else initPNG("Tileset/base.PNG",'L');
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
            file<<"\t"<<nTexture[i]<<" #nTexture\n\t";
            for (int j=0; j<nTexture[i]; j++)
                file<<nSprite[i][j]<<" ";
            if (nTexture[i] > 0) file<<"#nSprite\n";
            for (int j=0; j<nTexture[i]; j++)
            {
                file<<"\t"<<nameTexture[i][j]<<" { ";
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    fooPos = Sprite[i][j][k].getPosition();
                    fooRect = Sprite[i][j][k].getTextureRect();
                    fooPNG = sf::Vector2f(fooRect.left, fooRect.top);
                    file<<fooPos.x<<" "<<fooPos.y<<" ";
                    file<<fooPNG.x<<" "<<fooPNG.y<<" ";
                    if (k == nSprite[i][j]-1)
                        file<<"}\n";
                    else
                        file<<", ";
                }
            }
            file<<"\n\n";
        }
        file<<"passOrNot\n\n"<<nExceptions<<" #nExceptions\n\t";
        for (int i=0; i<nPrio; i++)
        {
            for (int j=0; j<lxMap; j++)
                for (int k=0; k<lyMap; k++)
                    if (passOrNot[i][j][k] != 0)
                        file<<i<<" "<<j<<" "<<k<<" "<<passOrNot[i][j][k]<<"  ";
                    
            if (i == nPrio-1)
                file<<"\n\n";
        }
        file<<"\n";
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
        for (int i=0; i<nPrio; i++)
        {
            for (int j=0; j<lxMap; j++)
            {
                for (int k=0; k<lyMap; k++)
                {
                    delete[] indexSprites[i][j][k];
                    indexSprites[i][j][k] = 0;
                }
                delete[] indexSprites[i][j];
                indexSprites[i][j] = 0;
                delete[] passOrNot[i][j];
                passOrNot[i][j] = 0;
            }
            for (int j=0; j<nTexture[i]; j++)
            {
                delete[] Sprite[i][j];
                Sprite[i][j] = 0;
            }
            if (nTexture[i] > 0)
            {
                delete[] Sprite[i];
                delete[] nSprite[i];
                delete[] nameTexture[i];
                delete[] iTexture[i];
            }
            delete[] passOrNot[i];
        
            passOrNot[i] = 0;
            Sprite[i] = 0;
            nSprite[i] = 0;
            nameTexture[i] = 0;
            nTexture[i] = 0;
            indexSprites[i] = 0;
            iTexture[i] = 0;
        }
        if (Texture != 0)
        {
            for (int i=0; i<nTotTexture; i++)
            {
                delete Texture[i];
                Texture[i] = 0;
            }
            delete[] Texture;
            nTotTexture = 0;
            Texture = 0;
        }
        boundary.setFillColor(sf::Color::Transparent);
        boundary.setOutlineColor(sf::Color::Green);
    
        double thickness = xSprites;
        boundary.setOutlineThickness(thickness);
    
        boundary.setPosition(0,0);
        boundary.setSize(sf::Vector2f(lxMap*xSprites, lyMap*ySprites));
    
        viewMap.reset(sf::FloatRect(0,0,lx,ly));
        mapWindow.setView(viewMap);
        
        double sizeGrid = 4;
        gridX = new sf::RectangleShape[lxMap-1];
        gridY = new sf::RectangleShape[lyMap-1];
        
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
            passOrNot[i] = new int*[lxMap];
            for (int j=0; j<lxMap; j++)
            {
                passOrNot[i][j] = new int[lyMap];
                for (int k=0; k<lyMap; k++)
                    passOrNot[i][j][k] = 0;
            }
        }
    
        for (int i=0; i<nPrio; i++)
        {
            indexSprites[i] = new int**[lxMap];
            for (int j=0; j<lxMap; j++)
            {
                indexSprites[i][j] = new int*[lyMap];
                for (int k=0; k<lyMap; k++)
                {
                    indexSprites[i][j][k] = new int[2];
                    indexSprites[i][j][k][0] = -1;
                    indexSprites[i][j][k][1] = -1;
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
    
    viewMap.reset(sf::FloatRect(-4*xSprites-thickness,-thickness,lx,ly));
    mapWindow.setView(viewMap);
    
    double sizeGrid = 4;
    gridX = new sf::RectangleShape[lxMap-1];
    gridY = new sf::RectangleShape[lyMap-1];
    
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
            toErase = (toErase || indexSprites[iPrio][s.x/xSprites][s.y/ySprites][0]!=-1);
            toErase = (toErase || indexSprites[iPrio][s.x/xSprites][s.y/ySprites][0]!=-1);
            if (toErase)
                removeSprite(prio, s.x/xSprites, s.y/ySprites);
            toErase = 0;
        }
        int indexTexture = -1;
        for (int j=0; j<nTexture[prio]; j++)
        {
            if (t == nameTexture[prio][j])
                indexTexture = j;
        }
        if (indexTexture == -1)
        {
            string* foo = new string[nTexture[prio]+1];
            int* foo2 = new int[nTexture[prio]+1];
            sf::Sprite** foo3 = new sf::Sprite*[nTexture[prio]+1];
            int* fooInt = new int[nTexture[prio]+1];
            for (int j=0; j<nTexture[prio]; j++)
            {
                foo[j] = nameTexture[prio][j];
                foo2[j] = nSprite[prio][j];
                foo3[j] = new sf::Sprite[nSprite[prio][j]];
                fooInt[j] = iTexture[prio][j];
                for (int k=0; k<nSprite[prio][j]; k++)
                    foo3[j][k] = Sprite[prio][j][k];
                delete[] Sprite[prio][j];
                Sprite[prio][j] = 0;
            }
            fooInt[nTexture[prio]] = addTexture(t);
            foo[nTexture[prio]] = t;
            foo2[nTexture[prio]] = 1;
            foo3[nTexture[prio]] = new sf::Sprite[1];
            foo3[nTexture[prio]][0].setTexture(*Texture[fooInt[nTexture[prio]]]);
            foo3[nTexture[prio]][0].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
            foo3[nTexture[prio]][0].setPosition(s.x,s.y);
            
            if (nTexture[prio] > 0)
            {
                delete[] nameTexture[prio];
                delete[] nSprite[prio];
                delete[] Sprite[prio];
                delete[] iTexture[prio];
            }
            nameTexture[prio] = 0;
            nSprite[prio] = 0;
            Sprite[prio] = 0;
            iTexture[prio] = 0;
            
            indexSprites[prio][s.x/xSprites][s.y/ySprites][0] = nTexture[prio];
            indexSprites[prio][s.x/xSprites][s.y/ySprites][1] = 0;
            nTexture[prio] += 1;
            nameTexture[prio] = new string[nTexture[prio]];
            nSprite[prio] = new int[nTexture[prio]];
            Sprite[prio] = new sf::Sprite*[nTexture[prio]];
            iTexture[prio] = new int[nTexture[prio]];
            
            for (int j=0; j<nTexture[prio]; j++)
            {
                nameTexture[prio][j] = foo[j];
                nSprite[prio][j] = foo2[j];
                iTexture[prio][j] = fooInt[j];
                Sprite[prio][j] = new sf::Sprite[nSprite[prio][j]];
                for (int k=0; k<nSprite[prio][j]; k++)
                    Sprite[prio][j][k] = foo3[j][k];
                delete[] foo3[j];
                foo3[j] = 0;
            }
            delete[] foo;
            delete[] foo2;
            delete[] foo3;
            delete[] fooInt;
            foo = 0;
            foo2 = 0;
            foo3 = 0;
            fooInt = 0;
        }
        else
        {
            sf::Sprite* foo = new sf::Sprite[nSprite[prio][indexTexture]+1];
            for (int k=0; k<nSprite[prio][indexTexture]; k++)
                foo[k] = Sprite[prio][indexTexture][k];
            delete[] Sprite[prio][indexTexture];
            Sprite[prio][indexTexture] = 0;
            
            foo[nSprite[prio][indexTexture]].setTexture(*Texture[iTexture[prio][indexTexture]]);
            foo[nSprite[prio][indexTexture]].setTextureRect(sf::IntRect(s.xPNG,s.yPNG,xSprites,ySprites));
            foo[nSprite[prio][indexTexture]].setPosition(s.x,s.y);
            
            indexSprites[prio][s.x/xSprites][s.y/ySprites][0] = indexTexture;
            indexSprites[prio][s.x/xSprites][s.y/ySprites][1] = nSprite[prio][indexTexture];
            nSprite[prio][indexTexture] += 1;
            
            Sprite[prio][indexTexture] = new sf::Sprite[nSprite[prio][indexTexture]];
            for (int k=0; k<nSprite[prio][indexTexture]; k++)
                Sprite[prio][indexTexture][k] = foo[k];
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
                if (indexSprites[prio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = Sprite[prio][indexSprites[prio][ix][iy][0]][indexSprites[prio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextures[iTexture[prio][indexSprites[prio][ix][iy][0]]];
    
                    ctrlZObject->saveErasing(fooSprite, prio, passOrNot[prio][ix][iy], fooTexture);
                }
                removeSprite(prio, ix, iy);
            }
        }
        ctrlZObject->saveAdding(s, prio, lxS, lyS);
        int indexTexture = -1;
        for (int j=0; j<nTexture[prio]; j++)
        {
            if (t == nameTexture[prio][j])
                indexTexture = j;
        }
        if (indexTexture == -1)
        {
            string* foo = new string[nTexture[prio]+1];
            int* foo2 = new int[nTexture[prio]+1];
            sf::Sprite** foo3 = new sf::Sprite*[nTexture[prio]+1];
            int* fooInt = new int[nTexture[prio]+1];
            for (int j=0; j<nTexture[prio]; j++)
            {
                foo[j] = nameTexture[prio][j];
                foo2[j] = nSprite[prio][j];
                foo3[j] = new sf::Sprite[nSprite[prio][j]];
                fooInt[j] = iTexture[prio][j];
                for (int k=0; k<nSprite[prio][j]; k++)
                    foo3[j][k] = Sprite[prio][j][k];
                delete[] Sprite[prio][j];
                Sprite[prio][j] = 0;
            }
            fooInt[nTexture[prio]] = addTexture(t);
            foo[nTexture[prio]] = t;
            foo2[nTexture[prio]] = lxS*lyS;
            foo3[nTexture[prio]] = new sf::Sprite[lxS*lyS];
            sprite fooS = s;
            for (int ix=0; ix<lxS; ix++)
            {
                for (int iy=0; iy<lyS; iy++)
                {
                    fooS.x = s.x + ix*xSprites;
                    fooS.y = s.y + iy*ySprites;
                    fooS.xPNG = s.xPNG + (ix%nxS)*xSprites;
                    fooS.yPNG = s.yPNG + (iy%nyS)*ySprites;
                    foo3[nTexture[prio]][ix*lyS+iy].setTextureRect(sf::IntRect(fooS.xPNG,fooS.yPNG,xSprites,ySprites));
                    foo3[nTexture[prio]][ix*lyS+iy].setPosition(fooS.x,fooS.y);
                    indexSprites[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = nTexture[prio];
                    indexSprites[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = ix*lyS+iy;
                }
            }
            if (nTexture[prio] > 0)
            {
                delete[] nameTexture[prio];
                delete[] nSprite[prio];
                delete[] Sprite[prio];
                delete[] iTexture[prio];
            }
            
            nTexture[prio] += 1;
            nameTexture[prio] = new string[nTexture[prio]];
            nSprite[prio] = new int[nTexture[prio]];
            Sprite[prio] = new sf::Sprite*[nTexture[prio]];
            iTexture[prio] = new int[nTexture[prio]];
            
            for (int j=0; j<nTexture[prio]; j++)
            {
                nameTexture[prio][j] = foo[j];
                nSprite[prio][j] = foo2[j];
                Sprite[prio][j] = new sf::Sprite[nSprite[prio][j]];
                iTexture[prio][j] = fooInt[j];
                for (int k=0; k<nSprite[prio][j]; k++)
                {
                    Sprite[prio][j][k] = foo3[j][k];
                }
                    
                delete[] foo3[j];
                foo3[j] = 0;
            }
            delete[] foo;
            delete[] foo2;
            delete[] foo3;
            delete[] fooInt;
            foo = 0;
            foo2 = 0;
            foo3 = 0;
            fooInt = 0;
        }
        else
        {
            sf::Sprite* foo = new sf::Sprite[nSprite[prio][indexTexture]+lxS*lyS];
            for (int k=0; k<nSprite[prio][indexTexture]; k++)
                foo[k] = Sprite[prio][indexTexture][k];
            delete[] Sprite[prio][indexTexture];
            Sprite[prio][indexTexture] = 0;
            
            for (int ix=0; ix<lxS; ix++)
            {
                for (int iy=0; iy<lyS; iy++)
                {
                    foo[nSprite[prio][indexTexture]+ix*lyS+iy].setTexture(*Texture[iTexture[prio][indexTexture]]);
                    foo[nSprite[prio][indexTexture]+ix*lyS+iy].setTextureRect(sf::IntRect(s.xPNG+ (ix%nxS)*xSprites,s.yPNG + (iy%nyS)*ySprites,xSprites,ySprites));
                    foo[nSprite[prio][indexTexture]+ix*lyS+iy].setPosition(s.x + ix*xSprites,s.y + iy*ySprites);
                    indexSprites[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = indexTexture;
                    indexSprites[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = nSprite[prio][indexTexture]+ix*lyS+iy;
                }
            }
            nSprite[prio][indexTexture] += lxS*lyS;
            
            Sprite[prio][indexTexture] = new sf::Sprite[nSprite[prio][indexTexture]];
            for (int k=0; k<nSprite[prio][indexTexture]; k++)
                Sprite[prio][indexTexture][k] = foo[k];
                
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
        int iT = indexSprites[prio][ix][iy][0];
        int iS = indexSprites[prio][ix][iy][1];
        if (iT!= -1 && iS != -1)
        {
            int nT = nTexture[prio];
            int nS = nSprite[prio][iT];
            if (nS == 1)
            {
                removeTexture(prio, iT, fileTextures[iTexture[prio][iT]]);
                if (nT == 1)
                {
                    delete[] Sprite[prio][iT];
                    Sprite[prio][iT] = 0;
                    delete[] Sprite[prio];
                    delete[] nameTexture[prio];
                    delete[] nSprite[prio];
                    delete[] iTexture[prio];
                    iTexture[prio] = 0;
                    Sprite[prio] = 0;
                    nTexture[prio] = 0;
                    nameTexture[prio] = 0;
                    nSprite[prio] = 0;
                }
                else
                {
                    sf::Sprite** foo = new sf::Sprite*[nT-1];
                    string* foo2 = new string[nT-1];
                    int* foo3 = new int[nT-1];
            
                    for (int i=0; i<iT; i++)
                    {
                        foo[i] = new sf::Sprite[nSprite[prio][i]];
                        foo2[i] = nameTexture[prio][i];
                        foo3[i] = nSprite[prio][i];
                        for (int j=0; j<foo3[i]; j++)
                            foo[i][j] = Sprite[prio][i][j];
                    }
                    sf::Vector2f fooPos;
                    for (int i=iT+1; i<nT; i++)
                    {
                        foo[i-1] = new sf::Sprite[nSprite[prio][i]];
                        foo2[i-1] = nameTexture[prio][i];
                        foo3[i-1] = nSprite[prio][i];
                        for (int j=0; j<foo3[i-1]; j++)
                        {
                            foo[i-1][j] = Sprite[prio][i][j];
                            fooPos = Sprite[prio][i][j].getPosition();
                            indexSprites[prio][(int)fooPos.x/xSprites][(int)fooPos.y/ySprites][0] -= 1;
                        }
                        delete[] Sprite[prio][i];
                        Sprite[prio][i] = 0;                
                    }
                    delete[] nameTexture[prio];
                    delete[] nSprite[prio];
                    delete[] Sprite[prio];
                    nameTexture[prio] = 0;
                    nSprite[prio] = 0;
                    Sprite[prio] = 0;
            
                    nTexture[prio] -= 1;
                    nT = nTexture[prio];
            
                    Sprite[prio] = new sf::Sprite*[nT];
                    nSprite[prio] = new int[nT];
                    nameTexture[prio] = new string[nT];
                    sf::Texture textFoo;
                    for (int i=0; i<nT; i++)
                    {
                        Sprite[prio][i] = new sf::Sprite[foo3[i]];
                        nameTexture[prio][i] = foo2[i];
                        nSprite[prio][i] = foo3[i];
                        for (int j=0; j<nSprite[prio][i]; j++)
                            Sprite[prio][i][j] = foo[i][j];
                        delete[] foo[i];
                        foo[i] = 0;
                    }
                    delete[] foo;
                    delete[] foo2;
                    delete[] foo3;    
                    foo = 0;
                    foo2 = 0;
                    foo3 = 0;
                }
            }
            else
            {
                sf::Sprite* foo = new sf::Sprite[nS-1];
                for (int i=0; i<iS; i++)
                    foo[i] = Sprite[prio][iT][i];
                sf::Vector2f fooPos;
                for (int i=iS+1; i<nS; i++)
                {
                    foo[i-1] = Sprite[prio][iT][i];
                    fooPos = foo[i-1].getPosition();
                    indexSprites[prio][(int)fooPos.x/xSprites][(int)fooPos.y/ySprites][1] -= 1;
                }
                
                delete[] Sprite[prio][iT];
                Sprite[prio][iT] = 0;
                
                nSprite[prio][iT] -= 1;
                nS = nSprite[prio][iT];
        
                Sprite[prio][iT] = new sf::Sprite[nS];
                for (int i=0; i<nS; i++)
                    Sprite[prio][iT][i] = foo[i];
                delete[] foo;
                foo = 0;
            }
        }
        indexSprites[prio][ix][iy][0] = -1;
        indexSprites[prio][ix][iy][1] = -1;    
        if (passOrNot[prio][ix][iy] != 0)
        {
            nExceptions -= 1;
            passOrNot[prio][ix][iy] = 0;    
        }
    }
    resetTextureSprite();
}

void mapi::freeSpritesCtrlC()
{
    if (spriteCtrlC != 0)
    {
        for (int prio=0; prio<nPrio; prio++)
        {
            if (spriteCtrlC[prio] != 0)
            {
                for (int i=0; i<nxCtrlC; i++)
                {
                    delete[] spriteCtrlC[prio][i];
                    spriteCtrlC[prio][i] = 0;
                    delete[] fileCtrlC[prio][i];
                    fileCtrlC[prio][i] = 0;
                }
                delete[] spriteCtrlC[prio];
                spriteCtrlC[prio] = 0;
                delete[] fileCtrlC[prio];
                fileCtrlC[prio] = 0;
            }
        }
    }
    nxCtrlC = nyCtrlC = 0;
    ghostSpriteCtrlC = 0;
}

void mapi::ctrlC()
{
    if (select && select2 && state==selecting)
    {
        if (spriteCtrlC != 0)
        {
            for (int prio=0; prio<nPrio; prio++)
            {
                if (spriteCtrlC[prio] != 0)
                {
                    for (int i=0; i<nxCtrlC; i++)
                    {
                        delete[] spriteCtrlC[prio][i];
                        spriteCtrlC[prio][i] = 0;
                        delete[] fileCtrlC[prio][i];
                        fileCtrlC[prio][i] = 0;
                    }
                    delete[] spriteCtrlC[prio];
                    spriteCtrlC[prio] = 0;
                    delete[] fileCtrlC[prio];
                    fileCtrlC[prio] = 0;
                }
            }
        }
        
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
                spriteCtrlC[prio] = new sf::Sprite*[nxCtrlC];
                fileCtrlC[prio] = new string*[nxCtrlC];
                sf::Sprite s;
                int iT, iS;
                for (int i=0; i<nxCtrlC; i++)
                {
                    spriteCtrlC[prio][i] = new sf::Sprite[nyCtrlC];
                    fileCtrlC[prio][i] = new string[nyCtrlC];
                    for (int j=0; j<nyCtrlC; j++)
                    {
                        iT = indexSprites[prio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][0];
                        iS = indexSprites[prio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][1];
                        if (iT != -1 && iS != -1)
                        {
                            s = Sprite[prio][iT][iS];
                            spriteCtrlC[prio][i][j].setTexture(*Texture[iTexture[prio][iT]]);
                            spriteCtrlC[prio][i][j].setTextureRect(s.getTextureRect());
                            spriteCtrlC[prio][i][j].setColor(sf::Color(255,255,255,128));
                            fileCtrlC[prio][i][j] = fileTextures[iTexture[prio][iT]];
                        }
                        else fileCtrlC[prio][i][j] = "No File";
                    }
                }
            }
        }
        else
        {
            prioCtrlC = currentPrio;
            spriteCtrlC[currentPrio] = new sf::Sprite*[nxCtrlC];
            fileCtrlC[currentPrio] = new string*[nxCtrlC];
            sf::Sprite s;
            int iT, iS;
            for (int i=0; i<nxCtrlC; i++)
            {
                spriteCtrlC[currentPrio][i] = new sf::Sprite[nyCtrlC];
                fileCtrlC[currentPrio][i] = new string[nyCtrlC];
                for (int j=0; j<nyCtrlC; j++)
                {
                    iT = indexSprites[currentPrio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][0];
                    iS = indexSprites[currentPrio][(int)xCtrlC/xSprites+i][(int)yCtrlC/ySprites+j][1];
                    if (iT != -1 && iS != -1)
                    {
                        s = Sprite[currentPrio][iT][iS];
                        spriteCtrlC[currentPrio][i][j].setTexture(*Texture[iTexture[currentPrio][iT]]);
                        spriteCtrlC[currentPrio][i][j].setTextureRect(s.getTextureRect());
                        spriteCtrlC[currentPrio][i][j].setColor(sf::Color(255,255,255,128));
                        fileCtrlC[currentPrio][i][j] = fileTextures[iTexture[currentPrio][iT]];
                    }
                    else fileCtrlC[currentPrio][i][j] = "No File";
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
                    if (indexSprites[prio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = Sprite[prio][indexSprites[prio][ix][iy][0]][indexSprites[prio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextures[iTexture[prio][indexSprites[prio][ix][iy][0]]];
        
                        ctrlZObject->saveErasing(fooSprite, prio, passOrNot[prio][ix][iy], fooTexture);
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
                if (indexSprites[currentPrio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = Sprite[currentPrio][indexSprites[currentPrio][ix][iy][0]][indexSprites[currentPrio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextures[iTexture[currentPrio][indexSprites[currentPrio][ix][iy][0]]];
    
                    ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNot[currentPrio][ix][iy], fooTexture);
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
                    if (fileCtrlC[prio][i%nxCtrlC][j%nyCtrlC] != "No File")
                    {
                        s.x = fooPos.x + i*xSprites;
                        s.y = fooPos.y + j*ySprites;
                        fooRect = spriteCtrlC[prio][i%nxCtrlC][j%nyCtrlC].getTextureRect();
                        s.xPNG = fooRect.left;
                        s.yPNG = fooRect.top;
                        addSprite(s, prio, fileCtrlC[prio][i%nxCtrlC][j%nyCtrlC]);
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
                if (fileCtrlC[prioCtrlC][i%nxCtrlC][j%nyCtrlC] != "No File")
                {
                    s.x = fooPos.x + i*xSprites;
                    s.y = fooPos.y + j*ySprites;
                    fooRect = spriteCtrlC[prioCtrlC][i%nxCtrlC][j%nyCtrlC].getTextureRect();
                    s.xPNG = fooRect.left;
                    s.yPNG = fooRect.top;
                    addSprite(s, currentPrio, fileCtrlC[prioCtrlC][i%nxCtrlC][j%nyCtrlC]);
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
                        if (indexSprites[prio][ix][iy][0] != -1)
                        {
                            fooSprite.x = ix*xSprites;
                            fooSprite.y = iy*ySprites;
                            fooRect = Sprite[prio][indexSprites[prio][ix][iy][0]][indexSprites[prio][ix][iy][1]].getTextureRect();
                            fooSprite.xPNG = fooRect.left;
                            fooSprite.yPNG = fooRect.top;
                            fooTexture = fileTextures[iTexture[prio][indexSprites[prio][ix][iy][0]]];
            
                            ctrlZObject->saveErasing(fooSprite, prio, passOrNot[prio][ix][iy], fooTexture);
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
                    if (indexSprites[currentPrio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = Sprite[currentPrio][indexSprites[currentPrio][ix][iy][0]][indexSprites[currentPrio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextures[iTexture[currentPrio][indexSprites[currentPrio][ix][iy][0]]];
                
                        ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNot[currentPrio][ix][iy], fooTexture);
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
                    if (passOrNot[currentPrio][ix][iy] == 0) nExceptions += 1;
                    if (passOrNot[currentPrio][ix][iy] == 0 and dir != 4)
                        passOrNot[currentPrio][ix][iy] = (int)pow(2,dir);
                    else if(passOrNot[currentPrio][ix][iy] == 0 and dir == 4)
                        passOrNot[currentPrio][ix][iy] = 16-1;
                    else
                    {
                        if (dir == 4)
                            passOrNot[currentPrio][ix][iy] = 15 - passOrNot[currentPrio][ix][iy];
                        else
                        {
                            int foo = passOrNot[currentPrio][ix][iy]%(int)pow(2,dir+1)-passOrNot[currentPrio][ix][iy]%(int)pow(2,dir);
                            foo /= pow(2,dir);
                            passOrNot[currentPrio][ix][iy] = passOrNot[currentPrio][ix][iy]+(int)pow(2,dir)*pow(-1,foo);
                        }
                    }
                    if (passOrNot[currentPrio][ix][iy] == 0) nExceptions -= 1;
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
            foo = foo && (passOrNot[prio][ix][iy]%2 == 0);
            foo = foo && (passOrNot[prio][ix][iy+1]-passOrNot[prio][ix][iy+1]%8 == 0);
        }
        break;
        
        case 1:
        if (ix-1 < 0) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNot[prio][ix][iy]%4-passOrNot[prio][ix][iy]%2 == 0);
            foo = foo && (passOrNot[prio][ix-1][iy]%8-passOrNot[prio][ix-1][iy]%4 == 0);
        }
        break;
        
        case 2:
        if (ix+1 >= lxMap) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNot[prio][ix][iy]%8-passOrNot[prio][ix][iy]%4 == 0);
            foo = foo && (passOrNot[prio][ix+1][iy]%4-passOrNot[prio][ix+1][iy]%2 == 0);
        }
        break;
        
        case 3:
        if (iy-1 < 0) return 0;
        for (int prio=0; prio<nPrio; prio++)
        {
            foo = foo && (passOrNot[prio][ix][iy]-passOrNot[prio][ix][iy]%8 == 0);
            foo = foo && (passOrNot[prio][ix][iy-1]%2 == 0);
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
        returnBool = returnBool && (passOrNot[prio][ix][iy+1]%(int)pow(2,dirMiror+1)-passOrNot[prio][ix][iy+1]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNot[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNot[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
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
        returnBool = returnBool && (passOrNot[prio][ix][iy-1]%(int)pow(2,dirMiror+1)-passOrNot[prio][ix][iy-1]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNot[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNot[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
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
        returnBool = returnBool && (passOrNot[prio][ix-1][iy]%(int)pow(2,dirMiror+1)-passOrNot[prio][ix-1][iy]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNot[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNot[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
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
        returnBool = returnBool && (passOrNot[prio][ix+1][iy]%(int)pow(2,dirMiror+1)-passOrNot[prio][ix+1][iy]%(int)pow(2,dirMiror) == 0);
        returnBool = returnBool && (passOrNot[prio][ix][iy]%(int)pow(2,dirHeros+1)-passOrNot[prio][ix][iy+1]%(int)pow(2,dirHeros) == 0);
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
                                spriteCtrlC[k][i][j].setPosition(xmin+i*xSprites, ymin+j*ySprites);
                        else
                            spriteCtrlC[prioCtrlC][i][j].setPosition(xmin+i*xSprites, ymin+j*ySprites);
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
                    if (indexSprites[prio][ix][iy][0] != -1)
                    {
                        fooSprite.x = ix*xSprites;
                        fooSprite.y = iy*ySprites;
                        fooRect = Sprite[prio][indexSprites[prio][ix][iy][0]][indexSprites[prio][ix][iy][1]].getTextureRect();
                        fooSprite.xPNG = fooRect.left;
                        fooSprite.yPNG = fooRect.top;
                        fooTexture = fileTextures[iTexture[prio][indexSprites[prio][ix][iy][0]]];
                    
                        ctrlZObject->saveErasing(fooSprite, prio, passOrNot[prio][ix][iy], fooTexture);
                    }
                    removeSprite(prio, ix, iy);
                }
            }
            else
            {
                sprite fooSprite;
                string fooTexture;
                sf::IntRect fooRect;
                if (indexSprites[currentPrio][ix][iy][0] != -1)
                {
                    fooSprite.x = ix*xSprites;
                    fooSprite.y = iy*ySprites;
                    fooRect = Sprite[currentPrio][indexSprites[currentPrio][ix][iy][0]][indexSprites[currentPrio][ix][iy][1]].getTextureRect();
                    fooSprite.xPNG = fooRect.left;
                    fooSprite.yPNG = fooRect.top;
                    fooTexture = fileTextures[iTexture[currentPrio][indexSprites[currentPrio][ix][iy][0]]];
                
                    ctrlZObject->saveErasing(fooSprite, currentPrio, passOrNot[currentPrio][ix][iy], fooTexture);
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
                    posClick = invConvertPos(spriteCtrlC[foo][0][0].getPosition())+sf::Vector2i(1,1);
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
            for (int j=0; j<nTexture[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(nameTexture[i][j]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    mapWindow.draw(Sprite[i][j][k]);
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
            for (int j=0; j<nTexture[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(nameTexture[i][j]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    mapWindow.draw(Sprite[i][j][k]);
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
            for (int j=0; j<nTexture[i]; j++)
            {
                //sf::Texture foo2;
                //foo2.loadFromFile(nameTexture[i][j]);
                //foo.setTexture(foo2);
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    //s = Sprite[i][j][k];
                    //foo.setTextureRect(sf::IntRect(s.xPNG, s.yPNG, xSprites, ySprites));
                    //foo.setPosition(s.x, s.y);
                    //if (toHide)    foo.setColor(sf::Color(255, 255, 255, 255));
                    //else foo.setColor(sf::Color(255,255,255,64));
                    if (toHide)    Sprite[i][j][k].setColor(sf::Color(255, 255, 255, 255));
                    else Sprite[i][j][k].setColor(sf::Color(255,255,255,64));
                    mapWindow.draw(Sprite[i][j][k]);
                    Sprite[i][j][k].setColor(sf::Color(255, 255, 255, 255));
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
                    if (passOrNot[prio][i][j] != 0)
                    {    
                        int fooInt = passOrNot[prio][i][j];
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
                if (passOrNot[currentPrio][i][j] != 0)
                {
                    int fooInt = passOrNot[currentPrio][i][j];
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
                        mapWindow.draw(spriteCtrlC[prio][i][j]);
                }
                else
                    mapWindow.draw(spriteCtrlC[prioCtrlC][i][j]);
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
}

void mapi::closeWindow()
{
    window = 0;
}
