#ifndef	__JSON_STRING_HPP__
#define	__JSON_STRING_HPP__

#include <stddef.h>

#include "ijson_value.hpp"
#include <vector>
#include <string>
#include <memory>

namespace json {
	class JsonString: public IJsonValue, public std::string {
	public:
		virtual JsonValueType getType() const override;
		virtual std::string getJsonString() const override;
		virtual std::vector<char>::iterator parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) override;
		inline JsonString& operator=(const std::string& other);
	};

	inline JsonString& JsonString::operator=(const std::string& other) {
		static_cast<std::string&>(*this) = other;
		return *this;
	}
}

#endif // __JSON_STRING_HPP__