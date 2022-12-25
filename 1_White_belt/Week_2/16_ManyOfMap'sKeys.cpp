#include <iostream>
#include <set>
#include <map>
#include <string>
using namespace std;

set<string> BuildMapValuesSet(const map<int, string>& strings)
{
    set<string> values;
    for (const auto& i : strings)
        values.insert(i.second);
    return values;
}
