#ifndef	__JSON_OBJECT_HPP__
#define	__JSON_OBJECT_HPP__

#include "ijson_value.hpp"
#include <map>
#include <vector>
#include <string>
#include <memory>

namespace json {
	class JsonObject: public IJsonValue, public std::map<std::string, std::shared_ptr<IJsonValue>> {
	public:
		virtual JsonValueType getType() const override;
		virtual std::string getJsonString() const override;
		virtual std::vector<char>::iterator parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) override;
	}; // class JsonObject
} // namespace json

#endif // __JSON_OBJECT_HPP__