#include "test_runner.h"
#include <string>
#include <set>
#include <map>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target){
        const auto it_s = data_.insert(string(source));
        const auto it_t = data_.insert(string(target));
        forward_dict[*it_s.first] = *it_t.first;
        backward_dict[*it_t.first] = *it_s.first;
    }
    string_view TranslateForward(string_view source) const{
        return Translate(forward_dict, source);
    }
    string_view TranslateBackward(string_view target) const{
        return Translate(backward_dict, target);
    }

private:
    static string_view Translate(const map<string_view,string_view>& dict, string_view s){
        auto it = dict.find(s);
        if (it != dict.end())
            return it->second;
        return {};
    }

    set<string> data_;
    map<string_view,string_view> forward_dict;
    map<string_view,string_view> backward_dict;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    return 0;
}
