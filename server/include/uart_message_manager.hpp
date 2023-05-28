#ifndef	__UART_MESSAGE_MANAGER_HPP__
#define	__UART_MESSAGE_MANAGER_HPP__

#include <cstddef>
#include <vector>
#include "message_manager.hpp"

namespace cnc_system {
	class UartMessageManager: public MessageManager {
	public:
		UartMessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size, const std::size_t& baud_rate = 115200);
		UartMessageManager(const UartMessageManager& other) = delete;
		UartMessageManager& operator=(const UartMessageManager& other) = delete;
		virtual ~UartMessageManager() noexcept override;
		virtual void send(const std::vector<char>& message) override;
		virtual void setMessageListener(common::IListener<const std::vector<char>&> *message_listener_ptr);
		virtual void onEvent(char event);
	private:
		std::size_t m_baud_rate;
	}; // class UartMessageManager
} // namespace cnc_system

#endif // __UART_MESSAGE_MANAGER_HPP__