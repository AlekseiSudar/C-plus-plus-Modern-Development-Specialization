#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

int main() {

	string expr;
	int num_oper;

	cin >> expr >> num_oper;

	for (int i = 0; i < num_oper; ++i) {
		expr.insert(expr.begin(), '(');
		string oper;
		string num;
		cin >> oper >> num;
		expr += ") " + oper + ' ' + num;
	}

	cout << expr;


	
	return 0;
}