#include "list.hpp"
#include "test_utils.hpp"

using namespace testing;

typename data::List<char> testing::strToList(const data::String& str) {
	data::List<char> list;
	const char *str_iter = str.c_str();
	while ('\0' != *str_iter) {
		list.push_back(*str_iter);
		++str_iter;
	}
	return list;
}