#include "parser.h"
#include <optional>
#include <string>

using namespace std;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter) {
    const size_t pos = s.find(delimiter);
    if (pos == s.npos)
        return {s, nullopt};
    else
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter) {
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter) {
    const auto [lhs, rhs] = SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}

size_t ReadNumberOnLine(istream& in_stream) {
    size_t number;
    in_stream >> number;
    string dummy;
    getline(in_stream, dummy);
    return number;
}