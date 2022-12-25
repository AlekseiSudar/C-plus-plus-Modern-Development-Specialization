#pragma once
#include <string>
#include <algorithm>

class Events {
public:

	void Add(string event) {
		data_set.insert(event);
		data_vector.push_back(event);
	}

	vector<string> GetVector() const {
		return data_vector;
	}
	set<string> GetSet() const {
		return data_set;
	}

	template <typename function>
	int RemoveIf(function f) {

		auto split_point = stable_partition(data_vector.begin(), data_vector.end(), f);

		int result = split_point - data_vector.begin();

		for (auto i = data_vector.begin(); i != split_point; ++i) {
			data_set.erase(*i);
		}
		data_vector.erase(data_vector.begin(), split_point);

		return result;
	}

private:
	vector<string> data_vector;
	set<string> data_set;
};
