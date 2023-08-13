#include <stdexcept>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "server_data_sender.hpp"

using namespace communication;

ServerDataSender::ServerDataSender(const std::vector<char>& header, const std::size_t& length_field_size, const std::string& port_path): DataSender(header, length_field_size), m_port_path(port_path), m_port_fd(open_port(port_path)) {
	config_port(m_port_fd);
}

ServerDataSender::~ServerDataSender() noexcept {
	close_port(m_port_fd);
	m_port_fd = BAD_FD;
}

int ServerDataSender::open_port(const std::string& port_path) {
	int port_fd = open(port_path.c_str(), O_RDWR | O_NONBLOCK | O_NDELAY);
	if (BAD_FD == port_fd) {
		throw std::runtime_error("failed to open port " + port_path);
	}
	return port_fd;
}

void ServerDataSender::config_port(int port_fd) {
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
	// tty_config.c_cflag |= CREAD;	// Enable reading from the port (TODO: add when reading)

	// Local modes
	tty_config.c_lflag &= ~ICANON;			// Disable canonical mode (processing lines when \n is received)
	tty_config.c_lflag &= ~ECHO;			// Disable echo
	tty_config.c_lflag &= ~ECHOE;			// Disable erasure
	tty_config.c_lflag &= ~ECHONL;			// Disable new-line echo
	tty_config.c_lflag &= ~ISIG; 			// Disable interpretation of SIG chars

	// Output modes
	tty_config.c_oflag &= ~OPOST; 			// Disable special interpretation of output bytes
	tty_config.c_oflag &= ~ONLCR; 			// Disable conversion of \n

	// Timing
	tty_config.c_cc[VTIME] = 0;				// Non-blocking read
	tty_config.c_cc[VMIN] = 0;

	// Baud rate
	if (0 != cfsetospeed(&tty_config, B9600)) {
		throw std::runtime_error("failed to set baud rate");
	}

	// Configure the port
	if (0 != tcsetattr(port_fd, TCSANOW, &tty_config)) {
		throw std::runtime_error("failed to setup serial port");
	}

	// <<TODO>>: Do I need to do some kind of flush in order to apply the settings?
}

void ServerDataSender::close_port(int port_fd) {
	if (0 != close(port_fd)) {
		throw std::runtime_error("failed to close the port");
	}
}

std::vector<char> ServerDataSender::wrap_data(const std::vector<char>& data) const {
	std::vector<char> wrapped_data;

	auto header = get_header();
	std::for_each(
		header.begin(),
		header.end(),
		[&](const auto& iter) {
			wrapped_data.push_back(iter);
		}
	);

	auto data_size = serialize_data_size(data.size());
	std::for_each(
		data_size.begin(),
		data_size.end(),
		[&](const auto& iter) {
			wrapped_data.push_back(iter);
		}
	);

	std::for_each(
		data.begin(),
		data.end(),
		[&](const auto& iter) {
			wrapped_data.push_back(iter);
		}
	);
	
	return wrapped_data;
}

void ServerDataSender::send(const std::vector<char>& data) {
	auto wrapped_data = wrap_data(data);
	std::unique_ptr<char> data_buffer(new char[wrapped_data.size()]);
	for (std::size_t i = 0; i < wrapped_data.size(); ++i) {
		(data_buffer.get())[i] = wrapped_data[i];
	}
	auto write_result = write(m_port_fd, static_cast<const void *>(data_buffer.get()), wrapped_data.size());
	if (wrapped_data.size() != static_cast<std::size_t>(write_result)) {
		throw std::runtime_error("failed to write into the serial port (" + std::to_string(write_result) + " successfully written, errno = " + std::to_string(errno) + ")");
	}
}