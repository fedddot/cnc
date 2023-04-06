#include "message_parser.hpp"

using namespace common;

MessageParser::MessageParser(IListener<Message>& message_listener_ref_, const Signature& signature_, const std::size_t& size_field_size_) : m_message_listener_ref(message_listener_ref_), m_signature(signature_), m_size_field_size(size_field_size_), m_state(ParserState::EXCEPT_SIGNATURE), m_signature_bytes_captured(0UL), m_message_size(0UL) {

}

void MessageParser::onEvent(const Byte& event) {
	switch (m_state)
	{
	case ParserState::EXCEPT_SIGNATURE:
		exceptSignatureHandler(event);
		break;
	case ParserState::EXCEPT_SIZE:
		exceptSizeHandler(event);
		break;
	case ParserState::EXCEPT_DATA:
		exceptDataHandler(event);
		break;	
	default:
		break;
	}	
}

void MessageParser::exceptSignatureHandler(const Byte& event) {
	if (m_signature.size() == m_signature_bytes_captured) {
		m_signature_bytes_captured = 0UL;
		m_state = ParserState::EXCEPT_SIZE;
		onEvent(event);
		return;
	}

	if (m_signature[m_signature_bytes_captured] == event) {
		++m_signature_bytes_captured;
	} else {
		m_signature_bytes_captured = 0;
		if (m_signature[m_signature_bytes_captured] == event) {
		++m_signature_bytes_captured;
		}
	}
}

// abc - sig
// aabc

void MessageParser::exceptSizeHandler(const Byte& event) {
	if (m_message_size_buff.size() == m_size_field_size) {
		m_message_size = parseMessageSize(m_message_size_buff);
		m_message_size_buff.clear();
		m_state = ParserState::EXCEPT_DATA;
		onEvent(event);
		return;
	}

	m_message_size_buff.push_back(event);
}

void MessageParser::exceptDataHandler(const Byte& event) {
	if (0 == m_message_size) {
		m_message_listener_ref.onEvent(m_message);
		m_message.clear();
		m_message_size = 0UL;
		m_state = ParserState::EXCEPT_SIGNATURE;
		onEvent(event);
		return;
	}
	m_message.push_back(event);
	--m_message_size;

	if (0 == m_message_size) {
		m_message_listener_ref.onEvent(m_message);
		m_message.clear();
		m_message_size = 0UL;
		m_state = ParserState::EXCEPT_SIGNATURE;
	}
}

std::size_t MessageParser::parseMessageSize(const std::vector<Byte>& message_size_buff_) {
	std::size_t message_size = 0;
	for (auto it = message_size_buff_.begin(); it != message_size_buff_.end(); ++it) {
		message_size <<= BITS_IN_BYTE;
		message_size |= (std::size_t)(*it);
	}
	return message_size;
}