#ifndef	__JSON_COMMON_HPP__
#define	__JSON_COMMON_HPP__

#include <stdexcept>
#include <string>

namespace json {
	enum class JsonSpecialChar : char {
		STRING_START = '\"',
		STRING_END = '\"',
		ARRAY_START = '[',
		ARRAY_END = ']',
		OBJECT_START = '{',
		OBJECT_END = '}',
		DELIMITER = ',',
		SEMICOLON = ':',
		SPACE = ' ',
		TAB = '\t',
		NEW_LINE = '\n'
	};

	class JsonParserExceptionUnexpectedToken: public std::exception {
	public:
		JsonParserExceptionUnexpectedToken(const std::string& where, const std::size_t& pos_in_stream, const std::string& expected, const std::string& received);
		JsonParserExceptionUnexpectedToken(const JsonParserExceptionUnexpectedToken& other) = default;
		JsonParserExceptionUnexpectedToken& operator=(const JsonParserExceptionUnexpectedToken& other) = default;
		virtual const char* what() const noexcept override;
	private:
		std::string m_where;
		std::size_t m_pos_in_stream;
		std::string m_expected;
		std::string m_received;
		std::string m_msg;
	};

	inline const char *JsonParserExceptionUnexpectedToken::what() const noexcept {
		return m_msg.c_str();
	}

	class JsonParserExceptionUnexpectedEof: public std::exception {
	public:
		JsonParserExceptionUnexpectedEof(const std::string& where);
		JsonParserExceptionUnexpectedEof(const JsonParserExceptionUnexpectedEof& other) = default;
		JsonParserExceptionUnexpectedEof& operator=(const JsonParserExceptionUnexpectedEof& other) = default;
		virtual const char* what() const noexcept override;
	private:
		std::string m_where;
		std::string m_msg;
	};

	inline const char *JsonParserExceptionUnexpectedEof::what() const noexcept {
		return m_msg.c_str();
	}
}

#endif // __JSON_COMMON_HPP__