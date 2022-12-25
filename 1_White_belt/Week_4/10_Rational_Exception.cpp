#include <iostream>
#include <exception>
#include <numeric>
using namespace std;

class Rational {
public:
    Rational() {
        numerator = 0;
        denominator = 1;
    }
    Rational(int num, int den) {
        if (den == 0)
            throw invalid_argument(" ");
        else if (den > 0) {
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

Rational operator/(const Rational& l, const Rational& r) {
    if (r.Numerator() == 0)
        throw domain_error(" ");
    return Rational(
        l.Numerator() * r.Denominator(),
        l.Denominator() * r.Numerator()
    );
}