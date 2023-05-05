#include <stdexcept>
#include "message_parser.hpp"

using namespace message;

MessageParser::MessageParser(const std::vector<char>& start_signature, const std::size_t& length_field_size) : m_start_signature(start_signature), m_length_field_size(length_field_size), m_message_listener_ptr(nullptr) {
	if ((m_length_field_size < 1) || (m_length_field_size > sizeof(std::size_t))) {
		throw std::invalid_argument("invalid length field size");
	}
	resetParserState();
}

void MessageParser::setMessageListener(common::IListener<const Message&> *message_listener_ptr) {
	m_message_listener_ptr = message_listener_ptr;
}

void MessageParser::onEvent(const char& event) {
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
	if (m_reading_buff.size() < m_start_signature_size) {
		m_reading_buff.push_back(event);
		return;
	}
	if (m_reading_buff == m_start_signature) {
		m_state = READING_MSG_LENGTH;
		m_reading_buff.clear();
		handleReadingMsgLength(event);
		return;
	}
	m_reading_buff.erase(m_reading_buff.begin());
	m_reading_buff.push_back(event);
}

void MessageParser::handleReadingMsgLength(const char& event) {
	if (m_reading_buff.size() < m_length_field_size) {
		m_reading_buff.push_back(event);
		return;
	}
	m_msg_size = parseMessageSize(m_reading_buff);
	m_reading_buff.clear();
	handleReadingMsgLength(event);
}

void MessageParser::handleReadingMsgData(const char& event) {
	if (m_reading_buff.size() < m_msg_size) {
		m_reading_buff.push_back(event);
		return;
	}
	Message msg = parseMessage(m_reading_buff);
	resetParserState();
	if (nullptr != m_message_listener_ptr) {
		m_message_listener_ptr->onEvent(msg);
	}
}

void MessageParser::resetParserState(void) {
	m_start_signature_size = m_start_signature.size();
	m_state = MATCHING_SIGNATURE;
	m_reading_buff.clear();
}

std::size_t MessageParser::parseMessageSize(const std::vector<char>& buff) {
	static const std::size_t bits_in_byte = 8UL;
	std::size_t msg_size = 0;
	for (auto iter = buff.begin(); iter != buff.end(); ++iter) {
		msg_size |= (std::size_t)(*iter);
		msg_size <<= bits_in_byte;
	}
	return msg_size;
}

Message MessageParser::parseMessage(const std::vector<char>& buff) {
	std::string msg_string = "";
	for (auto iter = buff.begin(); iter != buff.end(); ++iter) {
		msg_string += *iter;
	}
	return Message(msg_string);
}