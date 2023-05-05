#ifndef	__MESSAGE_PARSER_HPP__
#define	__MESSAGE_PARSER_HPP__

#include <vector>

#include "message.hpp"
#include "ilistener.hpp"

namespace message
{
	class MessageParser: public common::IListener<const char&>
	{
	public:
		MessageParser(const std::vector<char>& start_signature, const std::size_t& length_field_size);
		virtual ~MessageParser() noexcept = default;
		virtual void setMessageListener(common::IListener<const Message&> *message_listener_ptr);
		virtual void onEvent(const char& event) override;
	private:
		enum ParserState {
			MATCHING_SIGNATURE,
			READING_MSG_LENGTH,
			READING_MSG_DATA
		};

		std::vector<char> m_start_signature;
		std::size_t m_length_field_size;
		common::IListener<const Message&> *m_message_listener_ptr;

		ParserState m_state;

		std::size_t m_start_signature_size;
		std::vector<char> m_reading_buff;

		std::size_t m_msg_size;

		void handleMatchingSignature(const char& event);
		void handleReadingMsgLength(const char& event);
		void handleReadingMsgData(const char& event);

		void resetParserState(void);

		static std::size_t parseMessageSize(const std::vector<char>& buff);
		static Message parseMessage(const std::vector<char>& buff);
	}; // MessageParser
} // message

#endif // __MESSAGE_PARSER_HPP__