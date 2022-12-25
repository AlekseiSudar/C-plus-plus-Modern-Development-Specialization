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
            throw invalid_argument("Invalid argument");
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

Rational operator+(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Denominator() + l.Numerator() * r.Denominator(),
        r.Denominator() * l.Denominator());
}
Rational operator-(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Denominator() - l.Numerator() * r.Denominator(),
        r.Denominator() * l.Denominator());
}
Rational operator*(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Numerator(),
        r.Denominator() * l.Denominator()
    );
}
Rational operator/(const Rational& l, const Rational& r) {
    if (r.Numerator() == 0)
        throw domain_error("Division by zero");
    return Rational(
        l.Numerator() * r.Denominator(),
        l.Denominator() * r.Numerator()
    );
}

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

ostream& operator<<(ostream& stream, const Rational& value) {
    stream << value.Numerator() << '/' << value.Denominator();
    return stream;
}
istream& operator>>(istream& stream, Rational& value) {
    int num, div;
    char d;
    if (stream >> num &&
        stream >> d &&
        stream >> div)
    {
        if (d == '/')
            value = Rational(num, div);
    }
    return stream;
}

int main() {
    Rational r1, r2;
    char c;
    try {
        cin >> r1 >> c >> r2;
    }
    catch (invalid_argument& a) {
        cout << a.what();
        return 0;
    }

    if (c == '+')
        cout << r1 + r2;
    if (c == '-')
        cout << r1 - r2;
    if (c == '*')
        cout << r1 * r2;
    if (c == '/')
    {
        try {
            cout << r1 / r2;
        }
        catch (domain_error& a) {
            cout << a.what();
            return 0;
        }
    }

    return 0;
}