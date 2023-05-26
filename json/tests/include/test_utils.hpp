#ifndef	__TEST_UTILS_HPP__
#define	__TEST_UTILS_HPP__

#include "string.hpp"
#include "list.hpp"

namespace testing {
	typename data::List<char> strToList(const data::String& str);
} // namespace testing

#endif // __TEST_UTILS_HPP__