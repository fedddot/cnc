#ifndef	__UART_MESSAGE_MANAGER_HPP__
#define	__UART_MESSAGE_MANAGER_HPP__

#include <stddef.h>
#include "message_manager.hpp"
#include "list.hpp"

namespace cnc_system {
	class UartMessageManager: public MessageManager {
	public:
		UartMessageManager(const data::List<char>& start_signature, const size_t& length_field_size);
		UartMessageManager(const UartMessageManager& other) = delete;
		UartMessageManager& operator=(const UartMessageManager& other) = delete;
		
		virtual void send(const data::List<char>& message) override;
	}; // McuResourceManager

} // namespace cnc_system

#endif // __UART_MESSAGE_MANAGER_HPP__