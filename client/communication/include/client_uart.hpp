#ifndef	__CLIENT_UART_HPP__
#define	__CLIENT_UART_HPP__

#include <string>

#include "uart.hpp"

namespace communication {
	class ClientUart: public Uart {
	public:
		ClientUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, const std::string& path);
		ClientUart(const ClientUart& other) = delete;
		ClientUart& operator=(const ClientUart& other) = delete;

		~ClientUart() noexcept override;
		virtual void send(const std::vector<char>& data) override;

		inline std::string path() const;
	private:
		std::string m_path;
		
		void init();
		void deinit();
	}; // ClientUart

	inline std::string ClientUart::path() const {
		return m_path;
	}
} // namespace communication
#endif // __CLIENT_UART_HPP__