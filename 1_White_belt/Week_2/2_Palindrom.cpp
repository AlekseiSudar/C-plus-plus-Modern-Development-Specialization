#include <iostream>
#include <string>
using namespace std;

bool IsPalindrom(string s)
{
    for (int i = 0; i < s.size() / 2; ++i)
        if (s[i] != s[s.size() - (1 + i)])
            return false;
    return true;
}