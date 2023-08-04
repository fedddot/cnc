#ifndef	__VALUE_HPP__
#define	__VALUE_HPP__

#include "idata.hpp"

namespace data {
	class Value: public IData {
	public:
		enum class DataValueType: int {
			STRING,
			// INT,
			// REAL
		};
		
		virtual inline DataType getDataType() const override;
		virtual DataValueType getDataValueType() const = 0;
	}; // Value

	inline Value::DataType Value::getDataType() const {
		return DataType::VALUE;
	}
} // namespace data

#endif // __VALUE_HPP__