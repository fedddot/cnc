#include <string>
#include <vector>

#include "server_uart.hpp"
#include "package_manager.hpp"
#include "package_descriptor.hpp"

using namespace common;
using namespace hardware;
using namespace communication;

class ResultReporter: public IListener<std::vector<char>> {
public:
	ResultReporter(PackageManager& package_manager);
	virtual void onEvent(const std::vector<char>& event) override;
private:
	PackageManager& m_package_manager;
};

static PackageDescriptor init_package_descriptor();

int main(void) {
	int result = 0;
	ServerUart uart(ServerUart::BaudRate::BR115200, ServerUart::Parity::NONE, ServerUart::StopBits::ONE, ServerUart::BitsNumber::BN8, ServerUart::UartId::UART0);
	
	try {
		PackageManager package_manager(init_package_descriptor(), uart);
		uart.set_char_listener(&(package_manager.receiver()));

		ResultReporter reporter(package_manager);
		package_manager.receiver().set_data_listener(&reporter);		

		while (true) {

		}
	} catch (const std::exception& e) {
		std::string err_msg_str("{\"error\": \"" + std::string(e.what()) + "\"}");
		std::vector<char> err_msg(err_msg_str.begin(), err_msg_str.end());
		uart.send(err_msg);
		result = -1;
	}
	return result;
}

ResultReporter::ResultReporter(PackageManager& package_manager): m_package_manager(package_manager) {

}

void ResultReporter::onEvent(const std::vector<char>& event) {
	m_package_manager.sender().send(event);
};

static PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}