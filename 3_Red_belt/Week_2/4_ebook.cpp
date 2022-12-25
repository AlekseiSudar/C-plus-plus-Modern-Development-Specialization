#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
    ReadingManager() :
    users_(MAX_USER_ + 1, 0),
    pages_(MAX_PAGE_ + 1, 0) {}

    void Read(int user_id, int page) {
        pages_[users_[user_id]]--;
        pages_[page]++;
        users_[user_id] = page;
    }

    double Cheer(int user_id) const {
        if (users_[user_id] == 0)
            return 0;
        if (pages_[0] == -1)
            return 1;

        int sum = 0;
        for(auto it = pages_.begin() + 1; it != pages_.begin() + users_[user_id]; ++it)
            sum += *it;

        return sum / (-pages_[0] - 1.0);
    }

private:
    static const int MAX_USER_ = 100'000;
    static const int MAX_PAGE_ = 1'000;

    vector<int> users_;
    vector<int> pages_;

};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}