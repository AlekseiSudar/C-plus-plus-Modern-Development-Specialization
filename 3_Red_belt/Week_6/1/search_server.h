#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <future>
#include <string_view>
using namespace std;

class InvertedIndex {
public:
    explicit InvertedIndex()  {
        docs.reserve(50000);
    }
    void Add(string document);
    vector<pair<size_t, size_t>> Lookup(string_view word) const;

    const string& GetDocument(size_t id) const {
        return docs[id];
    }

    size_t DocsSize() const {
        return docs.size();
    }

private:
    map<string_view, vector<pair<size_t, size_t>>> index;
    vector<string> docs;
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    InvertedIndex index;
};
