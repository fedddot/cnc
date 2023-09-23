#include "hardware/gpio.h"
#include "hardware/uart.h"

#include <stdexcept>
#include <algorithm>
#include <map>

#include "ilistener.hpp"
#include "uart.hpp"

#include "server_uart.hpp"

using namespace communication;
using namespace common;
using namespace hardware;

std::map<ServerUart::UartId, ServerUart *> hardware::ServerUart::s_interrupts_mapping;

static uart_inst_t *convert_id(ServerUart::UartId id);
static uint convert_baud(ServerUart::BaudRate baud);
static uint convert_data_bits(ServerUart::BitsNumber bits_num);
static uint convert_stop_bits(ServerUart::StopBits stop_bits);
static uart_parity_t convert_parity(ServerUart::Parity parity);

ServerUart::ServerUart(BaudRate baud_rate, Parity parity, StopBits stop_bits, BitsNumber bits_number, UartId uart_id): Uart(baud_rate, parity, stop_bits, bits_number), m_uart_id(uart_id) {
	init_gpios();
	init();
	init_data_format();
	init_interrupts();	
}

ServerUart::~ServerUart() noexcept {
	uart_deinit(convert_id(uart_id()));
	auto iter = s_interrupts_mapping.find(uart_id());
	if (s_interrupts_mapping.end() != iter) {
		s_interrupts_mapping.erase(iter);
	}
}

void ServerUart::send(const std::vector<char>& data) {
	uart_inst_t *uart_ptr = convert_id(uart_id());
	std::for_each(data.begin(), data.end(),
		[&](const auto& ch) {
			uart_putc(uart_ptr, ch);
		}
	);
}

void ServerUart::init_gpios() {
	enum {
		UART0_TX_PIN = 0,
		UART0_RX_PIN = 1,

		UART1_TX_PIN = 2,	// TODO: check it!
		UART1_RX_PIN = 3,	// TODO: check it!
	};
	uint tx_pin(0U); uint rx_pin(0U);
	switch (uart_id()) {
	case UartId::UART0:
		tx_pin = UART0_TX_PIN; rx_pin = UART0_RX_PIN;
		break;
	case UartId::UART1:
		tx_pin = UART1_TX_PIN; rx_pin = UART1_RX_PIN;
		break;	
	default:
		throw std::invalid_argument("unsupported uart_id received");
	}
	gpio_set_function(tx_pin, GPIO_FUNC_UART);
	gpio_set_function(rx_pin, GPIO_FUNC_UART);
}

void ServerUart::init() {
	uart_inst_t *uart_ptr = convert_id(uart_id());
	uint baud = convert_baud(baud_rate());	
	uart_init(uart_ptr, baud);
	uart_set_baudrate(uart_ptr, baud);
}

void ServerUart::init_data_format() {
	uart_inst_t *uart_ptr = convert_id(uart_id());
	uart_set_hw_flow(uart_ptr, false, false);
	uart_set_fifo_enabled(uart_ptr, false);
	uart_set_format(uart_ptr, convert_data_bits(bits_number()), convert_stop_bits(stop_bits()), convert_parity(parity()));
}

void ServerUart::init_interrupts() {
	int uart_irq = 0;
	void (*callback_ptr)(void) = nullptr;
	switch (uart_id()) {
	case ServerUart::UartId::UART0:
		uart_irq = UART0_IRQ;
		callback_ptr = &ServerUart::on_uart0_rx;
		break;
	case ServerUart::UartId::UART1:
		uart_irq = UART1_IRQ;
		callback_ptr = &ServerUart::on_uart1_rx;
		break;
	default:
		throw std::runtime_error("unsupported uart_id");
	}
	s_interrupts_mapping.insert({uart_id(), this});
	irq_set_exclusive_handler(uart_irq, callback_ptr);
	irq_set_enabled(uart_irq, true);
	uart_set_irq_enables(convert_id(uart_id()), true, false);
}

Uart *ServerUart::get_listening_uart(const UartId& id) {
	auto iter = s_interrupts_mapping.find(id);
	if ((s_interrupts_mapping.end() == iter) || (nullptr == iter->second)) {
		return nullptr;
	}
	return iter->second;
}

void ServerUart::on_uart0_rx() {
	const UartId id = UartId::UART0;
	Uart *listening_uart = get_listening_uart(id);
	if (nullptr != listening_uart) {
		listening_uart->dispatch(uart_getc(convert_id(id)));
	}
}

void ServerUart::on_uart1_rx() {
	const UartId id = UartId::UART1;
	Uart *listening_uart = get_listening_uart(id);
	if (nullptr != listening_uart) {
		listening_uart->dispatch(uart_getc(convert_id(id)));
	}
}

static uart_inst_t *convert_id(ServerUart::UartId id) {
	switch (id) {
	case ServerUart::UartId::UART0:
		return uart0;
	case ServerUart::UartId::UART1:
		return uart1;		
	default:
		break;
	}
	throw std::invalid_argument("unsupported uart_id received");
}

static uint convert_baud(ServerUart::BaudRate baud) {
	switch (baud) {
	case ServerUart::BaudRate::BR9600:
		return static_cast<uint>(9600);
	case ServerUart::BaudRate::BR115200:
		return static_cast<uint>(115200);	
	default:
		break;
	}
	throw std::invalid_argument("unsupported baud rate received");
}

static uint convert_data_bits(ServerUart::BitsNumber bits_num) {
	switch (bits_num) {
	case ServerUart::BitsNumber::BN5:
		return static_cast<uint>(5);
	case ServerUart::BitsNumber::BN6:
		return static_cast<uint>(6);
	case ServerUart::BitsNumber::BN7:
		return static_cast<uint>(7);
	case ServerUart::BitsNumber::BN8:
		return static_cast<uint>(8);
	default:
		break;
	}
	throw std::invalid_argument("unsupported bits number received");
}

static uint convert_stop_bits(ServerUart::StopBits stop_bits) {
	switch (stop_bits) {
	case ServerUart::StopBits::ONE:
		return static_cast<uint>(1);
	case ServerUart::StopBits::TWO:
		return static_cast<uint>(2);
	default:
		break;
	}
	throw std::invalid_argument("unsupported stop bits number received");
}

static uart_parity_t convert_parity(ServerUart::Parity parity) {
	switch (parity) {
	case ServerUart::Parity::NONE:
		return UART_PARITY_NONE;
	case ServerUart::Parity::EVEN:
		return UART_PARITY_EVEN;
	case ServerUart::Parity::ODD:
		return UART_PARITY_ODD;
	default:
		break;
	}
	throw std::invalid_argument("unsupported parity received");
}