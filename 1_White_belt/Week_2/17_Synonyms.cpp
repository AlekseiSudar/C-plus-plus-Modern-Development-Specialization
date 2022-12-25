#include <iostream>
#include <set>
#include <map>
#include <string>
using namespace std;

int main()
{
	int q;
	cin >> q;

	map<string, set<string>> synonyms;

	for (int iter1 = 0; iter1 < q; ++iter1)
	{
		string comm;
		cin >> comm;

		if (comm == "ADD")
		{
			string word1, word2;
			cin >> word1 >> word2;

			synonyms[word1].insert(word2);
			synonyms[word2].insert(word1);
		}
		else if (comm == "COUNT")
		{
			string word;
			cin >> word;

			cout << synonyms[word].size() << endl;
		}
		else if (comm == "CHECK")
		{
			string word1, word2;
			cin >> word1 >> word2;

			if (synonyms[word1].count(word2))
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
		}
	}

	return 0;
}
