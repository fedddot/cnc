#include "list.hpp"
#include "string.hpp"
#include "exception.hpp"
#include "exception_handler.hpp"
#include "message_parser.hpp"

using namespace system;
using namespace data;
using namespace except;

MessageParser::MessageParser(const List<char>& start_signature, const size_t& length_field_size) : m_start_signature(start_signature), m_start_signature_size(m_start_signature.size()), m_length_field_size(length_field_size), m_message_listener_ptr(nullptr) {
	if (m_start_signature_size < 1) {
		ExceptionHandler::getInstance()->onEvent(Exception("invalid start signature size"));
	}
	if ((m_length_field_size < 1) || (m_length_field_size > sizeof(size_t))) {
		ExceptionHandler::getInstance()->onEvent(Exception("invalid length field size"));
	}
	resetParserState();
}

void MessageParser::setMessageListener(common::IListener<const data::List<char>&> *message_listener_ptr) {
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
	m_reading_buff.pop_front();
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

size_t MessageParser::parseMessageSize(const List<char>& buff) {
	static const size_t bits_in_byte = 8UL;
	size_t msg_size = 0;
	for (auto iter = const_cast<List<char>&>(buff).begin(); !(iter.isEndIter()); ++iter) {
		msg_size <<= bits_in_byte;
		msg_size |= (size_t)(iter.get());
	}
	return msg_size;
}
