#pragma once

#include <string>
#include <vector>

std::vector<std::string> SplitString(std::string const& value, char seperator);
std::string RandomString(std::string const& candidate, size_t length);
std::string itos(size_t number);
