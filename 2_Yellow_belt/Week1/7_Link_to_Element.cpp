#include <iostream>
#include <map>
using namespace std;

template<typename Key, typename Value>
Value& GetRefStrict(map<Key, Value>& Map, Key key);

template<typename Key, typename Value>
Value& GetRefStrict(map<Key, Value>& Map, Key key) {
	if (Map.count(key))
		return Map[key];
	else
		throw(runtime_error("runtime_error"));
}


int main() {
	map<int, string> m = { {0, "value"} };
	string& item = GetRefStrict(m, 0);
	item = "newvalue";
	cout << m[0] << endl;
}