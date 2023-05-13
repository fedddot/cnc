#ifndef	__IVECTOR_HPP__
#define	__IVECTOR_HPP__

#include <stddef.h>

#include "iiterable.hpp"

namespace data {
	template <class T>
	class IVector {
	public:
		virtual ~IVector() noexcept = 0;
		virtual size_t size() const = 0;
		virtual T& operator[](const size_t& index) = 0;
		virtual const T& operator[](const size_t& index) const = 0;
		virtual void push_back(const T& value) = 0;
	};
}

#endif // __VECTOR_HPP__