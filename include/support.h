#ifndef SUPPORT_H_INCLUDED
#define SUPPORT_H_INCLUDED

#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;

string unsignedIntToString(unsigned int i);
unsigned int stringToUnsignedInt(string s);
const unsigned int constStringToUnsignedInt(string s);

#endif 

