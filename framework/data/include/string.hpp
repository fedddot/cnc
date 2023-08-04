#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <string>
#include "value.hpp"

namespace data {
	class String: public Value, public std::string {
	public:
		typedef std::string ValueType;

		String() = default;
		String(const String& other);
		String& operator=(const String& other);

		String(const IData& other);
		String& operator=(const IData& other);

		String(const ValueType& other);
		String& operator=(const ValueType& other);

		virtual inline DataValueType getDataValueType() const override;
	private:
		typedef std::string UnderlyingClass;
	}; // String

	inline String::DataValueType String::getDataValueType() const {
		return DataValueType::STRING;
	}
} // namespace data

#endif // __STRING_HPP__