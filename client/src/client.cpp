#include <unistd.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

#include "client_uart.hpp"
#include "package_descriptor.hpp"
#include "package_manager.hpp"
#include "task_result_listener.hpp"
#include "json_parser.hpp"
#include "json_serializer.hpp"
#include "array.hpp"
#include "idata.hpp"

using namespace json;
using namespace data;
using namespace common;
using namespace communication;

static std::vector<std::string> get_args(int argc, char **argv);
static Array read_tasks(const std::string& task_file_path);
static void perform_task(const IData& task_data, PackageManager& package_manager, TaskResultListener& result_listener);
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

		PackageManager package_manager(init_package_descriptor(), uart, uart);

		TaskResultListener result_listener;
		package_manager.receiver().subscribe(&result_listener);
		
		Array tasks(read_tasks(task_path));

		std::for_each(
			tasks.begin(),
			tasks.end(),
			[&](auto& task_data) {
				perform_task(*task_data, package_manager, result_listener);
			}
		);

	} catch (const std::exception& e) {
		std::cout << "client: an exception catched: " << e.what() << std::endl;
		result = -1;
	}
	return result;
}

static PackageDescriptor init_package_descriptor() {
	const std::string header_str("cnc_package_header");
	const std::vector<char> header(header_str.begin(), header_str.end());
	const std::size_t package_length_field_size(2UL);
	return PackageDescriptor(header, package_length_field_size);
}

static Array read_tasks(const std::string& task_file_path) {
	std::vector<char> raw_data;
	std::ifstream task_file(task_file_path);
	if (!task_file.is_open()) {
		throw std::runtime_error("failed to open " + task_file_path);
	}
	char ch('A');
	while (EOF != (ch = task_file.get())) {
		raw_data.push_back(static_cast<char>(ch));
	}
	JsonParser parser;
	auto parsed_data = parser.parse(raw_data);
	return Array(dynamic_cast<Array&>(*parsed_data));;
}

static void perform_task(const IData& task_data, PackageManager& package_manager, TaskResultListener& result_listener) {
	JsonSerializer serializer;
	std::vector<char> serial_task_data(serializer.serialize(task_data));
	std::cout << "[C2S]: " << std::string(serial_task_data.begin(), serial_task_data.end()) << std::endl;
	package_manager.sender().send(serial_task_data);

	std::vector<char> result_data = result_listener.wait_result();
	std::cout << "[S2C]: " << std::string(result_data.begin(), result_data.end()) << std::endl;
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