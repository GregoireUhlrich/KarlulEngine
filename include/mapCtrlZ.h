#ifndef MAPCTRLZ_H_INCLUDED
#define MAPCTRLZ_H_INCLUDED

#include "../include/map.h"

enum actionCtrlZ{ addCtrlZ, eraseCtrlZ, supprCtrlZ, passOrNotCtrlZ};
	
class mapCtrlZ
{
	private:
	
	mapi* M;
	int xSprites, ySprites;
	int memoryCtrlZ;
	int nCtrlZ;
	actionCtrlZ* action;
	int nAddCtrlZ;
	int** nSpriteAdd;
	int** posAddCtrlZ;
	int* prioAddCtrlZ;
	int nEraseCtrlZ;
	int* nSpriteErase;
	int** prioSpriteEraseCtrlZ;
	int** passSpriteEraseCtrlZ;
	sprite** spriteEraseCtrlZ;
	int** nPassCtrlZ;
	int** initPosPassCtrlZ;
	int*** passCtrlZ;
	
	public:
	
	mapCtrlZ(mapi* Mi);
	~mapCtrlZ();
	
	void saveAdding(sprite s, int prio, int lxS, int lyS);
	void clearAdding();
	
	void CtrlZ();
	
};

#endif
