#include "test_runner.h"
#include <cstddef>    // нужно для nullptr_t

using namespace std;



// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* data_;
public:
    UniquePtr() : data_(nullptr) {}
    UniquePtr(T * ptr) : data_(ptr) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) : data_(other.Release()) {}
    UniquePtr& operator = (const UniquePtr&) = delete;
    UniquePtr& operator = (nullptr_t){
        if (data_) delete data_;
        data_ = nullptr;
        return *this;
    }
    UniquePtr& operator = (UniquePtr&& other){
        if (data_) delete data_;
        data_ = other.Release();
        return *this;
    }
    ~UniquePtr(){
        if (data_) delete data_;
    }

    T& operator * () const{
        return *data_;
    }

    T * operator -> () const{
        return data_;
    }

    T * Release(){
        T* ans = data_;
        data_ = nullptr;
        return ans;
    }

    void Reset(T * ptr){
        if (data_) delete data_;
        data_ = ptr;
    }

    void Swap(UniquePtr& other){
        T* buf = data_;
        data_ = other.Release();
        other.Reset(buf);
    }

    T * Get() const{
        return data_;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0): value(v) {
        ++counter;
    }
    Item(const Item& other): value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}
