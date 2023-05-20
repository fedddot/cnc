#include <string.h>
#include <stdlib.h>
#include "string.hpp"

using namespace data;

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

const char *String::c_str() const {
	return (const char *)m_string;
}

size_t String::size() const {
	return strlen(m_string);
}

String data::operator+(const String& left, const String& right) {
	size_t left_size = left.size();
	size_t right_size = right.size();
	char *buff = new char[left_size + right_size + 1UL];
	if ((nullptr == strcpy(buff, left.c_str())) || (nullptr == strcpy(buff + left_size, right.c_str()))) {
		delete[] buff;
		return String("");
	}
	buff[left_size + right_size] = '\0';
	String result(buff);
	delete[] buff;
	return result;
}

char *String::copyString(const char * const str) {
	if (nullptr == str) {
		return nullptr;
	}
	char *buff = new char[strlen(str) + 1UL];
	if (nullptr == strcpy(buff, str)) {
		delete[] buff;
		return nullptr;
	}
	return buff;
}
