#include <vector>
#include "json_utils.hpp"

using namespace json;

std::vector<char>::iterator json::skipChars(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end, const std::vector<char>& chars_to_skip) {
	std::vector<char>::iterator iter(begin);
	while (end != iter) {
		if (!isCharInList(*iter, chars_to_skip)) {
			return iter;
		}
		++iter;
	}
	return iter;
}

bool json::isCharInList(char chr, const std::vector<char>& list_of_chars) {
	auto iter = list_of_chars.begin();
	while (list_of_chars.end() != iter) {
		if (chr == *iter) {
			return true;
		}
		++iter;
	}
	return false;
}