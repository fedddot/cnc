#ifndef	__MESSAGE_PARSER_HPP__
#define	__MESSAGE_PARSER_HPP__

#include <cstddef>
#include <vector>
#include "ilistener.hpp"

namespace cnc_system {
	class MessageParser: public common::IListener<char> {
	public:
		MessageParser(const std::vector<char>& start_signature, const std::size_t& length_field_size);
		MessageParser(const MessageParser& other) = delete;
		MessageParser& operator=(const MessageParser& other) = delete;
		virtual void onEvent(char event) override;
		void setMessageListener(common::IListener<const std::vector<char>&> *message_listener_ptr);
		inline const std::vector<char> getStartSignature() const;
		inline std::size_t getLengthFieldSize() const;
		std::vector<char> sizeToVector(const std::size_t& msg_size) const;
	private:
		enum ParserState {
			MATCHING_SIGNATURE,
			READING_MSG_LENGTH,
			READING_MSG_DATA
		};

		std::vector<char> m_start_signature;
		std::size_t m_start_signature_size;
		std::size_t m_length_field_size;
		common::IListener<const std::vector<char>&> *m_message_listener_ptr;

		ParserState m_state;

		std::vector<char> m_reading_buff;

		std::size_t m_msg_size;

		void handleMatchingSignature(const char& event);
		void handleReadingMsgLength(const char& event);
		void handleReadingMsgData(const char& event);

		void resetParserState(void);

		static std::size_t parseMessageSize(const std::vector<char>& buff);
	}; // class MessageParser

	inline const std::vector<char> MessageParser::getStartSignature() const {
		return m_start_signature;
	}

	inline std::size_t MessageParser::getLengthFieldSize() const {
		return m_length_field_size;
	}
} // namespace message

#endif // __MESSAGE_PARSER_HPP__