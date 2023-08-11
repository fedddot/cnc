#ifndef	__CLIENT_SERIAL_PORT_SENDER_HPP__
#define	__CLIENT_SERIAL_PORT_SENDER_HPP__

#include <vector>
#include "serial_port_sender.hpp"

namespace data {
	class ClientSerialPortSender: public SerialPortSender {
	public:
		ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size);

		ClientDataSender(const ClientDataSender& other) = default;
		ClientDataSender& operator=(const ClientDataSender& other) = default;

		virtual void send(const std::vector<char>& data) override;
	private:
	}; // ClientDataSender
} // namespace data
#endif // __CLIENT_SERIAL_PORT_SENDER_HPP__