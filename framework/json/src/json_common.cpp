#include <sstream>

#include "json_common.hpp"

using namespace json;

JsonParserExceptionUnexpectedToken::JsonParserExceptionUnexpectedToken(const std::string& where, const std::size_t& pos_in_stream, const std::string& expected, const std::string& received) : m_where(where), m_pos_in_stream(pos_in_stream), m_expected(expected), m_received(received), m_msg("") {
	std::stringstream msg_stream;
	msg_stream << "In \"" << where << "\": unexpected token received (\"" << received << "\") while \"" << expected << "\" expected (position in input stream = " << pos_in_stream << ")";
	m_msg = msg_stream.str();
}

JsonParserExceptionUnexpectedEof::JsonParserExceptionUnexpectedEof(const std::string& where, char expected) : m_where(where), m_expected(expected), m_msg("") {
	std::stringstream msg_stream;
	msg_stream << "In \"" << where << "\": unexpected end of file while \'" << expected << "\' expected";
	m_msg = msg_stream.str();
}