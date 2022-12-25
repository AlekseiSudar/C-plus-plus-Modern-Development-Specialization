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
	auto it1 = first + (last - first) / 3;
	auto it2 = first + (last - first) / 3 * 2;

	MergeSort(first, it1);
	MergeSort(it1, it2);
	MergeSort(it2, last);

	vector<typename RandomIt::value_type> buf;
	merge(first, it1, it1, it2, back_inserter(buf));
	merge(buf.begin(), buf.end(), it2, last, range_begin);
}