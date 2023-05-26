#include "list.hpp"
#include "json_utils.hpp"

using namespace json;
using namespace data;

List<char>::Iter skipChars(const List<char>::Iter& from, const List<char>& chars_list) {
	List<char>::Iter iter(from);
	while (!iter.isEndIter()) {
		if (!isCharInList(iter.get(), chars_list)) {
			return iter;
		}
		++iter;
	}
	return iter;
}

bool isCharInList(const char& chr, const List<char>& list) {
	List<char>::Iter iter(const_cast<List<char>&>(list).begin());
	while (!iter.isEndIter()) {
		if (chr == iter.get()) {
			return true;
		}
		++iter;
	}
	return false;
}