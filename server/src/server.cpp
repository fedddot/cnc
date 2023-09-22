#include <string>
#include <vector>

#include "server_uart.hpp"
#include "package_manager.hpp"
#include "package_descriptor.hpp"

using namespace hardware;
using namespace communication;

int main(void) {
	const std::string header_str("cnc_server");
	const std::size_t package_length_field_size(2UL);
	ServerUart uart(
		ServerUart::BaudRate::B115200, 
		ServerUart::Parity::NONE, 
		ServerUart::StopBits::ONE, 
		ServerUart::BitsNumber::BN8, 
		ServerUart::UartId::UART0
	);

	PackageDescriptor package_descriptor(
		std::vector<char>({'c'})
	);
	PackageManager package_manager()
}