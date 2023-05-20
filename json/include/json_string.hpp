#ifndef	__JSON_STRING_HPP__
#define	__JSON_STRING_HPP__

#include <stddef.h>

#include "ijson_value.hpp"
#include "string.hpp"

namespace json {
	class JsonString: public IJsonValue {
	public:
		JsonString(const data::String& value = "");
		virtual JsonValueType getType() const override;
		virtual data::String getJsonString() const override;
		virtual const char *parse(const char * const from) override;
		inline data::String get() const;
	private:
		data::String m_value;
	};

	inline data::String JsonString::get() const {
		return m_value;
	}
}

#endif // __JSON_STRING_HPP__