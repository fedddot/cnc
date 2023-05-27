#include "string.hpp"
#include "exception.hpp"
#include "exception_handler.hpp"

using namespace data;

static const String invalidPointerMsg("inv_ptr");
static const String failedCopyMsg("cpy_fld");

String::String(const char * const str) : m_string(strDup(str)) {

}

String::String(const char& chr) : m_string(strDup(" ")) {
	m_string[0] = chr;
}

String::String(const String& other) : m_string(strDup(other.m_string)) {

}

String::~String() noexcept {
	delete[] m_string;
}

String& String::operator=(const String& other) {
	char *string_buff = strDup(other.m_string);
	delete[] m_string;
	m_string = string_buff;
	string_buff = nullptr;
	return *this;
}

String String::operator+(const String& other) const {
	char *buff = strCat(m_string, other.m_string);
	String result(buff);
	delete[] buff;
	buff = nullptr;
	return result;
}

char *String::strDup(const char * const str) {
	if (nullptr == str) {
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(invalidPointerMsg));
	}
	size_t str_size = strLen(str);
	char *buff = new char[str_size + 1UL];
	strCpy(buff, str);
	*(buff + str_size) = '\0';
	return buff;
}

char *String::strCpy(char * const dst, const char * const src) {
	if ((nullptr == dst) || (nullptr == src)){
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(invalidPointerMsg));
	}
	const char *src_char_iter = src;
	char *dst_char_iter = dst;
	while ('\0' != *src_char_iter) {
		*dst_char_iter = *src_char_iter;
		++src_char_iter;
		++dst_char_iter;
	}
	return dst;
}

size_t String::strLen(const char * const str) {
	if (nullptr == str) {
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(invalidPointerMsg));
	}
	const char *str_char_iter = str;
	while ('\0' != *str_char_iter) {
		++str_char_iter;
	}
	return str_char_iter - str;
}

int String::strCmp(const char * const one, const char * const other) {
	if ((nullptr == one) || (nullptr == other)){
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(invalidPointerMsg));
	}
	const char *src_char_iter = one;
	const char *dst_char_iter = other;
	int result = 0;
	while (0 == (result = static_cast<int>(*src_char_iter) - static_cast<int>(*dst_char_iter))) {
		if (('\0' == *src_char_iter) || ('\0' == *dst_char_iter)) {
			break;
		}		
		++src_char_iter;
		++dst_char_iter;
	}
	return result;
}

char *String::strCat(const char * const one, const char * const other) {
	size_t one_len = strLen(one);
	size_t other_len = strLen(other);
	char *result = new char[one_len + other_len + 1];
	strCpy(result, one);
	strCpy(result + one_len, other);
	*(result + one_len + other_len) = '\0';
	return result;
}