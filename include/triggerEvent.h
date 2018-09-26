#ifndef TRIGGEREVENT_H_INCLUDED
#define TRIGGEREVENT_H_INCLUDED

#include "character.h"
#include <cmath>
using namespace std;

bool cross(hero* h, int x, int y);
bool action(hero* h, int x, int y);
bool boundaryMap(hero* h, int x, int y, int dir);
bool gate(hero* h, int x, int y, int dir);
bool interactionStaticPNJ(hero* h, int x, int y);

#endif
