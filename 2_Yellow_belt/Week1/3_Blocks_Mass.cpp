#include <iostream>
#include <vector>
using namespace std;

int main() {
	uint64_t mass = 0;

	size_t n;
	cin >> n;

	uint64_t ro;
	cin >> ro;
	unsigned int a, b, c;
	for (size_t i = 0; i < n; ++i) {
		cin >> a >> b >> c;
		mass += ro * a * b * c;
	}

	cout << mass;


	return 0;
}