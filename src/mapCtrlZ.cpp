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
	action = 0;
	nAddCtrlZ = 0;
	nSpriteAdd = 0;
	prioAddCtrlZ = 0;
	posAddCtrlZ = 0;
	prioAddCtrlZ = 0;
	
	thresholdTime = 0.5;
	elapsedTime = 0;
	nEraseCtrlZ = 0;
	nSpriteErase = 0;
	prioSpriteEraseCtrlZ = 0;
	passSpriteEraseCtrlZ = 0;
	spriteEraseCtrlZ = 0;
	initPosPassCtrlZ = 0;
	
	nPassOrNotCtrlZ = 0;
	nPassCtrlZ = 0;
	initPosPassCtrlZ = 0;
	passCtrlZ = 0;
	prioPassCtrlZ = 0;
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
	actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ+1];
	for (int i=0; i<nCtrlZ; i++)
		fooAction[i+1] = action[i];
	if (action != 0)
		delete[] action;
	fooAction[0] = addCtrlZ;
	nCtrlZ += 1;
	action = new actionCtrlZ[nCtrlZ];
	for (int i=0; i<nCtrlZ; i++)
		action[i] = fooAction[i];
	delete[] fooAction;
	
	int** fooNSprite = new int*[nAddCtrlZ+1];
	int** fooPosAdd = new int*[nAddCtrlZ+1];
	int* fooPrioAdd = new int[nAddCtrlZ+1];
	for (int i=0; i<nAddCtrlZ; i++)
	{
		fooNSprite[i+1] = new int[2];
		fooNSprite[i+1][0] = nSpriteAdd[i][0];
		fooNSprite[i+1][1] = nSpriteAdd[i][1];
		fooPosAdd[i+1] = new int[2];
		fooPosAdd[i+1][0] = posAddCtrlZ[i][0];
		fooPosAdd[i+1][1] = posAddCtrlZ[i][1];
		fooPrioAdd[i+1] = prioAddCtrlZ[i];			
		delete[] nSpriteAdd[i];
		delete[] posAddCtrlZ[i];
	}
	if (nSpriteAdd != 0)
	{
		delete[] nSpriteAdd;
		delete[] posAddCtrlZ;
		delete[] prioAddCtrlZ;
	}
	fooNSprite[0] = new int[2];
	fooNSprite[0][0] = lxS;
	fooNSprite[0][1] = lyS;
	fooPosAdd[0] = new int[2];
	fooPosAdd[0][0] = round(s.x/xSprites);
	fooPosAdd[0][1] = round(s.y/ySprites);
	fooPrioAdd[0] = prio;
	nAddCtrlZ += 1;
	nSpriteAdd = new int*[nAddCtrlZ];
	posAddCtrlZ = new int*[nAddCtrlZ];
	prioAddCtrlZ = new int[nAddCtrlZ];
	for (int i=0; i<nAddCtrlZ; i++)
	{
		nSpriteAdd[i] = new int[2];
		nSpriteAdd[i][0] = fooNSprite[i][0];
		nSpriteAdd[i][1] = fooNSprite[i][1];
		posAddCtrlZ[i] = new int[2];
		posAddCtrlZ[i][0] = fooPosAdd[i][0];
		posAddCtrlZ[i][1] = fooPosAdd[i][1];
		prioAddCtrlZ[i] = fooPrioAdd[i];			
		delete[] fooNSprite[i];
		delete[] fooPosAdd[i];
	}
	delete[] fooNSprite;
	delete[] fooPosAdd;
	delete[] fooPrioAdd;
}

void mapCtrlZ::clearAdding()
{
	if (nCtrlZ > 1)
	{
		actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ-1];
		for (int i=1; i<nCtrlZ; i++)
			fooAction[i-1] = action[i];
		delete[] action;
		action = 0;
		nCtrlZ -= 1;
		action = new actionCtrlZ[nCtrlZ];
		for (int i=0; i<nCtrlZ; i++)
			action[i] = fooAction[i];
		delete[] fooAction;
	
		if (nAddCtrlZ > 1)
		{
			int** fooNSprite = new int*[nAddCtrlZ-1];
			int** fooPosAdd = new int*[nAddCtrlZ-1];
			int* fooPrioAdd = new int[nAddCtrlZ-1];
			delete[] posAddCtrlZ[0];
			delete[] nSpriteAdd[0];
			for (int i=1; i<nAddCtrlZ; i++)
			{
				fooNSprite[i-1] = new int[2];
				fooNSprite[i-1][0] = nSpriteAdd[i][0];
				fooNSprite[i-1][1] = nSpriteAdd[i][1];
				fooPosAdd[i-1] = new int[2];
				fooPosAdd[i-1][0] = posAddCtrlZ[i][0];
				fooPosAdd[i-1][1] = posAddCtrlZ[i][1];
				fooPrioAdd[i-1] = prioAddCtrlZ[i];			
				delete[] nSpriteAdd[i];
				delete[] posAddCtrlZ[i];
			}
			if (nSpriteAdd != 0)
			{
				delete[] nSpriteAdd;
				delete[] posAddCtrlZ;
				delete[] prioAddCtrlZ;
			}
			nAddCtrlZ -= 1;
			nSpriteAdd = new int*[nAddCtrlZ];
			posAddCtrlZ = new int*[nAddCtrlZ];
			prioAddCtrlZ = new int[nAddCtrlZ];
			for (int i=0; i<nAddCtrlZ; i++)
			{
				nSpriteAdd[i] = new int[2];
				nSpriteAdd[i][0] = fooNSprite[i][0];
				nSpriteAdd[i][1] = fooNSprite[i][1];
				posAddCtrlZ[i] = new int[2];
				posAddCtrlZ[i][0] = fooPosAdd[i][0];
				posAddCtrlZ[i][1] = fooPosAdd[i][1];
				prioAddCtrlZ[i] = fooPrioAdd[i];			
				delete[] fooNSprite[i];
				delete[] fooPosAdd[i];
			}
			delete[] fooNSprite;
			delete[] fooPosAdd;
			delete[] fooPrioAdd;
		}
		else
		{
			nAddCtrlZ = 0;
			delete[] posAddCtrlZ[0];
			delete[] nSpriteAdd[0];
			delete[] nSpriteAdd;
			delete[] posAddCtrlZ;
			delete[] prioAddCtrlZ;
			nSpriteAdd = 0;
			posAddCtrlZ = 0;
			prioAddCtrlZ = 0;
		}
	}
	else
	{
		nCtrlZ = 0;
		delete[] action;
		action = 0;
		nAddCtrlZ = 0;
		delete[] posAddCtrlZ[0];
		delete[] nSpriteAdd[0];
		delete[] nSpriteAdd;
		delete[] posAddCtrlZ;
		delete[] prioAddCtrlZ;
		nSpriteAdd = 0;
		posAddCtrlZ = 0;
		prioAddCtrlZ = 0;
	}
}

void mapCtrlZ::saveErasing(sprite s, int prio, int pass, string t)
{
	
	if (nEraseCtrlZ == 0 || elapsedTime > thresholdTime || elapsedTime < 0)
	{
		actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ+1];
		for (int i=0; i<nCtrlZ; i++)
			fooAction[i+1] = action[i];
		if (action != 0)
			delete[] action;
		fooAction[0] = eraseCtrlZ;
		nCtrlZ += 1;
		action = new actionCtrlZ[nCtrlZ];
		for (int i=0; i<nCtrlZ; i++)
			action[i] = fooAction[i];
		delete[] fooAction;
		int* fooNSprite = new int[nEraseCtrlZ+1];
		int** fooPrio = new int*[nEraseCtrlZ+1];
		int** fooPass = new int*[nEraseCtrlZ+1];
		sprite** fooSprite = new sprite*[nEraseCtrlZ+1];
		string** fooString = new string*[nEraseCtrlZ+1];
		for (int i=0; i<nEraseCtrlZ; i++)
		{
			fooNSprite[i+1] = nSpriteErase[i];
			fooPrio[i+1] = new int[nSpriteErase[i]];
			fooPass[i+1] = new int[nSpriteErase[i]];
			fooSprite[i+1] = new sprite[nSpriteErase[i]];
			fooString[i+1] = new string[nSpriteErase[i]];
			for (int j=0; j<nSpriteErase[i]; j++)
			{
				fooPrio[i+1][j] = prioSpriteEraseCtrlZ[i][j];
				fooPass[i+1][j] = passSpriteEraseCtrlZ[i][j];
				fooSprite[i+1][j] = spriteEraseCtrlZ[i][j];
				fooString[i+1][j] = fileEraseCtrlZ[i][j];
			}
			delete[] prioSpriteEraseCtrlZ[i];
			delete[] passSpriteEraseCtrlZ[i];
			delete[] spriteEraseCtrlZ[i];
			delete[] fileEraseCtrlZ[i];
		}
		if (nEraseCtrlZ > 0)
		{
			delete[] nSpriteErase;
			delete[] prioSpriteEraseCtrlZ;
			delete[] passSpriteEraseCtrlZ;
			delete[] spriteEraseCtrlZ;
			delete[] fileEraseCtrlZ;
		}
		fooNSprite[0] = 1;
		fooPrio[0] = new int[1];
		fooPrio[0][0] = prio;
		fooPass[0] = new int[1];
		fooPass[0][0] = pass;
		fooString[0] = new string[1];
		fooString[0][0] = t;
		fooSprite[0] = new sprite[1];
		fooSprite[0][0] = s;
		
		nEraseCtrlZ += 1;
		nSpriteErase = new int[nEraseCtrlZ];
		prioSpriteEraseCtrlZ = new int*[nEraseCtrlZ];
		passSpriteEraseCtrlZ = new int*[nEraseCtrlZ];
		spriteEraseCtrlZ = new sprite*[nEraseCtrlZ];
		fileEraseCtrlZ = new string*[nEraseCtrlZ];
		for (int i=0; i<nEraseCtrlZ; i++)
		{
			nSpriteErase[i] = fooNSprite[i];
			prioSpriteEraseCtrlZ[i] = new int[nSpriteErase[i]];
			passSpriteEraseCtrlZ[i] = new int[nSpriteErase[i]];
			spriteEraseCtrlZ[i] = new sprite[nSpriteErase[i]];
			fileEraseCtrlZ[i] = new string[nSpriteErase[i]];
			for (int j=0; j<nSpriteErase[i]; j++)
			{
				prioSpriteEraseCtrlZ[i][j] = fooPrio[i][j];
				passSpriteEraseCtrlZ[i][j] = fooPass[i][j];
				spriteEraseCtrlZ[i][j] = fooSprite[i][j];
				fileEraseCtrlZ[i][j] = fooString[i][j];
			}
			delete[] fooPrio[i];
			delete[] fooPass[i];
			delete[] fooSprite[i];
			delete[] fooString[i];
		}
		delete[] fooNSprite;
		delete[] fooPrio;
		delete[] fooPass;
		delete[] fooSprite;
		delete[] fooString;	
	}
	else
	{
		int n = nSpriteErase[0];
		int* fooPrio = new int[n+1];
		int* fooPass = new int[n+1];
		sprite* fooSprite = new sprite[n+1];
		string* fooString = new string[n+1];
		for (int i=0; i<n; i++)
		{
			fooPrio[i] = prioSpriteEraseCtrlZ[0][i];
			fooPass[i] = passSpriteEraseCtrlZ[0][i];
			fooSprite[i] = spriteEraseCtrlZ[0][i];
			fooString[i] = fileEraseCtrlZ[0][i];
		}
		delete[] prioSpriteEraseCtrlZ[0];
		delete[] passSpriteEraseCtrlZ[0];
		delete[] spriteEraseCtrlZ[0];
		delete[] fileEraseCtrlZ[0];
		
		fooPrio[n] = prio;
		fooPass[n] = pass;
		fooSprite[n] = s;
		fooString[n] = t;
		nSpriteErase[0] += 1;
		n += 1;
		prioSpriteEraseCtrlZ[0] = new int[n];
		passSpriteEraseCtrlZ[0] = new int[n];
		spriteEraseCtrlZ[0] = new sprite[n];
		fileEraseCtrlZ[0] = new string[n];
		for (int i=0; i<n; i++)
		{
			prioSpriteEraseCtrlZ[0][i] = fooPrio[i];
			passSpriteEraseCtrlZ[0][i] = fooPass[i];
			spriteEraseCtrlZ[0][i] = fooSprite[i];
			fileEraseCtrlZ[0][i] = fooString[i];
		}	
		delete[] fooPrio;
		delete[] fooPass;
		delete[] fooSprite;
		delete[] fooString;	
	}
	elapsedTime = 0;
}

void mapCtrlZ::clearErasing()
{
	if (nCtrlZ > 1)
	{
		actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ-1];
		for (int i=1; i<nCtrlZ; i++)
			fooAction[i-1] = action[i];
		delete[] action;
		action = 0;
		nCtrlZ -= 1;
		action = new actionCtrlZ[nCtrlZ];
		for (int i=0; i<nCtrlZ; i++)
			action[i] = fooAction[i];
		delete[] fooAction;
	
		if (nEraseCtrlZ > 1)
		{
			int* fooNSprite = new int[nEraseCtrlZ-1];
			int** fooPrio = new int*[nEraseCtrlZ-1];
			int** fooPass = new int*[nEraseCtrlZ-1];
			sprite** fooSprite = new sprite*[nEraseCtrlZ-1];
			string** fooString = new string*[nEraseCtrlZ-1];
			for (int i=1; i<nEraseCtrlZ; i++)
			{
				fooNSprite[i-1] = nSpriteErase[i];
				fooPrio[i-1] = new int[nSpriteErase[i]];
				fooPass[i-1] = new int[nSpriteErase[i]];
				fooSprite[i-1] = new sprite[nSpriteErase[i]];
				fooString[i-1] = new string[nSpriteErase[i]];
				for (int j=0; j<nSpriteErase[i]; j++)
				{
					fooPrio[i-1][j] = prioSpriteEraseCtrlZ[i][j];
					fooPass[i-1][j] = passSpriteEraseCtrlZ[i][j];
					fooSprite[i-1][j] = spriteEraseCtrlZ[i][j];
					fooString[i-1][j] = fileEraseCtrlZ[i][j];
				}
				delete[] prioSpriteEraseCtrlZ[i];
				delete[] passSpriteEraseCtrlZ[i];
				delete[] spriteEraseCtrlZ[i];
				delete[] fileEraseCtrlZ[i];
			}
			delete[] nSpriteErase;
			delete[] prioSpriteEraseCtrlZ;
			delete[] passSpriteEraseCtrlZ;
			delete[] spriteEraseCtrlZ;
			delete[] fileEraseCtrlZ;
		
			nEraseCtrlZ -= 1;
			nSpriteErase = new int[nEraseCtrlZ];
			prioSpriteEraseCtrlZ = new int*[nEraseCtrlZ];
			passSpriteEraseCtrlZ = new int*[nEraseCtrlZ];
			spriteEraseCtrlZ = new sprite*[nEraseCtrlZ];
			fileEraseCtrlZ = new string*[nEraseCtrlZ];
			for (int i=0; i<nEraseCtrlZ; i++)
			{
				nSpriteErase[i] = fooNSprite[i];
				prioSpriteEraseCtrlZ[i] = new int[nSpriteErase[i]];
				passSpriteEraseCtrlZ[i] = new int[nSpriteErase[i]];
				spriteEraseCtrlZ[i] = new sprite[nSpriteErase[i]];
				fileEraseCtrlZ[i] = new string[nSpriteErase[i]];
				for (int j=0; j<nSpriteErase[i]; j++)
				{
					prioSpriteEraseCtrlZ[i][j] = fooPrio[i][j];
					passSpriteEraseCtrlZ[i][j] = fooPass[i][j];
					spriteEraseCtrlZ[i][j] = fooSprite[i][j];
					fileEraseCtrlZ[i][j] = fooString[i][j];
				}
				delete[] fooPrio[i];
				delete[] fooPass[i];
				delete[] fooSprite[i];
				delete[] fooString[i];
			}
			delete[] fooNSprite;
			delete[] fooPrio;
			delete[] fooPass;
			delete[] fooSprite;
			delete[] fooString;	
		}
		else
		{
			nEraseCtrlZ = 0;
			delete[] nSpriteErase;
			delete[] prioSpriteEraseCtrlZ[0];
			delete[] passSpriteEraseCtrlZ[0];
			delete[] spriteEraseCtrlZ[0];
			delete[] fileEraseCtrlZ[0];
			delete[] prioSpriteEraseCtrlZ;
			delete[] passSpriteEraseCtrlZ;
			delete[] spriteEraseCtrlZ;
			delete[] fileEraseCtrlZ;
			nSpriteErase = 0;
			prioSpriteEraseCtrlZ = 0;
			passSpriteEraseCtrlZ = 0;
			spriteEraseCtrlZ = 0;
			fileEraseCtrlZ = 0;
		}
	}
	else
	{
		nCtrlZ = 0;
		delete[] action;
		action = 0;
		nEraseCtrlZ = 0;
		delete[] nSpriteErase;
		delete[] prioSpriteEraseCtrlZ[0];
		delete[] passSpriteEraseCtrlZ[0];
		delete[] spriteEraseCtrlZ[0];
		delete[] fileEraseCtrlZ[0];
		delete[] prioSpriteEraseCtrlZ;
		delete[] passSpriteEraseCtrlZ;
		delete[] spriteEraseCtrlZ;
		delete[] fileEraseCtrlZ;
		nSpriteErase = 0;
		prioSpriteEraseCtrlZ = 0;
		passSpriteEraseCtrlZ = 0;
		spriteEraseCtrlZ = 0;
		fileEraseCtrlZ = 0;
	}
}

void mapCtrlZ::saveChangingPass(int prio, int dir, int ix, int iy, int lxS, int lyS)
{
	int pass = dir;
	actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ+1];
	for (int i=0; i<nCtrlZ; i++)
		fooAction[i+1] = action[i];
	if (action != 0)
		delete[] action;
	fooAction[0] = passOrNotCtrlZ;
	nCtrlZ += 1;
	action = new actionCtrlZ[nCtrlZ];
	for (int i=0; i<nCtrlZ; i++)
		action[i] = fooAction[i];
	delete[] fooAction;
	
	int** fooNPass = new int*[nPassOrNotCtrlZ+1];
	int** fooPos = new int*[nPassOrNotCtrlZ+1];
	int* fooPass = new int[nPassOrNotCtrlZ+1];
	int* fooPrio = new int[nPassOrNotCtrlZ+1];
	for (int i=0; i<nPassOrNotCtrlZ; i++)
	{
		fooNPass[i+1] = new int[2];
		fooNPass[i+1][0] = nPassCtrlZ[i][0];
		fooNPass[i+1][1] = nPassCtrlZ[i][1];
		fooPos[i+1] = new int[2];
		fooPos[i+1][0] = initPosPassCtrlZ[i][0];
		fooPos[i+1][1] = initPosPassCtrlZ[i][1];
		fooPass[i+1] = passCtrlZ[i];
		fooPrio[i+1] = prioPassCtrlZ[i];
		delete[] nPassCtrlZ[i];
		delete[] initPosPassCtrlZ[i];
	}
	if (nSpriteAdd != 0)
	{
		delete[] nPassCtrlZ;
		delete[] initPosPassCtrlZ;
		delete[] passCtrlZ;
		delete[] prioPassCtrlZ;
	}
	fooNPass[0] = new int[2];
	fooNPass[0][0] = lxS;
	fooNPass[0][1] = lyS;
	fooPos[0] = new int[2];
	fooPos[0][0] = ix;
	fooPos[0][1] = iy;
	fooPass[0] = pass;
	fooPrio[0] = prio;
	nPassOrNotCtrlZ += 1;
	nPassCtrlZ = new int*[nPassOrNotCtrlZ];
	initPosPassCtrlZ = new int*[nPassOrNotCtrlZ];
	passCtrlZ = new int[nPassOrNotCtrlZ];
	prioPassCtrlZ = new int[nPassOrNotCtrlZ];
	for (int i=0; i<nPassOrNotCtrlZ; i++)
	{
		nPassCtrlZ[i] = new int[2];
		nPassCtrlZ[i][0] = fooNPass[i][0];
		nPassCtrlZ[i][1] = fooNPass[i][1];
		initPosPassCtrlZ[i] = new int[2];
		initPosPassCtrlZ[i][0] = fooPos[i][0];
		initPosPassCtrlZ[i][1] = fooPos[i][1];
		passCtrlZ[i] = fooPass[i];			
		prioPassCtrlZ[i] = fooPrio[i];			
		delete[] fooNPass[i];
		delete[] fooPos[i];
	}
	delete[] fooNPass;
	delete[] fooPos;
	delete[] fooPass;
	delete[] fooPrio;
}

void mapCtrlZ::clearChangingPass()
{
	if (nCtrlZ > 1)
	{
		actionCtrlZ* fooAction = new actionCtrlZ[nCtrlZ-1];
		for (int i=1; i<nCtrlZ; i++)
			fooAction[i-1] = action[i];
		delete[] action;
		nCtrlZ -= 1;
		action = new actionCtrlZ[nCtrlZ];
		for (int i=0; i<nCtrlZ; i++)
			action[i] = fooAction[i];
		delete[] fooAction;
		
		if (nPassOrNotCtrlZ > 1)
		{
			int** fooNPass = new int*[nPassOrNotCtrlZ-1];
			int** fooPos = new int*[nPassOrNotCtrlZ-1];
			int* fooPass = new int[nPassOrNotCtrlZ-1];
			int* fooPrio = new int[nPassOrNotCtrlZ-1];
			for (int i=1; i<nPassOrNotCtrlZ; i++)
			{
				fooNPass[i-1] = new int[2];
				fooNPass[i-1][0] = nPassCtrlZ[i][0];
				fooNPass[i-1][1] = nPassCtrlZ[i][1];
				fooPos[i-1] = new int[2];
				fooPos[i-1][0] = initPosPassCtrlZ[i][0];
				fooPos[i-1][1] = initPosPassCtrlZ[i][1];
				fooPass[i-1] = passCtrlZ[i];
				fooPrio[i-1] = prioPassCtrlZ[i];
				delete[] nPassCtrlZ[i];
				delete[] initPosPassCtrlZ[i];
			}
			delete[] nPassCtrlZ;
			delete[] initPosPassCtrlZ;
			delete[] passCtrlZ;
			delete[] prioPassCtrlZ;
			
			nPassOrNotCtrlZ -= 1;
			nPassCtrlZ = new int*[nPassOrNotCtrlZ];
			initPosPassCtrlZ = new int*[nPassOrNotCtrlZ];
			passCtrlZ = new int[nPassOrNotCtrlZ];
			prioPassCtrlZ = new int[nPassOrNotCtrlZ];
			for (int i=0; i<nPassOrNotCtrlZ; i++)
			{
				nPassCtrlZ[i] = new int[2];
				nPassCtrlZ[i][0] = fooNPass[i][0];
				nPassCtrlZ[i][1] = fooNPass[i][1];
				initPosPassCtrlZ[i] = new int[2];
				initPosPassCtrlZ[i][0] = fooPos[i][0];
				initPosPassCtrlZ[i][1] = fooPos[i][1];
				passCtrlZ[i] = fooPass[i];			
				prioPassCtrlZ[i] = fooPrio[i];			
				delete[] fooNPass[i];
				delete[] fooPos[i];
			}
			delete[] fooNPass;
			delete[] fooPos;
			delete[] fooPass;
			delete[] fooPrio;
		}
		else
		{
			nPassOrNotCtrlZ = 0;
			delete[] nPassCtrlZ[0];
			delete[] initPosPassCtrlZ[0];
			delete[] nPassCtrlZ;
			delete[] initPosPassCtrlZ;
			delete[] passCtrlZ;
			delete[] prioPassCtrlZ;
		}
	}
	else
	{
		nCtrlZ = 0;
		delete[] action;
		action = 0;
		nPassOrNotCtrlZ = 0;
		delete[] nPassCtrlZ[0];
		delete[] initPosPassCtrlZ[0];
		delete[] nPassCtrlZ;
		delete[] initPosPassCtrlZ;
		delete[] passCtrlZ;
		delete[] prioPassCtrlZ;
		nPassCtrlZ = 0;
		initPosPassCtrlZ = 0;
		passCtrlZ = 0;
		prioPassCtrlZ = 0;
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
