#include "../include/support.h"
using namespace std;


string unsignedIntToString(unsigned int i)
{
    stringstream stream;
    stream<<i;
    return stream.str();
}

unsigned int stringToUnsignedInt(string s)
{
    unsigned int i;
    istringstream (s) >> i;
    if (s == "") i = 0;
    return i;
}

const unsigned int constStringToUnsignedInt(string s)
{
    unsigned int i;
    istringstream (s) >> i;
    const int iReturn = i;
    return iReturn;
}

