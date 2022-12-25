#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <typename T>
class ObjectPool {
public:
    T* Allocate(){
        if (free.empty()){
            T* in = new T;
            used.insert(in);
            return in;
        }
        else{
            T* in = free.front();
            used.insert(in);
            free.pop();
            return in;
        }
    }
    T* TryAllocate(){
        if (free.empty()){
            return nullptr;
        }
        else{
            T* in = free.front();
            used.insert(in);
            free.pop();
            return in;
        }
    }

    void Deallocate(T* object){
        if (used.count(object)){
            free.push(object);
            used.erase(object);
        }
        else{
            throw invalid_argument("ObjectPool::Deallocate - invalid_argument");
        }
    }

    ~ObjectPool(){
        while (!free.empty()){
            delete free.front();
            free.pop();
        }
        while (!used.empty()){
            delete *used.begin();
            used.erase(used.begin());
        }
    }

private:
    set<T*> used;
    queue<T*> free;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}