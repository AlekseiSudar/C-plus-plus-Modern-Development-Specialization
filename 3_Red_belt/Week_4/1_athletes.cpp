#include <iostream>
#include <list>
#include <map>
#include <algorithm>

using namespace std;


int main() {
	list<int> athletes;
	map<int, list<int>::iterator> its;

	

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		int current, next;
		cin >> current >> next;

		if (its.count(next))
			its[current] = athletes.insert(its[next], current);
		else {
			athletes.push_back(current);
			its[current] = --(athletes.end());
		}

	}

	for (auto& athlete : athletes)
		cout << athlete << '\n';

	return 0;
}