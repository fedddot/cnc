#include <string>
#include <vector>

#include "pico/stdlib.h"

#include "server_uart.hpp"
#include "package_manager.hpp"
#include "package_descriptor.hpp"
#include "output_gpio.hpp"
#include "server_task_manager.hpp"

using namespace common;
using namespace hardware;
using namespace communication;
using namespace task;

static PackageDescriptor init_package_descriptor();

int main(void) {
	ServerUart uart(ServerUart::BaudRate::BR115200, ServerUart::Parity::NONE, ServerUart::StopBits::ONE, ServerUart::BitsNumber::BN8, ServerUart::UartId::UART0);
	PackageManager package_manager(init_package_descriptor(), uart, uart);
	
	ServerTaskManager task_manager(package_manager);
	
	OutputGpio led(25);	
	while (true) {
		if (task_manager.is_task_pending()) {
			task_manager.run_pending_task();
		}
		led.write_value(OutputGpio::Value::HIGH);
		sleep_ms(500);
		led.write_value(OutputGpio::Value::LOW);
		sleep_ms(500);
	}	

	return 0;
}

static PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}