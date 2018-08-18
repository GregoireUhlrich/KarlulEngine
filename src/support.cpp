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
	return i;
}

