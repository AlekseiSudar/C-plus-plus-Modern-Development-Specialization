#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
	int n;
	cin >> n;

	for (int i = 0; i < n; ++i)
	{
		string word1, word2;
		cin >> word1 >> word2;

		map<char, int> w1, w2;
		for (auto i : word1)
			++w1[i];
		for (auto i : word2)
			++w2[i];

		if (w1 == w2)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
}
