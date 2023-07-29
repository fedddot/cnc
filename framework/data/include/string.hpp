#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <string>
#include "idata.hpp"

namespace data {
	class String: public IData, public std::string {
	public:
		String() = default;
		String& operator=(const String& other) = default;

		String(const IData& other);
		String& operator=(const IData& other);

		virtual inline DataType getDataType() const override;
	private:
		typedef std::string UnderlyingClass;
	}; // String

	inline String::DataType String::getDataType() const {
		return DataType::VALUE;
	}
} // namespace data

#endif // __STRING_HPP__