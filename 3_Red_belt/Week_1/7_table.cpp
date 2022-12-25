#include "test_runner.h"
#include <vector>

using namespace std;

template<typename T>
class Table{
public:
  Table(size_t rows, size_t colomns){
    for (size_t i = 0; i < rows; i++)
      data_.push_back(vector<T>(colomns, T()));
  }

  vector<T>& operator[](size_t row){
    return data_[row];
  }
  vector<T>& operator[](size_t row) const{
    return data_[row];
  }

  pair<size_t, size_t> Size() const{
    if (data_.empty() || data_[0].empty())
      return {0, 0};
    return {data_.size(), data_[0].size()};
  }

  void Resize(size_t rows, size_t colomns){
    data_.resize(rows);
    for (auto& row : data_){
      row.resize(colomns);
    }
  }

private:
  vector<vector<T>> data_;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
