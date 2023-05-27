#ifndef	__MESSAGE_MANAGER_HPP__
#define	__MESSAGE_MANAGER_HPP__

#include <stddef.h>

#include "isender.hpp"
#include "ilistener.hpp"
#include "list.hpp"
#include "message_parser.hpp"

namespace system {
	class MessageManager: public MessageParser, public ISender {
	public:
		MessageManager(const data::List<char>& start_signature, const size_t& length_field_size);
		MessageManager(const MessageManager& other) = delete;
		MessageManager& operator=(const MessageManager& other) = delete;

		virtual ~MessageManager() noexcept override;

		virtual void setMessageListener(common::IListener<const data::List<char>&> *message_listener_ptr);
		virtual void onEvent(char event);
		
		virtual void send(const data::List<char>& message) = 0;
	}; // class MessageManager
} // namespace system

#endif // __MESSAGE_MANAGER_HPP__