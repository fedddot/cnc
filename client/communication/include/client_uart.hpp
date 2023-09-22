#ifndef	__CLIENT_UART_HPP__
#define	__CLIENT_UART_HPP__

#include <string>
#include <atomic>
#include <thread>

#include "uart.hpp"

namespace communication {
	class ClientUart: public Uart {
	public:
		ClientUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, const std::string& path);
		ClientUart(const ClientUart& other) = delete;
		ClientUart& operator=(const ClientUart& other) = delete;

		~ClientUart() noexcept override;
		virtual void send(const std::vector<char>& data) override;
	private:
		enum {BAD_FD = -1};
		int m_port_fd;
		int m_polling_timeout;
		std::atomic_bool m_is_polling_thread_running;
		std::thread m_polling_thread;

		static int open_port(const std::string& path);
		static void close_port(int port_fd);
		
		void config_port();
		void poll_fd();
	}; // ClientUart
} // namespace communication
#endif // __CLIENT_UART_HPP__