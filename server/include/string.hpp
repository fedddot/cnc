#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <stddef.h>
#include "istring.hpp"

namespace server {
	class String: public data::IString {
	public:
		String(const char * const cstr = "");
		String(const String& other);
		String& operator=(const String& other);
		virtual ~String() noexcept override;
		virtual size_t size() const override;
		virtual const char *c_str() const override;
	private:
		char *m_string;
		size_t m_string_size;
	}; // String
} // namespace server

#endif // __STRING_HPP__