#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s;
    cin >> s;

    int i = 0;
    int num = 0;
    for (auto c : s)
    {
        if (c == 'f')
        {
            if (num == 1)
            {
                cout << i;
                return 0;
            }
            ++num;
        }
        ++i;
    }
    if (num == 0)
        cout << -2;
    else
        cout << -1;
    return 0;
}