#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <stddef.h>

namespace data {
	class String {
	public:
		String(const char * const str = "");
		String(const char& chr);
		String(const String& other);
		String& operator=(const String& other);
		virtual ~String() noexcept;
		
		virtual const char *c_str() const;
		virtual size_t size() const;
	private:
		char *m_string;
		static char *copyString(const char * const str);
	}; // String

	String operator+(const String& left, const String& right);

} // namespace data

#endif // __STRING_HPP__