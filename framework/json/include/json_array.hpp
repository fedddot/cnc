#ifndef	__JSON_ARRAY_HPP__
#define	__JSON_ARRAY_HPP__

#include "ijson_value.hpp"
#include "list.hpp"
#include "string.hpp"
#include "shared_ptr.hpp"

namespace json {
	class JsonArray: public IJsonValue, public data::List<data::SharedPtr<IJsonValue>> {
	public:
		virtual JsonValueType getType() const override;
		virtual data::String getJsonString() const override;
		virtual data::List<char>::Iter parse(const data::List<char>::Iter& start) override;
	}; // class JsonArray
} // namespace json

#endif // __JSON_ARRAY_HPP__