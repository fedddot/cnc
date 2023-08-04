#ifndef	__COMPOSITE_HPP__
#define	__COMPOSITE_HPP__

#include "idata.hpp"

namespace data {
	class Composite: public IData {
	public:		
		enum class CompositeType: int {
			OBJECT,
			ARRAY
		};
		
		virtual inline DataType getDataType() const override;
		virtual CompositeType getCompositeType() const = 0;
	}; // Composite

	inline Composite::DataType Composite::getDataType() const {
		return DataType::COMPOSITE;
	}
} // namespace data

#endif // __COMPOSITE_HPP__