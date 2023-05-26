#include "json_object.hpp"
#include "json_string.hpp"
#include "json_array.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

using namespace json;
using namespace data;

JsonObject::JsonValueType JsonObject::getType() const {
	return JsonValueType::OBJECT;
}

String JsonObject::getJsonString() const {
	String output_string("");
	output_string += (char)(JsonSpecialChar::OBJECT_START);
	Iter iter = const_cast<JsonObject *>(this)->begin();
	Iter iter_end = const_cast<JsonObject *>(this)->end();
	while (!iter.isEndIter()) {
		output_string += (char)(JsonSpecialChar::STRING_START); 
		output_string += iter.get().first();
		output_string += (char)(JsonSpecialChar::STRING_END);
		output_string += (char)(JsonSpecialChar::SEMICOLON);
		output_string += (char)(JsonSpecialChar::SPACE);
		output_string += iter.get().second().get()->getJsonString();
		++iter;
		if (!iter.isEndIter()) {
			output_string += (char)(JsonSpecialChar::DELIMITER);
			output_string += (char)(JsonSpecialChar::SPACE);
		}
	}
	output_string += (char)(JsonSpecialChar::OBJECT_END);
	return output_string;
}

List<char>::Iter JsonObject::parse(const data::List<char>::Iter& start) {
	List<char> skip_chars;
	skip_chars.push_front('\t');
	skip_chars.push_front('\n');
	skip_chars.push_front((char)(JsonSpecialChar::SPACE));
	
	List<char>::Iter iter = skipChars(start, skip_chars);
	if (iter.isEndIter()) {
		return iter;
	}

	if ((char)(JsonSpecialChar::OBJECT_START) != iter.get()) {
		return iter;
	}
	++iter;

	JsonObject value;
	while (!iter.isEndIter()) {
		iter = skipChars(iter, skip_chars);
		if (iter.isEndIter()) {
			return iter;
		}

		if ((char)(JsonSpecialChar::OBJECT_END) == iter.get()) {
			*this = value;
			++iter;
			return iter;
		}

		JsonString field_name("");
		iter = field_name.parse(iter);
		if (0 == field_name.size()) {
			return iter;
		}

		iter = skipChars(iter, skip_chars);
		if (iter.isEndIter()) {
			return iter;
		}

		if ((char)(JsonSpecialChar::SEMICOLON) != iter.get()) {
			return iter;
		}
		++iter;

		iter = skipChars(iter, skip_chars);
		if (iter.isEndIter()) {
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
		value.push_back(Pair<String, SharedPtr<IJsonValue>>(field_name, new_value));
		
		iter = skipChars(iter, skip_chars);
		if (iter.isEndIter()) {
			return iter;
		}

		switch (iter.get())
		{
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::OBJECT_END)):
			;
			break;
		default:
			return iter;
		}
	}
	return iter;
}
