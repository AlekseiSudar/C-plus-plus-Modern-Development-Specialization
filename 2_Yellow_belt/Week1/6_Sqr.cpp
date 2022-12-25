#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
using namespace std;



template<typename F, typename S> pair<F, S> operator*(pair<F, S> lhs, pair<F, S> rhs);
template<typename T> vector<T> operator*(vector<T> lhs, vector<T> rhs);
template<typename Key, typename Value> map<Key, Value> operator*(map<Key, Value> lhs, map<Key, Value> rhs);
template<typename T> T Sqr(T x);

template<typename F, typename S>
pair<F, S> operator*(pair<F, S> lhs, pair<F, S> rhs) {
	return make_pair(
		lhs.first * rhs.first,
		lhs.second * rhs.second);
}

template<typename T>
vector<T> operator*(vector<T> lhs, vector<T> rhs) {
	vector<T> result;
	if (lhs.size() == rhs.size())
		for (int n = lhs.size(), i = 0; i < n; ++i)
			result.push_back(lhs[i] * rhs[i]);
	return result;
}

template<typename Key, typename Value>
map<Key, Value> operator*(map<Key, Value> lhs, map<Key, Value> rhs) {
	map<Key, Value> result;
	for (auto& i : lhs)
		if (rhs.count(i.first))
			result[i.first] = i.second * rhs[i.first];
	return result;
}


template<typename T>
T Sqr(T x) {
	return x * x;
}






int main() {
	vector<int> v = { 1, 2, 3 };
	cout << "vector:";
	for (int x : Sqr(v)) {
		cout << ' ' << x;
	}
	cout << endl;

	map<int, pair<int, int>> map_of_pairs = {
	  {4, {2, 2}},
	  {7, {4, 3}}
	};
	cout << "map of pairs:" << endl;
	for (const auto& x : Sqr(map_of_pairs)) {
		cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
	}

	return 0;
}