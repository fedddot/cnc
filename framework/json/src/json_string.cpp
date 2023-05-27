#include "json_string.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

#include "string.hpp"

using namespace json;
using namespace data;

JsonString::JsonString(const String& value) : String(value) {

}

JsonString::JsonValueType JsonString::getType() const {
	return JsonValueType::STRING;
}

String JsonString::getJsonString() const {
	return String((char)(JsonSpecialChar::STRING_START)) + *this + String((char)(JsonSpecialChar::STRING_END));
}

List<char>::Iter JsonString::parse(const List<char>::Iter& start) {
	List<char> skip_chars;
	skip_chars.push_front('\t');
	skip_chars.push_front('\n');
	skip_chars.push_front((char)(JsonSpecialChar::SPACE));
	
	List<char>::Iter iter = skipChars(start, skip_chars);
	if (iter.isEndIter()) {
		return iter;
	}

	if ((char)(JsonSpecialChar::STRING_START) != iter.get()) {
		return iter;
	}
	++iter;

	String value("");
	while (!iter.isEndIter()) {
		if ((char)(JsonSpecialChar::STRING_END) == iter.get()) {
			*this = value;
			++iter;
			return iter;
		}
		value += iter.get();
		++iter;
	}
	return iter;
}