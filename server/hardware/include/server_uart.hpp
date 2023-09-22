#ifndef	__SERVER_UART_HPP__
#define	__SERVER_UART_HPP__

#include <map>
#include <vector>

#include "ilistener.hpp"
#include "uart.hpp"

namespace communication {
	class ServerUart: public Uart {
	public:
		enum class UartId: int {
			UART0,
			UART1
		};
		ServerUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, UartId uart_id);
		ServerUart(const ServerUart& other) = delete;
		ServerUart& operator=(const ServerUart& other) = delete;

		~ServerUart() noexcept override;
		virtual void send(const std::vector<char>& data) override;

		inline UartId uart_id() const;
	private:
		UartId m_uart_id;
		static std::map<UartId, ServerUart *> s_interrupts_mapping;

		void init_gpios();
		void init();
		void init_data_format();
		void init_interrupts();

		static common::IListener<char> *get_char_listener(const UartId& id);
		static void on_uart0_rx();
		static void on_uart1_rx();
	}; // ServerUart

	inline ServerUart::UartId ServerUart::uart_id() const {
		return m_uart_id;
	}
} // namespace communication
#endif // __SERVER_UART_HPP__