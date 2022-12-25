#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) return;

    vector<typename RandomIt::value_type> values(
            make_move_iterator(range_begin),
            make_move_iterator(range_end)
            );

    auto it0 = values.begin();
    auto it1 = values.begin() + values.size() / 3;
    auto it2 = values.end() - values.size() / 3;
    auto it3 = values.end();

    MergeSort(it0, it1);
    MergeSort(it1, it2);
    MergeSort(it2, it3);

    vector<typename RandomIt::value_type> buffer;
    merge(
            make_move_iterator(it0),
            make_move_iterator(it1),
            make_move_iterator(it1),
            make_move_iterator(it2),
            back_inserter(buffer)
            );

    merge(
            make_move_iterator(buffer.begin()),
            make_move_iterator(buffer.end()),
            make_move_iterator(it2),
            make_move_iterator(it3),
            range_begin
            );
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
