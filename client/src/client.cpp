#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

#include "client_uart.hpp"
#include "package_descriptor.hpp"
#include "package_manager.hpp"

using namespace common;
using namespace communication;

class ResultReporter: public IListener<std::vector<char>> {
public:
	ResultReporter(std::condition_variable& cond_var);
	virtual void onEvent(const std::vector<char>& event) override;
private:
	std::condition_variable& m_cond_var;
};

static std::vector<std::string> get_args(int argc, char **argv);
static std::vector<char> read_task_payload(const std::string& task_file_path);
static PackageDescriptor init_package_descriptor();

int main(int argc, char **argv) {
	int result = 0;
	try {
		auto args = get_args(argc, argv);
		if (3 > args.size()) {
			throw std::invalid_argument("wrong command line arguments. format: " + args[0] + " PORT_PATH TASK_JSON_PATH");
		}
		const std::string uart_path(args[1]);
		const std::string task_path(args[2]);
		
		ClientUart uart(ClientUart::BaudRate::BR115200, ClientUart::Parity::NONE, ClientUart::StopBits::ONE, ClientUart::BitsNumber::BN8, uart_path);

		PackageManager package_manager(init_package_descriptor(), uart);
		uart.set_char_listener(&(package_manager.receiver()));

		std::mutex report_received_mux;
		std::condition_variable report_received_cond;

		ResultReporter reporter(report_received_cond);
		package_manager.receiver().set_data_listener(&reporter);
		
		auto payload = read_task_payload(task_path);
		package_manager.sender().send(payload);

		std::unique_lock<std::mutex> lock(report_received_mux);
		report_received_cond.wait(lock);
	} catch (const std::exception& e) {
		std::cout << "client: an exception catched: " << e.what() << std::endl;
		result = -1;
	}
	return result;
}

ResultReporter::ResultReporter(std::condition_variable& cond_var): m_cond_var(cond_var) {

}

void ResultReporter::onEvent(const std::vector<char>& event) {
	std::string event_str(event.begin(), event.end());
	std::cout << "Report:" << std::endl << event_str << std::endl;
	m_cond_var.notify_all();
};

static PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}

static std::vector<char> read_task_payload(const std::string& task_file_path) {
	std::vector<char> task_payload;
	std::ifstream task_file(task_file_path);
	if (!task_file.is_open()) {
		throw std::runtime_error("failed to open " + task_file_path);
	}
	char ch('A');
	while (EOF != (ch = task_file.get())) {
		task_payload.push_back(static_cast<char>(ch));
	}
	return task_payload;
}

static std::string port_path(int argc, char **argv) {
	if (2 > argc) {
		throw std::invalid_argument("wrong arguments number");
	}
	if (nullptr == argv) {
		throw std::invalid_argument("argv argument must not be a nullptr");
	}
	return std::string(argv[1]);
}

static std::vector<std::string> get_args(int argc, char **argv) {
	if (0 > argc) {
		throw std::invalid_argument("wrong arguments number");
	}
	if (nullptr == argv) {
		throw std::invalid_argument("argv argument must not be a nullptr");
	}
	std::vector<std::string> args;
	for (int arg_ind = 0; arg_ind < argc; ++arg_ind) {
		args.push_back(std::string(argv[arg_ind]));
	}
	return args;
}