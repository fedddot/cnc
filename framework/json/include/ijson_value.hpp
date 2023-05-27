#ifndef	__IJSON_VALUE_HPP__
#define	__IJSON_VALUE_HPP__

#include "iparsable.hpp"
#include "string.hpp"

namespace json {
	class IJsonValue: public common::IParsable {
	public:
		enum class JsonValueType : int {
			STRING,
			ARRAY,
			OBJECT
		};
		virtual JsonValueType getType() const = 0;
		virtual data::String getJsonString() const = 0;
	}; // IJsonValue
} // namespace json

#endif // __IJSON_VALUE_HPP__