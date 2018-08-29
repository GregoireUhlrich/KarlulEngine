#include "../include/map.h"
#include <iostream>
using namespace std;

mapCtrlZ::mapCtrlZ(mapi* Mi)
{
    M = Mi;
    sf::Vector2u fooSizeSprites = M->getSizeSprites();
    xSprites = fooSizeSprites.x;
    ySprites = fooSizeSprites.y;
    memoryCtrlZ = 50;
    nCtrlZ = 0;
    
    thresholdTime = 0.5;
    elapsedTime = 0;
    nAddCtrlZ = 0;
    nEraseCtrlZ = 0;
}

mapCtrlZ::~mapCtrlZ()
{
    for (int i=nCtrlZ; i>0; i--)
    {
        switch(action[0])
        {
            case addCtrlZ:
            clearAdding();
            break;
            
            case eraseCtrlZ:
            clearErasing();
            break;
            
            case passOrNotCtrlZ:
            clearChangingPass();
            break;
        }
    }
}

void mapCtrlZ::addElapsedTime(double eT)
{
    elapsedTime += eT;
}

void mapCtrlZ::saveAdding(sprite s, int prio, int lxS, int lyS)
{
    action.insert(action.begin(), addCtrlZ);
    nCtrlZ += 1;
    
    prioAddCtrlZ.insert(prioAddCtrlZ.begin(),prio);
    posAddCtrlZ.insert(posAddCtrlZ.begin(),vector<int>(2));
    posAddCtrlZ[0][0] = round(s.x/xSprites);
    posAddCtrlZ[0][1] = round(s.y/ySprites);
    nSpriteAdd.insert(nSpriteAdd.begin(), vector<int>(2));
    nSpriteAdd[0][0] = lxS;
    nSpriteAdd[0][1] = lyS;
    nAddCtrlZ += 1;
}

void mapCtrlZ::clearAdding()
{
    if (nCtrlZ > 1)
    {
        action.erase(action.begin());
        nCtrlZ -= 1;
    
        if (nAddCtrlZ > 1)
        {
            nSpriteAdd.erase(nSpriteAdd.begin());
            posAddCtrlZ.erase(posAddCtrlZ.begin());
            prioAddCtrlZ.erase(prioAddCtrlZ.begin());
            nAddCtrlZ -= 1;
        }
        else
        {
            nSpriteAdd.clear();
            posAddCtrlZ.clear();
            prioAddCtrlZ.clear();
            nAddCtrlZ = 0;
        }
    }
    else
    {
        nCtrlZ = 0;
        action.clear();
        nAddCtrlZ = 0;
        nSpriteAdd.clear();
        posAddCtrlZ.clear();
        prioAddCtrlZ.clear();
    }
}

void mapCtrlZ::saveErasing(sprite s, int prio, int pass, string t)
{
    
    if (nEraseCtrlZ == 0 || elapsedTime > thresholdTime || elapsedTime < 0)
    {
        action.insert(action.begin(), eraseCtrlZ);
        nCtrlZ += 1;
        
        nSpriteErase.insert(nSpriteErase.begin(), 1);
        prioSpriteEraseCtrlZ.insert(prioSpriteEraseCtrlZ.begin(), vector<int>(1));
        prioSpriteEraseCtrlZ[0][0] = prio;
        passSpriteEraseCtrlZ.insert(passSpriteEraseCtrlZ.begin(), vector<int>(1));
        passSpriteEraseCtrlZ[0][0] = pass;
        fileEraseCtrlZ.insert(fileEraseCtrlZ.begin(), vector<string>(1));
        fileEraseCtrlZ[0][0] = t;
        spriteEraseCtrlZ.insert(spriteEraseCtrlZ.begin(), vector<sprite>(1));
        spriteEraseCtrlZ[0][0] = s;
        nEraseCtrlZ += 1;
    }
    else
    {
        prioSpriteEraseCtrlZ[0].push_back(prio);
        passSpriteEraseCtrlZ[0].push_back(pass);
        spriteEraseCtrlZ[0].push_back(s);
        fileEraseCtrlZ[0].push_back(t);
        nSpriteErase[0] += 1;
    }
    elapsedTime = 0;
}

void mapCtrlZ::clearErasing()
{
    if (nCtrlZ > 1)
    {
        action.erase(action.begin());
        nCtrlZ -= 1;
    
        if (nEraseCtrlZ > 1)
        {
            nSpriteErase.erase(nSpriteErase.begin());
            prioSpriteEraseCtrlZ.erase(prioSpriteEraseCtrlZ.begin());
            passSpriteEraseCtrlZ.erase(passSpriteEraseCtrlZ.begin());
            spriteEraseCtrlZ.erase(spriteEraseCtrlZ.begin());
            fileEraseCtrlZ.erase(fileEraseCtrlZ.begin());        
            nEraseCtrlZ -= 1;
        }
        else
        {
            nEraseCtrlZ = 0;
            nSpriteErase.clear();
            prioSpriteEraseCtrlZ.clear();
            passSpriteEraseCtrlZ.clear();
            fileEraseCtrlZ.clear();
            spriteEraseCtrlZ.clear();
        }
    }
    else
    {
        nCtrlZ = 0;
        action.clear();
        nEraseCtrlZ = 0;
        nEraseCtrlZ = 0;
        nSpriteErase.clear();
        prioSpriteEraseCtrlZ.clear();
        passSpriteEraseCtrlZ.clear();
        fileEraseCtrlZ.clear();
        spriteEraseCtrlZ.clear();
    }
}

void mapCtrlZ::saveChangingPass(int prio, int dir, int ix, int iy, int lxS, int lyS)
{
    int pass = dir;
    action.insert(action.begin(), passOrNotCtrlZ);
    nCtrlZ += 1;
    
    nPassCtrlZ.insert(nPassCtrlZ.begin(), vector<int>(2));
    nPassCtrlZ[0][0] = lxS;
    nPassCtrlZ[0][1] = lyS;
    initPosPassCtrlZ.insert(initPosPassCtrlZ.begin(), vector<int>(2));
    initPosPassCtrlZ[0][0] = ix;
    initPosPassCtrlZ[0][1] = iy;
    passCtrlZ.insert(passCtrlZ.begin(), pass);
    prioPassCtrlZ.insert(prioPassCtrlZ.begin(), prio);
    nPassOrNotCtrlZ += 1;
}

void mapCtrlZ::clearChangingPass()
{
    if (nCtrlZ > 1)
    {
        action.erase(action.begin());
        nCtrlZ -= 1;
        
        if (nPassOrNotCtrlZ > 1)
        {
            nPassOrNotCtrlZ -= 1;
            nPassCtrlZ.erase(nPassCtrlZ.begin());
            initPosPassCtrlZ.erase(initPosPassCtrlZ.begin());
            passCtrlZ.erase(passCtrlZ.begin());
            prioPassCtrlZ.erase(prioPassCtrlZ.begin());
        }
        else
        {
            nPassOrNotCtrlZ = 0;
            nPassCtrlZ.clear();
            initPosPassCtrlZ.clear();
            passCtrlZ.clear();
            prioPassCtrlZ.clear();
        }
    }
    else
    {
        nCtrlZ = 0;
        action.clear();
        nPassOrNotCtrlZ = 0;
        nPassCtrlZ.clear();
        initPosPassCtrlZ.clear();
        passCtrlZ.clear();
        prioPassCtrlZ.clear();
    }
}

void mapCtrlZ::CtrlZ()
{
    if (nCtrlZ > 0)
    {
        int lxS, lyS, x0, y0, prioRemove;
        sprite s;
        string t;
        int pass;
        int prio;
        switch(action[0])
        {
            case addCtrlZ:
            lxS = nSpriteAdd[0][0];
            lyS = nSpriteAdd[0][1];
            x0 = posAddCtrlZ[0][0];
            y0 = posAddCtrlZ[0][1];
            prioRemove = prioAddCtrlZ[0];
            for (int i=0; i<lxS; i++)
                for (int j=0; j<lyS; j++)
                    M->removeSprite(prioRemove, x0+i, y0+j);
            clearAdding();
            break;
            
            case eraseCtrlZ:
            for (int i=0; i<nSpriteErase[0]; i++)
            {
                s = spriteEraseCtrlZ[0][i];
                t = fileEraseCtrlZ[0][i];
                prio = prioSpriteEraseCtrlZ[0][i];
                pass = passSpriteEraseCtrlZ[0][i];
                M->addSprite(s, prio, t);
                M->setPassOrNot(pass,prio,round(s.x/xSprites), round(s.y/ySprites));
            }
            clearErasing();
            break;
            
            case passOrNotCtrlZ:
            x0 = initPosPassCtrlZ[0][0];
            y0 = initPosPassCtrlZ[0][1];
            lxS = nPassCtrlZ[0][0];
            lyS = nPassCtrlZ[0][1];
            prio = prioPassCtrlZ[0];
            pass = passCtrlZ[0];
            M->pastePassOrNot(pass,prio,x0,y0,lxS,lyS);
            clearChangingPass();
            break;
        }
    }
}    
