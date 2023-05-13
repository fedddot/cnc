#include "json_utils.hpp"

using namespace json;

const char *json::skipChars(const char * const from, const char& chr) {
	const char *iter = from;
	while (chr == *iter) {
		++iter;
	}
	return iter;
}