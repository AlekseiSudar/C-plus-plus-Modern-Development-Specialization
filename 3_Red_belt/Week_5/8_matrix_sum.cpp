#include "test_runner.h"
#include <vector>
#include <numeric>
#include <future>
using namespace std;

template <typename It >
class IteratorRange {
public:
    IteratorRange (It f, It l) : first(f), last(l) {}

    It begin() const { return first; }
    It end() const { return last; }
    size_t size() const { return distance(first, last); }
private:
    It first , last;
};

template <typename Container >
auto Head(Container v, size_t top) {
    return IteratorRange {
        v.begin(), next(v.begin(), min(top, v.size()))
    };
}

template <typename It>
class Paginator {
        public:
            Paginator(It begin, It end, size_t page_size) {
                while (begin != end) {
                    auto buf = IteratorRange(begin, end);
                    pages.push_back(Head(buf, page_size));
                    begin = next(begin, min(page_size, buf.size()));
                }
            }

            auto begin() const { return pages.begin(); }
            auto end() const { return pages.end(); }
            size_t size() const { return pages.size(); }
        private:
            vector<IteratorRange<It>> pages;
        };

template <typename C>
auto Paginate(const C& c, size_t page_size) {
    auto a = Paginator(c.begin(), c.end(), page_size);
    return Paginator(c.begin(), c.end(), page_size);
}

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix) {
    int64_t sum = 0;
    for (const auto& row : matrix)
        sum += accumulate(row.begin(), row.end(), 0);
    return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    vector<future<int64_t>> future;
    for (auto page : Paginate(matrix, 2250)){
        future.push_back(async(
                [page](){ return SumSingleThread(page); }
                ));
    }
    int64_t ans = 0;
    for (auto& i : future){
        ans += i.get();
    }
    return ans;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);

}
