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

		virtual ~IJsonValue() noexcept = 0;
		virtual JsonValueType getType() const = 0;
		virtual std::string getJsonString() const = 0;
	};
}

#endif // __IJSON_VALUE_HPP__