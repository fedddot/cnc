#include <stdexcept>
#include <vector>
#include <cstddef>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "idata.hpp"
#include "iserializer.hpp"
#include "json_serializer.hpp"
#include "byte_sender.hpp"
#include "client_sender.hpp"

using namespace communication;
using namespace data;
using namespace json;

ClientSender::ClientSender(const std::vector<char>& header, const std::size_t& length_field_size, const std::string& port_path): ByteSender(header, length_field_size, ByteSender::SerializerSmartPtr(new JsonSerializer)), m_port_path(port_path), m_port_fd(open_port(port_path)) {
	config_port(m_port_fd);
}

ClientSender::~ClientSender() noexcept {
	close_port(m_port_fd);
	m_port_fd = BAD_FD;
}

int ClientSender::open_port(const std::string& port_path) {
	int port_fd = open(port_path.c_str(), O_RDWR | O_NONBLOCK | O_NDELAY);
	if (BAD_FD == port_fd) {
		throw std::runtime_error("failed to open port " + port_path);
	}
	return port_fd;
}

void ClientSender::config_port(int port_fd) {
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
	if (0 != cfsetospeed(&tty_config, B115200)) {
		throw std::runtime_error("failed to set baud rate");
	}

	// Configure the port
	if (0 != tcsetattr(port_fd, TCSANOW, &tty_config)) {
		throw std::runtime_error("failed to setup serial port");
	}
}

void ClientSender::close_port(int port_fd) {
	if (0 != close(port_fd)) {
		throw std::runtime_error("failed to close the port");
	}
}

void ClientSender::send(const IData& data) {
	auto wrapped_data = prepare_data(data);
	std::unique_ptr<char> data_buffer(new char[wrapped_data.size()]);
	for (std::size_t i = 0; i < wrapped_data.size(); ++i) {
		(data_buffer.get())[i] = wrapped_data[i];
	}
	auto write_result = write(m_port_fd, static_cast<const void *>(data_buffer.get()), wrapped_data.size());
	if (wrapped_data.size() != static_cast<std::size_t>(write_result)) {
		throw std::runtime_error("failed to write into the serial port (" + std::to_string(write_result) + " successfully written, errno = " + std::to_string(errno) + ")");
	}
}