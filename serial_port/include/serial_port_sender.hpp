#ifndef	__SERIAL_PORT_SENDER_HPP__
#define	__SERIAL_PORT_SENDER_HPP__

#include <vector>
#include <string>
#include <cstddef>

#include "data_sender.hpp"

namespace data {
	class SerialPortSender: public DataSender {
	public:
		SerialPortSender(const std::string& port_path, const std::size_t& baud_rate, const std::vector<char>& header, const std::size_t& length_field_size);

		SerialPortSender(const SerialPortSender& other) = default;
		SerialPortSender& operator=(const SerialPortSender& other) = default;

		inline std::string getPortPath() const;
		inline std::size_t getBaudRate() const;
	private:
		const std::string m_port_path;
		const std::size_t m_baud_rate;
	}; // SerialPortSender

	inline std::string SerialPortSender::getPortPath() const {
		return m_port_path;
	}

	inline std::size_t SerialPortSender::getBaudRate() const {
		return m_baud_rate;
	}
} // namespace data
#endif // __CLIENT_DATA_SENDER_HPP__