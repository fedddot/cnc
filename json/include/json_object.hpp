#ifndef	__JSON_OBJECT_HPP__
#define	__JSON_OBJECT_HPP__

#include <string>
#include <cstddef>
#include <map>
#include <memory>

#include "ijson_value.hpp"

namespace json {
	class JsonObject: public IJsonValue {
	public:
		virtual JsonValueType getType() const override;
		virtual std::string getJsonString() const override;
		virtual const char *parse(const char * const from) override;
		void addObject(const std::string& key, const std::shared_ptr<IJsonValue>& value);
		
		inline std::shared_ptr<IJsonValue> get(const std::string name) const;
	private:
		std::map<std::string, std::shared_ptr<IJsonValue>> m_values;
	}; // JsonObject

	inline std::shared_ptr<IJsonValue> JsonObject::get(const std::string name) const {
		auto it = m_values.find(name);
		if (m_values.end() != it) {
			return it->second;
		}		
		return std::shared_ptr<IJsonValue>(nullptr);
	}
}

#endif // __JSON_OBJECT_HPP__