#include <iostream>
#include <vector>
using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<bool> line;
	vector<int> out;

	for (int i = 0; i < n; ++i)
	{
		string command1;
		cin >> command1;

		if (command1 == "WORRY_COUNT")
		{
			int worry = 0;
			for (auto l : line)
				if (l)
					++worry;
			cout << worry << endl;
		}
		else
		{
			int arg;
			cin >> arg;

			if (command1 == "WORRY")
				line[arg] = true;
			else if (command1 == "QUIET")
				line[arg] = false;
			else if (command1 == "COME")
				line.resize(line.size() + arg);
		}
	}


	return 0;
}