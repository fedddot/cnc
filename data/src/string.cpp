#include <string.h>
#include "string.hpp"
#include "exception.hpp"
#include "exception_handler.hpp"

using namespace data;

static const String invalidPointerMsg("String class: invalid pointer received");
static const String failedCopyMsg("String class: failed to copy string");

String::String(const char * const str) : m_string(copyString(str)) {

}

String::String(const char& chr) : m_string(copyString(" ")) {
	m_string[0] = chr;
}

String::String(const String& other) : m_string(copyString(other.m_string)) {

}

String& String::operator=(const String& other) {
	char *string_buff = copyString(other.m_string);
	delete[] m_string;
	m_string = string_buff;
	string_buff = nullptr;
	return *this;
}

String::~String() noexcept {
	delete[] m_string;
}

String& String::operator+=(const String& other) {
	const size_t left_size = size();
	const size_t right_size = other.size();
	char *buff = new char[left_size + right_size + 1UL];
	if ((nullptr == strcpy(buff, c_str())) || (nullptr == strcpy(buff + left_size, other.c_str()))) {
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(failedCopyMsg));
	}
	*(buff + left_size + right_size) = '\0';
	delete m_string;
	m_string = buff;
	return *this;
}

String& String::operator+=(const char& chr) {
	const size_t left_size = size();
	const size_t right_size = 1UL;
	char *buff = new char[left_size + right_size + 1UL];
	if (nullptr == strcpy(buff, c_str())) {
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(failedCopyMsg));
	}
	*(buff + left_size) = chr;
	*(buff + left_size + right_size) = '\0';
	delete m_string;
	m_string = buff;
	return *this;
}

const char *String::c_str() const {
	return m_string;
}

size_t String::size() const {
	return strlen(m_string);
}

char *String::copyString(const char * const str) {
	if (nullptr == str) {
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(invalidPointerMsg));
	}
	char *buff = new char[strlen(str) + 1UL];
	if (nullptr == strcpy(buff, str)) {
		delete[] buff;
		buff = nullptr;
		except::ExceptionHandler::getInstance()->onEvent(except::Exception(failedCopyMsg));
	}
	return buff;
}

String data::operator+(const String& left, const String& right) {
	String result(left);
	result += right;
	return result;
}

bool data::operator==(const String& left, const String& right) {
	return (0 == strcmp(left.c_str(), right.c_str()));
}
