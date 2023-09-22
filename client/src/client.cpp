#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>

#include "client_uart.hpp"
#include "package_descriptor.hpp"
#include "package_manager.hpp"

using namespace common;
using namespace communication;

class TestListener: public IListener<std::vector<char>> {
public:
	virtual void onEvent(const std::vector<char>& event) override;
};

void TestListener::onEvent(const std::vector<char>& event) {
	std::string event_str(event.begin(), event.end());
	std::cout << "Payload: " << event_str << std::endl;
};

PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}

int main(void) {
	ClientUart uart(ClientUart::BaudRate::BR115200, ClientUart::Parity::NONE, ClientUart::StopBits::ONE, ClientUart::BitsNumber::BN8, "/dev/ttyUSB0");

	PackageManager package_manager(init_package_descriptor(), uart);
	uart.set_char_listener(&(package_manager.receiver()));

	TestListener listener;
	package_manager.receiver().set_data_listener(&listener);
	
	const std::string payload_str("asdkfjas;kdfmvnaksdnfvkavsdmfnk;avsdf askld fklasd fkjasdfh kjas dfh");
	const std::vector<char> payload(payload_str.begin(), payload_str.end());

	package_manager.sender().send(payload);

	sleep(1);
	return 0;
}