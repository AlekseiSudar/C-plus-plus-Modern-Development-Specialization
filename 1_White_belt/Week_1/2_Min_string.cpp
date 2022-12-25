#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    vector<string> a;
    for (int i = 0; i < 3; i++)
    {
        string b;
        cin >> b;
        a.push_back(b);
    }
    sort(a.begin(), a.end());
    cout << a[0];
}