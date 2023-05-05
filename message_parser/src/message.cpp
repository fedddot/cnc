#include <cstddef>

#include "message.hpp"

using namespace message;

Message::Message(const std::string& msg) : m_msg(msg) {

}

const char *Message::data() const {
	return m_msg.c_str();
}

std::size_t Message::length() const {
	return m_msg.length();
}