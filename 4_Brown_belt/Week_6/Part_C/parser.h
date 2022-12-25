#pragma once
#include <string_view>
#include <iostream>
#include <vector>

std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");
size_t ReadNumberOnLine(std::istream& in_stream);