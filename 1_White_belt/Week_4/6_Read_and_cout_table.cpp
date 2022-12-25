#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

int main() {
    ifstream input("input.txt");
    double n1, n2;;
    input >> n1 >> n2;
    
    vector<vector<int>> table;

    for (int i = 0; i < n1; ++i) {
        table.push_back({});
        for (int j = 0; j < n2; ++j) {
            int value;
            input >> value;
            input.ignore(1);
            table.back().push_back(value);
        }
    }

    for (int i = 0; i < n1; ++i) {
        
        for (int j = 0; j < n2; ++j) {
            cout << setw(10);
            cout << table[i][j];
            if (j < n2 - 1)
                cout << " ";
        }
        if (i < n1 - 1)
            cout << endl;
    }
    
    return 0;
}
