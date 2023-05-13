#include <string.h>
#include <stdlib.h>
#include "string.hpp"

using namespace server;

String::String(const char * const cstr) : m_string(strdup(cstr)) {
	if (nullptr == m_string) {
		// TODO: report about the problem
		return;
	}
	m_string_size = strlen(m_string);
}

String::String(const String& other) : m_string(strdup(other.m_string)) {
	if (nullptr == m_string) {
		// TODO: report about the problem
		return;
	}
	m_string_size = strlen(m_string);
}

String& String::operator=(const String& other) {
	char *string_buff = strdup(other.m_string);
	if (nullptr == string_buff) {
		// TODO: report about the problem
		return *this;
	}
	if (nullptr != m_string) {
		free(m_string);
		m_string = nullptr;
	}
	m_string = string_buff;
	string_buff = nullptr;
	m_string_size = strlen(m_string);
	return *this;
}

String::~String() noexcept {
	if (nullptr != m_string) {
		free(m_string);
		m_string = nullptr;
	}
}

size_t String::size() const {
	return m_string_size;
}

const char *String::c_str() const {
	return (const char *)m_string;
}