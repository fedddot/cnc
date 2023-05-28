#ifndef	__JSON_ARRAY_HPP__
#define	__JSON_ARRAY_HPP__

#include "ijson_value.hpp"
#include <vector>
#include <string>
#include <memory>

namespace json {
	class JsonArray: public IJsonValue, public std::vector<std::shared_ptr<IJsonValue>> {
	public:
		virtual JsonValueType getType() const override;
		virtual data::String getJsonString() const override;
		virtual std::vector<char>::iterator parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) override;
	}; // class JsonArray
} // namespace json

#endif // __JSON_ARRAY_HPP__