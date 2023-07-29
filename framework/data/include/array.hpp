#ifndef	__ARRAY_HPP__
#define	__ARRAY_HPP__

#include <cstddef>
#include <vector>
#include <memory>
#include "idata.hpp"
#include "composite.hpp"

namespace data {
	class Array: public Composite, public std::vector<std::shared_ptr<IData>> {
	public:
		class Accessor: public IAccessor {
		public:
			Accessor(const std::size_t& index);
			virtual inline AccessorType getType() const override;
			inline std::size_t getIndex() const;
		private:
			const std::size_t m_index;
		};

		Array() = default;
		Array& operator=(const Array& other) = default;

		Array(const IData& other);
		Array& operator=(const IData& other);

		virtual IData& access(const IAccessor& where) override;
		virtual const IData& access(const IAccessor& where) const override;
	private:
		typedef std::vector<std::shared_ptr<IData>> UnderlyingClass;

		IData& accessAt(const IAccessor& where);
	}; // Composite

	inline Array::Accessor::AccessorType Array::Accessor::getType() const {
		return AccessorType::INDEX;
	}

	inline std::size_t Array::Accessor::getIndex() const {
		return m_index;
	}
} // namespace data

#endif // __OBJECT_HPP__