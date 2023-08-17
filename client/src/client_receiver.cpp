#include <stdexcept>
#include <vector>
#include <cstddef>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <poll.h>

#include "json_parser.hpp"
#include "byte_receiver.hpp"
#include "client_receiver.hpp"

#define POLLING_TIMEOUT 2

using namespace communication;
using namespace data;
using namespace json;

ClientReceiver::ClientReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size, const std::string& port_path): ByteReceiver(header, length_field_size, max_data_size, ByteReceiver::ParserSmartPointer(new JsonParser)), m_port_path(port_path), m_port_fd(open_port(port_path)), m_polling_timeout(POLLING_TIMEOUT) {
	config_port(m_port_fd);
	m_polling_thread_run_permission.store(true, std::memory_order_release);
	m_polling_thread = std::thread(&ClientReceiver::poll_fd, this);
}

ClientReceiver::~ClientReceiver() noexcept {
	m_polling_thread_run_permission.store(false, std::memory_order_release);
	m_polling_thread.join();
	close_port(m_port_fd);
	m_port_fd = BAD_FD;
}

int ClientReceiver::open_port(const std::string& port_path) {
	int port_fd = open(port_path.c_str(), O_RDONLY);
	if (BAD_FD == port_fd) {
		throw std::runtime_error("failed to open port " + port_path);
	}
	return port_fd;
}

void ClientReceiver::config_port(int port_fd) {
	if (BAD_FD == port_fd) {
		throw std::invalid_argument("bad FD received");
	}
	termios tty_config;
	if (0 != tcgetattr(port_fd, &tty_config)) {
		throw std::runtime_error("failed to get current port config");
	}

	// Control config
	tty_config.c_cflag &= ~PARENB;	// No parity
	tty_config.c_cflag &= ~CSTOPB;	// One stop bit
	tty_config.c_cflag |= CS8;		// 8 bits per byte
	tty_config.c_cflag &= ~CRTSCTS; // No RTS/CTS flow control
	tty_config.c_cflag |= CLOCAL;	// Ignore ctrl lines
	tty_config.c_cflag |= CREAD;	// Enable reading from the port

	// Local modes
	tty_config.c_lflag &= ~ICANON;			// Disable canonical mode (processing lines when \n is received)
	tty_config.c_lflag &= ~ECHO;			// Disable echo
	tty_config.c_lflag &= ~ECHOE;			// Disable erasure
	tty_config.c_lflag &= ~ECHONL;			// Disable new-line echo
	tty_config.c_lflag &= ~ISIG; 			// Disable interpretation of SIG chars

	// Timing
	tty_config.c_cc[VTIME] = 0;				// Non-blocking read
	tty_config.c_cc[VMIN] = 0;

	tty_config.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	
	tty_config.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty_config.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

	// Baud rate
	if (0 != cfsetispeed(&tty_config, B115200)) {
		throw std::runtime_error("failed to set baud rate");
	}

	// Configure the port
	if (0 != tcsetattr(port_fd, TCSANOW, &tty_config)) {
		throw std::runtime_error("failed to setup serial port");
	}

	// <<TODO>>: Do I need to do some kind of flush in order to apply the settings?
}

void ClientReceiver::close_port(int port_fd) {
	if (0 != close(port_fd)) {
		throw std::runtime_error("failed to close the port");
	}
}

void ClientReceiver::poll_fd() {
	while (m_polling_thread_run_permission.load(std::memory_order_acquire)) {
		const short event_code = POLLIN;
		pollfd events {
			.fd = m_port_fd,
			.events = event_code,
			.revents = 0
		};

		int poll_result = poll(&events, 1, m_polling_timeout);
		if (-1 == poll_result) {
			throw std::runtime_error("failed to poll the port FD");
		}
		if (0 == poll_result) {
			continue;
		}
		char byte_ridden = 0x00;
		if (1 != read(m_port_fd, &byte_ridden, 1)) {
			continue;
		}
		onEvent(byte_ridden);
	}
}