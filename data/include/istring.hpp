#ifndef	__ISTRING_HPP__
#define	__ISTRING_HPP__

#include <stddef.h>

namespace data {
	class IString {
	public:
		virtual ~IString() noexcept = 0;
		virtual size_t size() const = 0;
		virtual const char *c_str() const = 0;
	}; // IString
} // namespace data

#endif // __ISTRING_HPP__