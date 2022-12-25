#include "database.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, const  pair<Date, string>& pair) {
	os << pair.first << ' ' << pair.second;
	return os;
}
