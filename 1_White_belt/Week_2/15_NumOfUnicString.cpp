#include <iostream>
#include <set>
#include <string>
using namespace std;

int main()
{
	int q;
	cin >> q;

	set<string> s;
	for (int i = 0; i < q; ++i)
	{
		string in;
		cin >> in;
		s.insert(in);
	}
	cout << s.size();
}
