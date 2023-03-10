#include "test_runner.h"

#include <map>
#include <string>
#include <string_view>
#include <iostream>
#include <future>
#include <vector>
using namespace std;


    #include <vector>
using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other){
        for (const auto& stat : other.word_frequences)
            word_frequences[stat.first] += stat.second;
    }
};

vector<string_view> SplitIntoWordsView (string_view str)    {
    vector<string_view> result;
    while(true) {
        size_t space = str.find(' ');
        result. push_back (str.substr(0, space));
        if (space == str.npos)
            break;
        else
            str.remove_prefix(space + 1);
    }
    return result;
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    vector<string_view> words = SplitIntoWordsView(line);
    for (auto word : words)
        if (key_words.count(string(word)))
            result.word_frequences[string(word)]++;
        return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    const size_t threads = 32;

    vector<stringstream> streams(threads);
    string line;
    for (size_t i = 0; getline(input, line); i++){
        streams[i % threads] << line << '\n';
    }

    vector<future<Stats>> futures;
    for(auto& s : streams){
        futures.push_back(
                async(ExploreKeyWordsSingleThread, ref(key_words), ref(s))
                );
    }

    Stats result;
    for(auto& f : futures){
        result += f.get();
    }

    return result;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks", 2},
            {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}