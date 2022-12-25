#include <cstdint>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
public:
    SimpleVector() = default;
    explicit SimpleVector(size_t size) : data_(new T[size]), end_(data_ + size), capacity_(size) {}
    ~SimpleVector() { delete[] data_; }

    T& operator[](size_t index) { return data_[index]; }

    T* begin() { return data_; }
    T* end() { return end_; }

    T* begin() const { return data_; }
    T* end() const { return end_; }

    size_t Size() const { return end_ - data_; }
    size_t Capacity() const { return capacity_; }
    void PushBack(T value) {
        if (end_ - data_ == capacity_) {
            capacity_ = capacity_ == 0 ? 1 : capacity_ * 2;
            T* new_data = new T[capacity_];
            move(data_, end_, new_data);
            delete[] data_;
            data_ = new_data;
            end_ = data_ + capacity_ / 2;
        }
        *(end_++) = move(value);
    }

private:
    T* data_ = nullptr;
    T* end_ = data_;
    size_t capacity_ = 0;
};
