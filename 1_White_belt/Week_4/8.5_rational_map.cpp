#include <iostream>
#include <sstream>
#include <numeric>
#include <set>
#include <map>
#include <vector>
using namespace std;

class Rational {
public:
    Rational() {
        numerator = 0;
        denominator = 1;
    }

    Rational(int num, int den) {
        if (den > 0) {
            int div = gcd(num, den);
            numerator = num / div;
            denominator = den / div;
        }
        else {
            int div = gcd(num, den);
            numerator = -num / div;
            denominator = -den / div;
        }
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

private:
    int numerator, denominator;
};

bool operator==(const Rational& r, const Rational& l) {
    if (r.Numerator() == l.Numerator())
        if (r.Denominator() == l.Denominator())
            return true;
    return false;
}


bool operator<(const Rational& r, const Rational& l) {
    int right = r.Numerator() * l.Denominator();
    int left = l.Numerator() * r.Denominator();
    if (right < left)
        return true;
    return false;
}