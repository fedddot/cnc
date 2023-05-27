#ifndef	__MESSAGE_PARSER_HPP__
#define	__MESSAGE_PARSER_HPP__

#include <stddef.h>
#include "list.hpp"
#include "ilistener.hpp"

namespace system {
	class MessageParser: public common::IListener<char> {
	public:
		MessageParser(const data::List<char>& start_signature, const size_t& length_field_size);
		MessageParser(const MessageParser& other) = delete;
		MessageParser& operator=(const MessageParser& other) = delete;
		virtual ~MessageParser() noexcept;
		
		virtual void setMessageListener(common::IListener<const data::List<char>&> *message_listener_ptr);
		virtual void onEvent(char event) override;
	private:
		enum ParserState {
			MATCHING_SIGNATURE,
			READING_MSG_LENGTH,
			READING_MSG_DATA
		};

		data::List<char> m_start_signature;
		size_t m_length_field_size;
		common::IListener<const data::List<char>&> *m_message_listener_ptr;

		ParserState m_state;

		size_t m_start_signature_size;
		data::List<char> m_reading_buff;

		size_t m_msg_size;

		void handleMatchingSignature(const char& event);
		void handleReadingMsgLength(const char& event);
		void handleReadingMsgData(const char& event);

		void resetParserState(void);

		static size_t parseMessageSize(const data::List<char>& buff);
		static data::List<char> parseMessage(const data::List<char>& buff);
	}; // class MessageParser
} // namespace message

#endif // __MESSAGE_PARSER_HPP__