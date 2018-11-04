#ifndef SUPPORTWINDOW_H_INCLUDED
#define SUPPORTWINDOW_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;

class mapi;

int quitWindow(sf::RenderWindow* w);
int saveWindow(sf::RenderWindow* w);
int loadWindow(sf::RenderWindow* w);
int reinitWindow(sf::RenderWindow* w);
void newMapWindow(sf::RenderWindow* w, mapi* M);
vector<int> changeSizeMapWindow(sf::RenderWindow* w, mapi* M);

#endif

