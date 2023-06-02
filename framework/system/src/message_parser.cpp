#include <stdexcept>
#include <cstddef>
#include <vector>
#include <string>
#include "message_parser.hpp"

using namespace cnc_system;

static const std::invalid_argument s_invalid_argument_exp("invalid argument");

MessageParser::MessageParser(const std::vector<char>& start_signature, const std::size_t& length_field_size) : m_start_signature(start_signature), m_start_signature_size(start_signature.size()), m_length_field_size(length_field_size), m_message_listener_ptr(nullptr) {
	if (m_start_signature_size < 1) {
		throw s_invalid_argument_exp;
	}
	if ((m_length_field_size < 1) || (m_length_field_size > sizeof(size_t))) {
		throw s_invalid_argument_exp;
	}
	resetParserState();
}

void MessageParser::setMessageListener(common::IListener<const std::vector<char>&> *message_listener_ptr) {
	m_message_listener_ptr = message_listener_ptr;
}

void MessageParser::onEvent(char event) {
	switch (m_state) {
	case MATCHING_SIGNATURE:
		handleMatchingSignature(event);
		break;
	case READING_MSG_LENGTH:
		handleReadingMsgLength(event);
		break;
	case READING_MSG_DATA:
		handleReadingMsgData(event);
		break;
	default:
		resetParserState();
		break;
	}
}

void MessageParser::handleMatchingSignature(const char& event) {
	m_reading_buff.push_back(event);
	if (m_reading_buff.size() < m_start_signature_size) {
		return;
	}
	if (m_reading_buff == m_start_signature) {
		m_state = READING_MSG_LENGTH;
		m_reading_buff.clear();
		return;
	}
	m_reading_buff.erase(m_reading_buff.begin());
}

void MessageParser::handleReadingMsgLength(const char& event) {
	m_reading_buff.push_back(event);
	if (m_reading_buff.size() < m_length_field_size) {
		return;
	}
	m_msg_size = parseMessageSize(m_reading_buff);
	if (m_msg_size == 0) {
		resetParserState();
		return;
	}
	m_reading_buff.clear();
	m_state = READING_MSG_DATA;
}

void MessageParser::handleReadingMsgData(const char& event) {
	m_reading_buff.push_back(event);
	if (m_reading_buff.size() < m_msg_size) {
		return;
	}
	if (nullptr != m_message_listener_ptr) {
		m_message_listener_ptr->onEvent(m_reading_buff);
	}
	resetParserState();
}

void MessageParser::resetParserState(void) {
	m_state = MATCHING_SIGNATURE;
	m_reading_buff.clear();
}

std::size_t MessageParser::parseMessageSize(const std::vector<char>& buff) {
	static const std::size_t bits_in_byte = 8UL;
	std::size_t msg_size = 0;
	for (auto iter = buff.begin(); buff.end() != iter; ++iter) {
		msg_size <<= bits_in_byte;
		msg_size |= (std::size_t)(*iter);
	}
	return msg_size;
}

std::vector<char> MessageParser::sizeToVector(const std::size_t& msg_size) const {
	static const std::size_t bits_in_byte(8UL);
	static const std::size_t mask(0xFF);
	std::size_t size(msg_size);
	std::vector<char> output;
	for (std::size_t shift = 0; shift < m_length_field_size; ++shift) {
		char block = static_cast<char>(size >> ((m_length_field_size - shift - 1) * bits_in_byte)) & mask;
		output.push_back(block);
	}
	return output;
}