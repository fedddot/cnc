#ifndef	__JSON_UTILS_HPP__
#define	__JSON_UTILS_HPP__

#include "list.hpp"

namespace json {
	data::List<char>::Iter skipChars(const data::List<char>::Iter& from, const data::List<char>& chars_list);
	bool isCharInList(const char& chr, const data::List<char>& list);
}

#endif // __JSON_UTILS_HPP__