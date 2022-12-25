#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <optional>

using namespace std;

struct Person {
    string name;
    int age, income;
    bool is_male;
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = gender == 'M';
    }

    return result;
}

template <typename Iter>
optional<string> MostPopularName(Iter begin, Iter end) {
    if (begin == end) 
        return std::nullopt;

    sort(begin, end, [](const Person& lhs, const Person& rhs) {
        return lhs.name < rhs.name;
    });
    const string* most_popular_name = &begin->name;
    int count = 1;
    for (auto i = begin; i != end; ) {
        auto same_name_end = find_if_not(i, end, [i](const Person& p) {
            return p.name == i->name;
        });
        const auto cur_name_count = distance(i, same_name_end);
        if (
            cur_name_count > count ||
            (cur_name_count == count && i->name < *most_popular_name)
        ) {
            count = cur_name_count;
            most_popular_name = &i->name;
        }
        i = same_name_end;
    }
    return *most_popular_name;
}

struct stats {
    optional<string> most_popular_m_name;
    optional<string> most_popular_f_name;
    vector<int> cumulative_wealth;
    vector<Person> sorted_by_age;
};

stats BuildStatsData(vector<Person> people) {
    stats stats;

    { // самые популярные имена
        auto it = partition(people.begin(), people.end(), [](const Person& pers){
            return pers.is_male;
        });
        stats.most_popular_m_name = MostPopularName(people.begin(), it);
        stats.most_popular_f_name = MostPopularName(it, people.end());
    }


    { // определение совокупного дохода
        sort(people.begin(), people.end(), [](const Person& lhs, const Person& rhs){
            return lhs.income > rhs.income;
        });
        stats.cumulative_wealth.resize(people.size());
        if(!people.empty()){
            stats.cumulative_wealth[0] = people[0].income;
            for (size_t i = 1, n = people.size(); i < n; ++i){
                stats.cumulative_wealth[i] = stats.cumulative_wealth[i - 1] + people[i].income;
            }
        }
    }
    
    { // сортировка по возрасту
        sort(people.begin(), people.end(), [](const Person& lhs, const Person& rhs){
            return lhs.age < rhs.age;
        });
        stats.sorted_by_age = move(people);
    }

    return stats;
}


int main() {
    stats stats = BuildStatsData(ReadPeople(cin));

    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;
            auto& sorted = stats.sorted_by_age;
            auto adult_begin = lower_bound(sorted.begin(), sorted.end(), adult_age, [](const Person& pers, int age){
                return pers.age < age;
            });
            cout << "There are " << distance(adult_begin, sorted.end())
                     << " adult people for maturity age " << adult_age << '\n';
        }
        else if (command == "WEALTHY") {
            int count;
            cin >> count;

            cout << "Top-" << count << " people have total income "
                << stats.cumulative_wealth[count - 1] << '\n';
        }
        else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;

            optional<string>& most_popular_name = 
                gender == 'M' ? stats.most_popular_m_name : stats.most_popular_f_name;

            if (most_popular_name){
                cout << "Most popular name among people of gender " << gender << " is "
                         << *most_popular_name << '\n';
            }
            else{
                cout << "No people of gender " << gender << '\n';
            }
        }
    }

}
