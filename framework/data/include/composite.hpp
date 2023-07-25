#ifndef	__COMPOSITE_HPP__
#define	__COMPOSITE_HPP__

#include "idata.hpp"

namespace data {
	class Composite: public IData {
	public:
		class IAccessor {
		public:
			enum class AccessorType: int {
				NAME,
				INDEX
			};
			virtual AccessorType getType() const = 0;
			virtual ~IAccessor() noexcept = 0;
		};
		
		virtual inline DataType getDataType() const override;
		virtual IData& access(const IAccessor& where) = 0;
		virtual const IData& access(const IAccessor& where) const = 0;
	}; // Composite

	inline Composite::DataType Composite::getDataType() const {
		return DataType::COMPOSITE;
	}
} // namespace data

#endif // __COMPOSITE_HPP__