#include <stdexcept>

#include "json_utils.hpp"

using namespace json;

const char *json::skipChars(const char * const from, const char& chr) {
	if (nullptr == from) {
		throw std::invalid_argument("nullptr received");
	}
	const char *iter = from;
	while (chr == *iter) {
		++iter;
	}
	return iter;
}