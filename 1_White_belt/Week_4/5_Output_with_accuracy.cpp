#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main() {
    ifstream input("input.txt");
    double value;
    
    cout << fixed << setprecision(3);
    while (input >> value) {
        cout << value << endl;
    }

    return 0;
}
