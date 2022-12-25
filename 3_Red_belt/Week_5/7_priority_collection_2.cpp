#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <list>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = int;
    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object){
        Id id = collection_.size();
        auto it = priority_.insert({0, id}).first;
        collection_.push_back({new T(move(object)), it});
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin){
        while(range_begin != range_end){
            *ids_begin = Add(move(*range_begin));
            ids_begin++;
            range_begin++;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const{
        return collection_[id].first != nullptr;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const{
        return *(collection_[id].first);
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id){
        auto old_priority = collection_[id].second;
        auto it = priority_.insert({old_priority->first + 1, old_priority->second}).first;
        collection_[id].second = it;
        priority_.erase(old_priority);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const{
        auto max_elem = priority_.rbegin();
        return {*(collection_[max_elem->second].first), max_elem->first};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax(){
        auto max_elem = --priority_.end();

        int priority = max_elem->first;

        T ans_elem = move(*(collection_[max_elem->second].first));
        collection_[max_elem->second].first = nullptr;

        priority_.erase(max_elem);

        return make_pair(move(ans_elem), priority);
    }

private:
    set<pair<int, Id>> priority_;
    vector<pair<T*,typename set<pair<int, Id>>::iterator>> collection_;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }

}

void TestGoodForMe() {
    const int COUNT = 3;
    PriorityCollection<StringNonCopyable> strings;

    vector<PriorityCollection<StringNonCopyable>::Id> ids;
    for (int i = 0; i < COUNT; ++i) {
        ids.push_back(strings.Add(StringNonCopyable(1, 'a' + i)));
    }
    strings.Promote(ids[0]);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestGoodForMe);
    return 0;
}
