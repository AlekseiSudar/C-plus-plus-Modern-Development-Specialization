#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
public:
    using Timestamp = int;
    using Karma = int;
    using User = string;
    using Id = string;

    bool Put(const Record& record){
        if (!by_id_.count(record.id)){
            Data data;
            data.record = record;
            by_id_[record.id] = move(data);

            const Record* r = &by_id_[record.id].record;
            auto it_t = by_timestamp_.insert({ record.timestamp, r });
            by_id_[record.id].it_timestamp = it_t;
            auto it_k = by_karma_.insert({ record.karma, r });
            by_id_[record.id].it_karma = it_k;
            auto it_u = by_user_.insert({ record.user, r });
            by_id_[record.id].it_user = it_u;
            return true;
        }
        return false;
    }
    const Record* GetById(const Id& id) const{
        if (by_id_.count(id))
            return &by_id_.at(id).record;
        return nullptr;
    }
    bool Erase(const Id& id){
        if (by_id_.count(id)){
            by_timestamp_.erase(by_id_[id].it_timestamp);
            by_karma_.erase(by_id_[id].it_karma);
            by_user_.erase(by_id_[id].it_user);
            by_id_.erase(id);

            return true;
        }
        return false;
    }

    template <typename Callback>
    void RangeByTimestamp(Timestamp low, Timestamp high, Callback callback) const{
        for (auto it1 = by_timestamp_.lower_bound(low),
                it2 = by_timestamp_.upper_bound(high);
                it1 != it2; it1++){
            if (!callback(*it1->second)) return;
        }
    }

    template <typename Callback>
    void RangeByKarma(Karma low, Karma high, Callback callback) const{
        for (auto it1 = by_karma_.lower_bound(low),
                it2 = by_karma_.upper_bound(high);
                it1 != it2; it1++){
            if (!callback(*it1->second)) return;
        }
    }

    template <typename Callback>
    void AllByUser(const User& user, Callback callback) const{
        for (auto [it1, it2] = by_user_.equal_range(user); it1 != it2; it1++){
            if (!callback(*it1->second)) return;
        }
    }

private:
    struct Data
    {
        Record record;
        multimap<Timestamp, const Record*>::iterator it_timestamp;
            multimap<Karma, const Record*>::iterator it_karma;
             multimap<User, const Record*>::iterator it_user;
    };
    
    unordered_map<Id, Data> by_id_; 

    multimap<Timestamp, const Record*> by_timestamp_;
    multimap<Karma, const Record*> by_karma_;
    multimap<User, const Record*> by_user_;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
