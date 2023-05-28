#ifndef	__IJSON_VALUE_HPP__
#define	__IJSON_VALUE_HPP__

#include <string>
#include "iparsable.hpp"

namespace json {
	class IJsonValue: public common::IParsable {
	public:
		enum class JsonValueType : int {
			STRING,
			ARRAY,
			OBJECT
		};
		virtual JsonValueType getType() const = 0;
		virtual std::string getJsonString() const = 0;
	}; // IJsonValue
} // namespace json

#endif // __IJSON_VALUE_HPP__