#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string_view> SplitIntoWords(string_view line) {
    vector<string_view> ans;

    size_t space = line.find_first_not_of(' ');

    while (space != line.npos){
        line.remove_prefix(space);

        space = line.find(' ');
        ans.push_back(line.substr(0, space));

        space = line.find_first_not_of(' ', space);
    }
    return ans;
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}
SearchServer::~SearchServer(){
    for_each(futures.begin(), futures.end(), [](auto& f)
    {
        f.get();
    });
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document); ) {
        new_index.Add(move(current_document));
    }

    lock_guard<mutex> l(m);
    index = move(new_index);
}

void SearchServer::AddQueriesStream(
        istream& query_input, ostream& search_results_output
        ) {
    auto f = [&query_input, &search_results_output, this](){
        vector<size_t> doc_count(index.DocsSize());
        for (string current_query; getline(query_input, current_query); ) {
            for (const auto word : SplitIntoWords(current_query)) {
                vector<pair<size_t, size_t>> docid_counts;
                {
                    lock_guard<mutex> l(m);
                    docid_counts = index.Lookup(word);
                }
                for (const auto docid_count : docid_counts) {
                   doc_count[docid_count.first] += docid_count.second;
                }
            }
            vector<pair<size_t, size_t>> search_results;
            search_results.reserve(50000);
            {
                size_t i = 0;
                for (auto& d: doc_count){
                    if (d > 0){
                        search_results.push_back({i,d});
                        d = 0;
                    }
                    i++;
                }
            }
            partial_sort(
                    search_results.begin(),
                    min(search_results.begin() + 5, search_results.end()),
                    search_results.end(),
                    [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                        int64_t lhs_docid = lhs.first;
                        auto lhs_hit_count = lhs.second;
                        int64_t rhs_docid = rhs.first;
                        auto rhs_hit_count = rhs.second;
                        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
                    }
                    );
            search_results_output << current_query << ':';
            for (auto [docid, hitcount] : Head(search_results, 5)) {
                search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << hitcount << '}';
            }
            search_results_output << endl;
        }
    };
    futures.push_back(async(f));
}

void InvertedIndex::Add(string document) {
    docs.push_back(move(document));

    const size_t docid = docs.size() - 1;
    for (const auto word : SplitIntoWords(docs.back())) {
        auto& doc_count = index[word];
        if (!doc_count.empty() && doc_count.back().first == docid){
            doc_count.back().second++;
        }
        else{
            doc_count.push_back({docid, 1});
        }
    }
}

vector<pair<size_t, size_t>> InvertedIndex::Lookup(string_view word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return {};
    }
}
