#ifndef	__STRING_HPP__
#define	__STRING_HPP__

#include <stddef.h>

namespace data {
	class String {
	public:
		String(const char * const str = "");
		String(const char& chr);
		String(const String& other);
		virtual ~String() noexcept;
		String& operator=(const String& other);
		String operator+(const String& other) const;
		inline String& operator+=(const String& other);
		inline String& operator+=(const char& chr);
		inline const char *c_str() const;
		inline size_t size() const;
		inline bool operator==(const String& other) const;
	private:
		char *m_string;
		static char *strDup(const char * const src);
		static char *strCpy(char * const dst, const char * const src);
		static size_t strLen(const char * const str);
		static int strCmp(const char * const one, const char * const other);
		static char *strCat(const char * const one, const char * const other);
	}; // String

	inline String& String::operator+=(const String& other) {
		*this = *this + other;
		return *this;
	}

	inline String& String::operator+=(const char& chr) {
		*this = *this + String(chr);
		return *this;
	}

	inline const char *String::c_str() const {
		return m_string;
	}

	inline size_t String::size() const {
		return strLen(m_string);
	}

	inline bool String::operator==(const String& other) const {
		return (0 == strCmp(m_string, other.m_string));
	}

} // namespace data

#endif // __STRING_HPP__