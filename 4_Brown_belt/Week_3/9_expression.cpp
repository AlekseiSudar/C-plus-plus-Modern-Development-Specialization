#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;


ExpressionPtr Value(int value){
    class Value : public Expression {
    public:
        Value(int value) : value_(value) {}
        int Evaluate() const override { return value_; }
        string ToString() const override { return std::to_string(value_); }
    private:
        int value_;
    };

    return make_unique<Value>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right){
    class Sum : public Expression {
    public:
        Sum(ExpressionPtr left, ExpressionPtr right) :
            left_(move(left)), right_(move(right)) {}
        int Evaluate() const override {
            return left_->Evaluate() + right_->Evaluate();
        }
        string ToString() const override {
            return '(' + left_->ToString() + ')' + '+' + '(' + right_->ToString() + ')';
        }
    private:
        ExpressionPtr left_, right_;
    };

    return make_unique<Sum>(move(left), move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right){
    class Product : public Expression {
    public:
        Product(ExpressionPtr left, ExpressionPtr right) :
            left_(move(left)), right_(move(right)) {}
        int Evaluate() const override {
            return left_->Evaluate() * right_->Evaluate();
        }
        string ToString() const override {
            return '(' + left_->ToString() + ')' + '*' + '(' + right_->ToString() + ')';
        }
    private:
        ExpressionPtr left_, right_;
    };

    return make_unique<Product>(move(left), move(right));
}

string Print(const Expression* e) {
    if (!e) {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

void Test() {
    ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
    ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

    ExpressionPtr e2 = Sum(move(e1), Value(5));
    ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

    ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}