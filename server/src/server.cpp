#include <string>
#include <vector>

#include "pico/stdlib.h"

#include "server_uart.hpp"
#include "package_manager.hpp"
#include "package_descriptor.hpp"
#include "output_gpio.hpp"

using namespace common;
using namespace hardware;
using namespace communication;

class ResultReporter: public IListener<std::vector<char>> {
public:
	ResultReporter(PackageManager& package_manager);
	virtual void on_event(const std::vector<char>& event) override;
private:
	PackageManager& m_package_manager;
};

static PackageDescriptor init_package_descriptor();

int main(void) {
	ServerUart uart(ServerUart::BaudRate::BR115200, ServerUart::Parity::NONE, ServerUart::StopBits::ONE, ServerUart::BitsNumber::BN8, ServerUart::UartId::UART0);
	PackageManager package_manager(init_package_descriptor(), uart, uart);
	ResultReporter reporter(package_manager);
	package_manager.receiver().subscribe(&reporter);	

	OutputGpio led(25);	
	while (true) {
		led.write_value(OutputGpio::Value::HIGH);
		sleep_ms(500);
		led.write_value(OutputGpio::Value::LOW);
		sleep_ms(500);
	}	

	return 0;
}

ResultReporter::ResultReporter(PackageManager& package_manager): m_package_manager(package_manager) {

}

void ResultReporter::on_event(const std::vector<char>& event) {
	m_package_manager.sender().send(event);
};

static PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}