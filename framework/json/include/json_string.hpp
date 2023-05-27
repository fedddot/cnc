#ifndef	__JSON_STRING_HPP__
#define	__JSON_STRING_HPP__

#include <stddef.h>

#include "ijson_value.hpp"
#include "list.hpp"
#include "string.hpp"

namespace json {
	class JsonString: public IJsonValue, public data::String {
	public:
		JsonString(const data::String& value = "");
		virtual JsonValueType getType() const override;
		virtual data::String getJsonString() const override;
		virtual data::List<char>::Iter parse(const data::List<char>::Iter& start) override;
	};
}

#endif // __JSON_STRING_HPP__