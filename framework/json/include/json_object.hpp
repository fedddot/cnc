#ifndef	__JSON_OBJECT_HPP__
#define	__JSON_OBJECT_HPP__

#include "ijson_value.hpp"
#include "shared_ptr.hpp"
#include "string.hpp"
#include "pair.hpp"

namespace json {
	class JsonObject: public IJsonValue, public data::List<data::Pair<data::String, memory::SharedPtr<IJsonValue>>> {
	public:
		virtual JsonValueType getType() const override;
		virtual data::String getJsonString() const override;
		virtual data::List<char>::Iter parse(const data::List<char>::Iter& start) override;
	}; // class JsonObject
} // namespace json

#endif // __JSON_OBJECT_HPP__