#ifndef	__UART_HANDLER_HPP__
#define	__UART_HANDLER_HPP__

#include <vector>

#include "ilistener.hpp"
#include "isender.hpp"

namespace communication {
	class UartHandler: public ISender<std::vector<char>> {
	public:
		UartHandler(unsigned int baud_rate);
		UartHandler(const UartHandler& other) = delete;
		UartHandler& operator=(const UartHandler& other) = delete;
		
		inline void set_byte_listener(common::IListener<char> *byte_listener_ptr);
		inline common::IListener<char> *get_byte_listener();

		virtual void send(const std::vector<char>& data) override;

		void start();
		void stop();
		inline bool is_running() const;
	private:
		unsigned int m_baud_rate;
		common::IListener<char> *m_byte_listener_ptr;
		bool m_is_running;

		static std::vector<UartHandler *> m_handlers;
		static void on_uart_rx();
	}; // UartHandler

	inline void UartHandler::set_byte_listener(common::IListener<char> *byte_listener_ptr) {
		m_byte_listener_ptr = byte_listener_ptr;
	}

	inline common::IListener<char> *UartHandler::get_byte_listener() {
		return m_byte_listener_ptr;
	}

	inline bool UartHandler::is_running() const {
		return m_is_running;
	}

} // namespace communication
#endif // __UART_HANDLER_HPP__