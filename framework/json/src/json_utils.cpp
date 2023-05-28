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

std::vector<char> json::stringToVector(const std::string& str) {
	std::vector<char> output_vector;
	auto iter = str.begin();
	while (str.end() != iter) {
		output_vector.push_back(*iter);
		++iter;
	}
	return output_vector;
}
std::string json::vectorToString(const std::vector<char>& vctr) {
	std::string output_string;
	auto iter = vctr.begin();
	while (vctr.end() != iter) {
		output_string += *iter;
		++iter;
	}
	return output_string;
}