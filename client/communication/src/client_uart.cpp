#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <poll.h>

#include <stdexcept>
#include <memory>
#include <string>
#include <atomic>
#include <thread>

#include "uart.hpp"

#include "client_uart.hpp"

using namespace communication;
using namespace common;

// static functions declarations
static struct termios read_tty_config(int port_fd);
static void set_parity(ClientUart::Parity parity, struct termios *config);
static void set_stop_bits(ClientUart::StopBits stop_bits, struct termios *config);
static void set_data_bits(ClientUart::BitsNumber bits_num, struct termios *config);
static void set_baud_rate(ClientUart::BaudRate baud_rate, struct termios *config);
static void set_other_configs(struct termios *config);


ClientUart::ClientUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, const std::string& path): Uart(baud_rate, parity, stop_bits, bits_number), m_port_fd(open_port(path)), m_polling_timeout(3), m_is_polling_thread_running(false) {
	config_port();
	m_is_polling_thread_running.store(true, std::memory_order_release);
	m_polling_thread = std::thread(&ClientUart::poll_fd, this);
}

ClientUart::~ClientUart() noexcept {
	m_is_polling_thread_running.store(false, std::memory_order_release);
	m_polling_thread.join();
	close_port(m_port_fd);
	m_port_fd = BAD_FD;
}

void ClientUart::send(const std::vector<char>& data) {
	std::size_t data_size(data.size());
	char *buffer = new char[data_size];
	for (std::size_t i = 0; i < data_size; ++i) {
		buffer[i] = data[i];
	}
	auto write_result = write(m_port_fd, buffer, data_size);
	delete[] buffer;
	buffer = nullptr;
	if (data_size != static_cast<std::size_t>(write_result)) {
		throw std::runtime_error("failed to write into the serial port");
	}
}

int ClientUart::open_port(const std::string& path) {
	int port_fd = open(path.c_str(), O_RDWR);
	if (BAD_FD == port_fd) {
		throw std::runtime_error("failed to open port " + path);
	}
	return port_fd;
}

void ClientUart::close_port(int port_fd) {
	if (0 != close(port_fd)) {
		throw std::runtime_error("failed to close the port");
	}
}

void ClientUart::config_port() {
	struct termios tty_config = read_tty_config(m_port_fd);
	
	set_parity(parity(), &tty_config);
	set_stop_bits(stop_bits(), &tty_config);
	set_data_bits(bits_number(), &tty_config);
	set_baud_rate(baud_rate(), &tty_config);
	set_other_configs(&tty_config);

	if (0 != tcsetattr(m_port_fd, TCSANOW, &tty_config)) {
		throw std::runtime_error("failed to setup serial port");
	}
}

void ClientUart::poll_fd() {
	while (m_is_polling_thread_running.load(std::memory_order_acquire)) {
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
		char byte_read = 0x00;
		if (1 != read(m_port_fd, &byte_read, 1)) {
			continue;
		}
		dispatch(byte_read);	
	}
}



// static functions implementations

static struct termios read_tty_config(int port_fd) {
	struct termios tty_config;
	if (0 != tcgetattr(port_fd, &tty_config)) {
		throw std::runtime_error("failed to get current port config");
	}
	return tty_config;
}

static void set_parity(ClientUart::Parity parity, struct termios *config) {
	switch (parity) {
	case ClientUart::Parity::NONE:
		config->c_cflag &= ~PARENB;
		break;
	case ClientUart::Parity::EVEN:
		config->c_cflag |= PARENB;
		config->c_cflag &= ~PARODD;
		break;
	case ClientUart::Parity::ODD:
		config->c_cflag |= PARENB;
		config->c_cflag |= PARODD;
		break;	
	default:
		throw std::invalid_argument("unsupported parity");
		break;
	}
}

static void set_stop_bits(ClientUart::StopBits stop_bits, struct termios *config) {
	switch (stop_bits) {
	case ClientUart::StopBits::ONE:
		config->c_cflag &= ~CSTOPB;
		break;
	case ClientUart::StopBits::TWO:
		config->c_cflag |= CSTOPB;
		break;
	default:
		throw std::invalid_argument("unsupported stop bits");
		break;
	}
}

static void set_data_bits(ClientUart::BitsNumber bits_num, struct termios *config) {
	switch (bits_num) {
	case ClientUart::BitsNumber::BN5:
		config->c_cflag |= CS5;
		break;
	case ClientUart::BitsNumber::BN6:
		config->c_cflag |= CS6;
		break;
	case ClientUart::BitsNumber::BN7:
		config->c_cflag |= CS7;
		break;
	case ClientUart::BitsNumber::BN8:
		config->c_cflag |= CS8;
		break;
	default:
		throw std::invalid_argument("unsupported bits number");
		break;
	}
}

static void set_baud_rate(ClientUart::BaudRate baud_rate, struct termios *config) {
	speed_t speed = B9600;
	switch (baud_rate) {
	case ClientUart::BaudRate::BR9600:
		speed = B9600;
		break;
	case ClientUart::BaudRate::BR115200:
		speed = B115200;
		break;
	default:
		throw std::invalid_argument("unsupported baud rate");
		break;
	}
	if (0 != cfsetispeed(config, speed)) {
		throw std::runtime_error("failed to set baud rate");
	}
}

static void set_other_configs(struct termios *config) {
	config->c_cflag &= ~CRTSCTS;	// No RTS/CTS flow control
	config->c_cflag |= CLOCAL;		// Ignore ctrl lines
	config->c_cflag |= CREAD;		// Enable reading from the port
	config->c_lflag &= ~ICANON;		// Disable canonical mode (processing lines when \n is received)
	config->c_lflag &= ~ECHO;		// Disable echo
	config->c_lflag &= ~ECHOE;		// Disable erasure
	config->c_lflag &= ~ECHONL;		// Disable new-line echo
	config->c_lflag &= ~ISIG;		// Disable interpretation of SIG chars
	// Timing
	config->c_cc[VTIME] = 0;		// Non-blocking read
	config->c_cc[VMIN] = 0;

	config->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	
	config->c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	config->c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
}
