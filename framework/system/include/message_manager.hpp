#ifndef	__MESSAGE_MANAGER_HPP__
#define	__MESSAGE_MANAGER_HPP__

#include <cstddef>
#include <vector>

#include "isender.hpp"
#include "ilistener.hpp"
#include "message_parser.hpp"

namespace cnc_system {
	class MessageManager: public MessageParser, public common::ISender {
	public:
		MessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size);
		MessageManager(const MessageManager& other) = delete;
		MessageManager& operator=(const MessageManager& other) = delete;
		virtual void send(const std::vector<char>& message) = 0;
	}; // class MessageManager
} // namespace cnc_system

#endif // __MESSAGE_MANAGER_HPP__