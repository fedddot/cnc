#ifndef	__JSON_ARRAY_HPP__
#define	__JSON_ARRAY_HPP__

#include <string>
#include <cstddef>
#include <vector>
#include <memory>

#include "ijson_value.hpp"

namespace json {
	class JsonArray: public IJsonValue {
	public:
		virtual JsonValueType getType() const override;
		virtual std::string getJsonString() const override;
		virtual const char *parse(const char * const from) override;
		void addObject(const std::shared_ptr<IJsonValue>& value);
		
		inline std::shared_ptr<IJsonValue> get(const std::size_t index) const;
	private:
		std::vector<std::shared_ptr<IJsonValue>> m_values;
	};

	inline std::shared_ptr<IJsonValue> JsonArray::get(const std::size_t index) const {
		if (m_values.size() > index) {
			return m_values[index];
		}
		return nullptr;
	}

}

#endif // __JSON_ARRAY_HPP__