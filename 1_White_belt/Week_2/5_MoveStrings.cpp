#include <iostream>
#include <string>
#include <vector>

void MoveStrings(vector<string>& source, vector<string>& destination)
{
    for (auto i : source)
        destination.push_back(i);
    source.clear();
}