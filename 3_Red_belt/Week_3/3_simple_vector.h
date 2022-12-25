#pragma once

#include <cstdlib>

// � еализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
	SimpleVector() : data_(nullptr), end_(nullptr), capacity_(0) {}
	explicit SimpleVector(size_t size) : data_(new T[size]), end_(data_ + size), capacity_(size) {}

	~SimpleVector() { delete[] data_; }

	T& operator[](size_t index) { return data_[index]; }

	T* begin() { return data_; }
	T* end() { return end_; }

	size_t Size() const { return end_ - data_; }
	size_t Capacity() const { return capacity_; }
	void PushBack(const T& value) {
		if (data_ == nullptr) {
			data_ = new T[1];
			data_[0] = value;
			end_ = data_ + 1;
			capacity_ = 1;
		}
		else if (this->Size() != capacity_) {
			data_[this->Size()] = value;
			end_++;
		}
		else {
			T* data_new = new T[capacity_*2];
			for (size_t i = 0, n = this->Size(); i < n; i++)
				data_new[i] = data_[i];
			data_new[this->Size()] = value;

			end_ = data_new + this->Size() + 1;
			delete[] data_;
			data_ = data_new;
			capacity_ = 2 * capacity_;
		}
	}

private:
	T* data_;
	T* end_;
	size_t capacity_;
};