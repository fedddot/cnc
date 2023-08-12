#include "serial_port_sender.hpp"

using namespace data;

SerialPortSender::SerialPortSender(const std::string& port_path, const std::size_t& baud_rate, const std::vector<char>& header, const std::size_t& length_field_size): DataSender(header, length_field_size), m_port_path(port_path), m_baud_rate(baud_rate) {

}