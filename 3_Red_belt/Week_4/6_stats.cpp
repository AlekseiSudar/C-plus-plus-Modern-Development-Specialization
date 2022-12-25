#include "stats.h"

Stats::Stats() :
method_stats_({ {"GET", 0}, {"POST", 0}, {"PUT", 0}, {"DELETE", 0}, {"UNKNOWN", 0} }),
uri_stats_({ {"/", 0}, {"/order", 0}, {"/product", 0}, {"/basket", 0}, {"/help", 0}, {"unknown", 0} }) {}

void Stats::AddMethod(string_view method){
    if (method_stats_.count(method))
        method_stats_[method]++;
    else
        method_stats_["UNKNOWN"]++;
}
void Stats::AddUri(string_view uri){
    if (uri_stats_.count(uri))
        uri_stats_[uri]++;
    else
        uri_stats_["unknown"]++;
}
const map<string_view, int>& Stats::GetMethodStats() const{
    return method_stats_;
}
const map<string_view, int>& Stats::GetUriStats() const{
    return uri_stats_;
}

HttpRequest ParseRequest(string_view line){
    HttpRequest ans;

    size_t space = line.find_first_not_of(' ');
    line.remove_prefix(space);

    space = line.find(' ');
    ans.method = line.substr(0, space);
    line.remove_prefix(++space);

    space = line.find(' ');
    ans.uri = line.substr(0, space);
    line.remove_prefix(++space);

    space = line.find(' ');
    ans.protocol = line.substr(0, space);

    return ans;
}
