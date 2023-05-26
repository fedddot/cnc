#include "json_object.hpp"
#include "json_string.hpp"
#include "json_array.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"
#include "string.hpp"

using namespace json;
using namespace data;

JsonArray::JsonValueType JsonArray::getType() const {
	return JsonValueType::ARRAY;
}

String JsonArray::getJsonString() const {
	String output_string("");
	output_string += (char)(JsonSpecialChar::ARRAY_START);
	Iter iter = const_cast<JsonArray *>(this)->begin();
	while (!iter.isEndIter()) {
		output_string += iter.get().get()->getJsonString();
		++iter;
		if (!iter.isEndIter()) {
			output_string += (char)(JsonSpecialChar::DELIMITER);
			output_string += (char)(JsonSpecialChar::SPACE);
		}
	}
	output_string += (char)(JsonSpecialChar::ARRAY_END);
	return output_string;
}

List<char>::Iter JsonArray::parse(const data::List<char>::Iter& start) {
	List<char> skip_chars;
	skip_chars.push_front('\t');
	skip_chars.push_front('\n');
	skip_chars.push_front((char)(JsonSpecialChar::SPACE));
	
	List<char>::Iter iter = skipChars(start, skip_chars);
	if (iter.isEndIter()) {
		return iter;
	}

	if ((char)(JsonSpecialChar::ARRAY_START) != iter.get()) {
		return iter;
	}
	++iter;

	JsonArray value;
	while (!iter.isEndIter()) {
		iter = skipChars(start, skip_chars);
		if (iter.isEndIter()) {
			return iter;
		}

		if ((char)(JsonSpecialChar::ARRAY_END) == iter.get()) {
			*this = value;
			++iter;
			return iter;
		}
		
		SharedPtr<IJsonValue> new_value(nullptr);
		switch (iter.get()) {
		case ((char)(JsonSpecialChar::STRING_START)):
			new_value = SharedPtr<IJsonValue>(new JsonString());
			break;
		case ((char)(JsonSpecialChar::OBJECT_START)):
			new_value = SharedPtr<IJsonValue>(new JsonObject());
			break;
		case ((char)(JsonSpecialChar::ARRAY_START)):
			new_value = SharedPtr<IJsonValue>(new JsonArray());
			break;		
		default:
			return iter;
		}

		iter = new_value.get()->parse(iter);
		push_back(new_value);

		iter = skipChars(start, skip_chars);
		if (iter.isEndIter()) {
			return iter;
		}

		switch (iter.get()) {
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::ARRAY_END)):
			;
			break;
		default:
			return iter;
		}
	}
	return iter;
}