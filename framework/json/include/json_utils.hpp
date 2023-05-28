#ifndef	__JSON_UTILS_HPP__
#define	__JSON_UTILS_HPP__

#include <vector>

namespace json {
	std::vector<char>::iterator skipChars(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end, const std::vector<char>& chars_to_skip);
	bool isCharInList(char chr, const std::vector<char>& list_of_chars);
}

#endif // __JSON_UTILS_HPP__