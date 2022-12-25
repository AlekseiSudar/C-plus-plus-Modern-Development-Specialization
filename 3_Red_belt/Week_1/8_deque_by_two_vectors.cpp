#include "test_runner.h"
#include <vector>
using namespace std;

template<typename T>
class Deque{
public:
    Deque(){ }

    bool Empty() const{ return v_front.empty() && v_back.empty(); }
    size_t Size() const{ return v_front.size() + v_back.size(); }

    T& At(size_t index){
        if (index < (this->Size())){
            if (index < v_front.size())
                return *(v_front.rbegin() + index);
            else
                return *(v_back.begin() + index - v_front.size());
        }
        throw out_of_range("Deque: out_of_range");
    }

    const T& At(size_t index) const{
        if (index < (this->Size())){
            if (index < v_front.size())
                return *(v_front.rbegin() + index);
            else
                return *(v_back.begin() + index - v_front.size());
        }
        throw out_of_range("Deque: out_of_range");
    }

    T& operator[](size_t index){ return this->At(index); }
    const T& operator[](size_t index) const{ return this->At(index); }

    T& Front(){ return v_front.empty() ? *v_back.begin() : *v_front.rbegin(); }
    const T& Front() const{ return v_front.empty() ? *v_back.begin() :*v_front.rbegin(); }

    T& Back(){ return v_back.empty() ? *v_front.begin() : *v_back.rbegin(); }
    const T& Back() const{ return v_back.empty() ? *v_front.begin() : *v_back.rbegin(); }

    void PushFront(const T& elem){ v_front.push_back(elem); }
    void PushBack(const T& elem){ v_back.push_back(elem); }

private:
    vector<T> v_front;
    vector<T> v_back;
};

/*
void TestDeque() {
    Deque<int> d;

    ASSERT_EQUAL(d.Empty(), true);

    d.PushFront(2);
    d.PushFront(1);
    d.PushFront(0);

    ASSERT_EQUAL(d.Front(), 0);
    ASSERT_EQUAL(d.Back(), 2);

    d.PushBack(3);
    d.PushBack(4);
    d.PushBack(5);

    ASSERT_EQUAL(d.Size(), 6u);
    ASSERT_EQUAL(d.At(0), 0);
    ASSERT_EQUAL(d.At(1), 1);
    ASSERT_EQUAL(d.At(2), 2);
    ASSERT_EQUAL(d.At(3), 3);
    ASSERT_EQUAL(d.At(4), 4);
    ASSERT_EQUAL(d.At(5), 5);

    ASSERT_EQUAL(d[0], 0);
    ASSERT_EQUAL(d[1], 1);
    ASSERT_EQUAL(d[2], 2);
    ASSERT_EQUAL(d[3], 3);
    ASSERT_EQUAL(d[4], 4);
    ASSERT_EQUAL(d[5], 5);

    ASSERT_EQUAL(d.Front(), 0);
    ASSERT_EQUAL(d.Back(), 5);

    Deque<int> d2;
    d2.PushBack(3);
    d2.PushBack(4);
    d2.PushBack(5);

    ASSERT_EQUAL(d2.Front(), 3);
    ASSERT_EQUAL(d2.Back(), 5);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestDeque);
    return 0;
}
*/