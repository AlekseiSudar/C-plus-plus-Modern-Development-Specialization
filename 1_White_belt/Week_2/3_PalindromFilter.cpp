#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool IsPalindrom(string s)
{
    for (int i = 0; i < s.size() / 2; ++i)
        if (s[i] != s[s.size() - (1 + i)])
            return false;
    return true;
}

vector<string> PalindromFilter(vector<string> words, int minLength)
{
    vector<string> out;
    for (auto w : words)
        if (IsPalindrom(w) == 1 && w.size() >= minLength)
            out.push_back(w);
    return out;
}