#ifndef	__ARRAY_HPP__
#define	__ARRAY_HPP__

#include <vector>
#include <memory>
#include "idata.hpp"
#include "composite.hpp"

namespace data {
	class Array: public Composite, public std::vector<std::shared_ptr<IData>> {
	public:
		Array() = default;
		Array(const Array& other) = default;
		Array& operator=(const Array& other) = default;

		Array(const IData& other);
		Array& operator=(const IData& other);

		virtual inline Composite::CompositeType Array::getCompositeType() const override;
	private:
		typedef std::vector<std::shared_ptr<IData>> UnderlyingClass;
	}; // Array

	inline Composite::CompositeType Array::getCompositeType() const {
		return CompositeType::ARRAY;
	}
} // namespace data

#endif // __OBJECT_HPP__