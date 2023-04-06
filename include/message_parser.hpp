#ifndef	__MESSAGE_PARSER_HPP__
#define	__MESSAGE_PARSER_HPP__

#include <vector>
#include <cstddef>

#include "common_types.hpp"
#include "ilistener.hpp"

namespace common {

	class MessageParser: public IListener<Byte> {
	public:
		typedef std::vector<Byte> Message;
		typedef std::vector<Byte> Signature;

		MessageParser(IListener<Message>& message_listener_ref_, const Signature& signature_, const std::size_t& size_field_size_);
		virtual void onEvent(const Byte& event) override;
	private:
		enum class ParserState : int {
			EXCEPT_SIGNATURE = 0,
			EXCEPT_SIZE = 1,
			EXCEPT_DATA = 2
		};

		enum {BITS_IN_BYTE = 8};

		IListener<Message>& m_message_listener_ref;
		Signature m_signature;
		std::size_t m_size_field_size;

		ParserState m_state;
		std::size_t m_signature_bytes_captured;

		std::vector<Byte> m_message_size_buff;

		std::size_t m_message_size;
		Message m_message;

		void exceptSignatureHandler(const Byte& event);
		void exceptSizeHandler(const Byte& event);
		void exceptDataHandler(const Byte& event);

		static std::size_t parseMessageSize(const std::vector<Byte>& message_size_buff_);
	};

	// <signature><std::size_t - msg size><msg_data>

}

#endif // __MESSAGE_PARSER_HPP__