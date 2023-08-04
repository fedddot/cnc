#ifndef	__ARRAY_HPP__
#define	__ARRAY_HPP__

#include <vector>
#include <memory>
#include "idata.hpp"
#include "composite.hpp"

namespace data {
	class Array: public Composite, public std::vector<std::shared_ptr<IData>> {
	public:
		typedef std::vector<std::shared_ptr<IData>> ContainerType;

		Array() = default;
		Array(const Array& other);
		Array& operator=(const Array& other);

		Array(const IData& other);
		Array& operator=(const IData& other);

		Array(const ContainerType& other);
		Array& operator=(const ContainerType& other);

		virtual inline CompositeType getCompositeType() const override;
	}; // Array

	inline Array::CompositeType Array::getCompositeType() const {
		return CompositeType::ARRAY;
	}
} // namespace data

#endif // __OBJECT_HPP__