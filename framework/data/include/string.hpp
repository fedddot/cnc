#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <string>
#include "value.hpp"

namespace data {
	class String: public Value, public std::string {
	public:
		String() = default;
		String(const String& other) = default;
		String& operator=(const String& other) = default;

		String(const std::string& other);
		String& operator=(const std::string& other);

		String(const IData& other);
		String& operator=(const IData& other);

		virtual inline ValueType getValueType() const override;
	private:
		typedef std::string UnderlyingClass;
	}; // String

	inline String::ValueType String::getValueType() const {
		return ValueType::STRING;
	}
} // namespace data

#endif // __STRING_HPP__