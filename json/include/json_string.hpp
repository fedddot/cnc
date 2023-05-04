#ifndef	__JSON_STRING_HPP__
#define	__JSON_STRING_HPP__

#include <string>
#include <cstddef>

#include "ijson_value.hpp"

namespace json {
	class JsonString: public IJsonValue {
	public:
		JsonString(const std::string& value = "");
		virtual JsonValueType getType() const override;
		virtual std::string getJsonString() const override;
		virtual const char *parse(const char * const from) override;
		inline std::string get() const;
	private:
		std::string m_value;
	};

	inline std::string JsonString::get() const {
		return m_value;
	}
}

#endif // __JSON_STRING_HPP__