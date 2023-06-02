#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "message_manager.hpp"
#include "json_utils.hpp"
#include "json_object.hpp"
#include "json_string.hpp"
#include "ilistener.hpp"

using namespace cnc_system;
using namespace json;
using namespace common;

class ClientMessageManager: public MessageManager {
public:
	ClientMessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size, const std::string& uart_path): MessageManager(start_signature, length_field_size), m_uart_path(uart_path), m_isRunning(false) {
		m_uart_fd = open("/dev/ttyUSB0", O_RDWR);
		if (-1 == m_uart_fd) {
			throw std::runtime_error("failed to open UART on " + uart_path);
		}
		struct termios settings;
		tcgetattr(m_uart_fd, &settings);

		cfsetispeed(&settings, B9600);  // Set input baud rate
		cfsetospeed(&settings, B9600);  // Set output baud rate

		settings.c_cflag |= (CLOCAL | CREAD);  // Enable receiver and set local mode
		settings.c_cflag &= ~PARENB;           // Disable parity
		settings.c_cflag &= ~CSTOPB;           // Set one stop bit
		settings.c_cflag &= ~CSIZE;            // Clear data size bits
		settings.c_cflag |= CS8;               // Set 8 data bits

		tcsetattr(m_uart_fd, TCSANOW, &settings);

		m_reading_thread = std::thread(
			[&]() {
				while (m_isRunning) {
					char ch = '\0';
					read(m_uart_fd, &ch, 1);
					onEvent(ch);
				}
			}
		);
	}
	~ClientMessageManager() noexcept {
		m_isRunning = false;
		m_reading_thread.join();
	}
	virtual void send(const std::vector<char>& message) override {
		auto start_signature = getStartSignature();
		for (auto iter = start_signature.begin(); start_signature.end() != iter; ++iter) {
			char data = *iter;
			if (1 != write(m_uart_fd, &data, 1)) {
				throw std::runtime_error("failed to send char");
			}
		}
		
		std::vector<char> size_encoded = MessageParser::sizeToVector(message.size());
		for (auto iter = size_encoded.begin(); size_encoded.end() != iter; ++iter) {
			char data = *iter;
			if (1 != write(m_uart_fd, &data, 1)) {
				throw std::runtime_error("failed to send char");
			}
		}

		for (auto iter = message.begin(); message.end() != iter; ++iter) {
			char data = *iter;
			if (1 != write(m_uart_fd, &data, 1)) {
				throw std::runtime_error("failed to send char");
			}
		}
	}

private:
	std::string m_uart_path;
	int m_uart_fd;
	std::thread m_reading_thread;
	bool m_isRunning;
};

class MsgListener: public IListener<const std::vector<char>&> {
public:
	virtual void onEvent(const std::vector<char>& message) override {
		for (auto it = message.begin(); it != message.end(); ++it) {
			std::cout << *it;
		}
		std::cout << '\n';
	}
};

int main(void) {
	JsonObject led_on_cmd;
	led_on_cmd["led"] = std::shared_ptr<IJsonValue>(new JsonString("on"));
	std::vector<char> led_on_vec(stringToVector(led_on_cmd.getJsonString()));
	JsonObject led_off_cmd;
	led_off_cmd["led"] = std::shared_ptr<IJsonValue>(new JsonString("off"));
	std::vector<char> led_off_vec(stringToVector(led_off_cmd.getJsonString()));
	
	std::vector<char> sign(stringToVector("sign"));
	ClientMessageManager msg_manager(sign, 2UL, "/dev/ttyUSB0");
	MsgListener msg_listener;
	msg_manager.setMessageListener(&msg_listener);
	while (true) {
		std::cout << "sending command: " << led_on_cmd.getJsonString() << std::endl;
		msg_manager.send(led_on_vec);
		sleep(2);
		std::cout << "sending command: " << led_off_cmd.getJsonString() << std::endl;
		msg_manager.send(led_off_vec);
		sleep(2);
	}

	return 0;
}