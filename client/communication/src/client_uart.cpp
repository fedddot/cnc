#include <stdexcept>
#include <string>

#include "uart.hpp"

#include "client_uart.hpp"

using namespace communication;
using namespace common;

static uart_inst_t *convert_id(ClientUart::UartId id);
static uint convert_baud(ClientUart::BaudRate baud);
static uint convert_data_bits(ClientUart::BitsNumber bits_num);
static uint convert_stop_bits(ClientUart::StopBits stop_bits);
// static uart_parity_t convert_parity(ClientUart::Parity parity);

ClientUart::ClientUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, const std::string& path): Uart(baud_rate, parity, stop_bits, bits_number), m_path(path) {
}

ClientUart::~ClientUart() noexcept {

}

void ClientUart::send(const std::vector<char>& data) {
	std::for_each(data.begin(), data.end(),
		[&](const auto& ch) {

		}
	);
}

void ClientUart::init() {

}

void ClientUart::deinit() {

}

static uint convert_baud(ClientUart::BaudRate baud) {
	switch (baud) {
	case ClientUart::BaudRate::B9600:
		return static_cast<uint>(9600);
	case ClientUart::BaudRate::B115200:
		return static_cast<uint>(115200);	
	default:
		break;
	}
	throw std::invalid_argument("unsupported baud rate received");
}

static uint convert_data_bits(ClientUart::BitsNumber bits_num) {
	switch (bits_num) {
	case ClientUart::BitsNumber::BN5:
		return static_cast<uint>(5);
	case ClientUart::BitsNumber::BN6:
		return static_cast<uint>(6);
	case ClientUart::BitsNumber::BN7:
		return static_cast<uint>(7);
	case ClientUart::BitsNumber::BN8:
		return static_cast<uint>(8);
	default:
		break;
	}
	throw std::invalid_argument("unsupported bits number received");
}

static uint convert_stop_bits(ClientUart::StopBits stop_bits) {
	switch (stop_bits) {
	case ClientUart::StopBits::ONE:
		return static_cast<uint>(1);
	case ClientUart::StopBits::TWO:
		return static_cast<uint>(2);
	default:
		break;
	}
	throw std::invalid_argument("unsupported stop bits number received");
}

// static uart_parity_t convert_parity(ClientUart::Parity parity) {
// 	switch (parity) {
// 	case ClientUart::Parity::NONE:
// 		return UART_PARITY_NONE;
// 	case ClientUart::Parity::EVEN:
// 		return UART_PARITY_EVEN;
// 	case ClientUart::Parity::ODD:
// 		return UART_PARITY_ODD;
// 	default:
// 		break;
// 	}
// 	throw std::invalid_argument("unsupported parity received");
// }