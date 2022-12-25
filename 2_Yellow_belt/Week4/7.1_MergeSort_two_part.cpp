#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	if (range_end - range_begin < 2)
		return;

	vector<typename RandomIt::value_type> diap(range_begin, range_end);
	auto first = begin(diap);
	auto last = end(diap);
	auto it = first + (last - first) / 2;

	MergeSort(first, it);
	MergeSort(it, last);

	merge(first, it, it, last, range_begin);
}