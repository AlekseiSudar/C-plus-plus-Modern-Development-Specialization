#pragma once
#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size_(a_size) {
        if (a_size > N)
            throw invalid_argument("invalid_argument");
    }

    T& operator[](size_t index){
        if (index < size_)
            return data_[index];
        throw invalid_argument("invalid_argument");
    }
    const T& operator[](size_t index) const{
        if (index < size_)
            return data_[index];
        throw invalid_argument("invalid_argument");
    }

    auto begin(){ return data_.begin(); }
    auto end(){ return data_.begin() + size_; }
    auto begin() const{ return data_.begin(); }
    auto end() const{ return data_.begin() + size_; }

    size_t Size() const{
        return size_;
    }
    size_t Capacity() const {
        return data_.size();
    }

    void PushBack(const T& value){
        if (size_ == data_.size())
            throw overflow_error("overflow_error");
        data_[size_++] = value;
    }
    T PopBack(){
        if (size_ == 0)
            throw underflow_error("underflow_error");
        return data_[--size_];
    }

private:
    array<T, N> data_;
    size_t size_;
};

