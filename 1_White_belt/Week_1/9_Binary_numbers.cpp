#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

int main()
{
    int n;
    cin >> n;

    vector<int> res;
    while (n > 0)
    {
        res.push_back(n % 2);
        n /= 2;
    }

    for (int i = res.size() - 1; i == 0; --i)
        res.erase(res.begin() + i);

    for (int i = res.size() - 1; i >= 0; --i)
        cout << res[i];

}
