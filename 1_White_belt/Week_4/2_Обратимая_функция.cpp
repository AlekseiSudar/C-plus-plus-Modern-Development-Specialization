#include <iostream>
#include <string>
#include <vector>
using namespace std;

class FunctionPart {
public:
    FunctionPart(const char& new_operation, const double& new_value) {
        operation = new_operation;
        value = new_value;
    }
    double Apply(const double& sourse_value) const {
        if (operation == '+')
            return sourse_value + value;
        else if (operation == '-')
            return sourse_value - value;
    }
    void Invert() {
        if (operation == '+')
            operation = '-';
        else if (operation == '-')
            operation = '+';
    }
private:
    char operation;
    double value;
};

class Function {
public:
    void AddPart(const char& operation, const double& value)
    {
        parts.push_back({ operation, value });
    };
    double Apply(const double& value) const {
        double result = value;
        for (const auto& part : parts)
            result = part.Apply(result);
        return result;
    };
    void Invert() {
        for (auto& part : parts)
            part.Invert();
        reverse(parts.begin(), parts.end());
    };
private:
    vector<FunctionPart> parts;
};