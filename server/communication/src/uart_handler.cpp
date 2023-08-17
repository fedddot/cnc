#include <stdexcept>
#include <algorithm>

#include <vector>
#include <cstddef>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/uart.h"

#include "ilistener.hpp"
#include "isender.hpp"
#include "uart_handler.hpp"

#define DATA_BITS 		8
#define STOP_BITS 		1
#define PARITY    		UART_PARITY_NONE
#define UART_ID 		uart0
#define UART_TX_PIN 	0
#define UART_RX_PIN 	1

static int chars_rxed = 0;

using namespace communication;

std::vector<UartHandler *> communication::UartHandler::m_handlers;

// RX interrupt handler
void UartHandler::on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
		for (auto iter = m_handlers.begin(); m_handlers.end() != iter; ++iter) {
			auto handler_ptr = *iter;
			if (nullptr == handler_ptr) {
				continue;
			}
			auto byte_listener_ptr = handler_ptr->get_byte_listener();
			if (nullptr == byte_listener_ptr) {
				continue;
			}
			byte_listener_ptr->onEvent(ch);
		}
    }
}

UartHandler::UartHandler(unsigned int baud_rate): m_baud_rate(baud_rate), m_byte_listener_ptr(nullptr), m_is_running(false) {

}

void UartHandler::send(const std::vector<char>& data) {
	std::for_each(
		data.begin(),
		data.end(),
		[&](const auto& ch) {
			uart_putc(UART_ID, ch);
		}
	);
}

void UartHandler::start() {
	if (is_running()) {
		throw std::runtime_error("already running");
	}
	uart_init(UART_ID, m_baud_rate);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    int __unused actual = uart_set_baudrate(UART_ID, m_baud_rate);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART0_IRQ, &UartHandler::on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

	m_handlers.push_back(this);
	m_is_running = true;
}

void UartHandler::stop() {
	if (!is_running()) {
		throw std::runtime_error("not running");
	}
	uart_deinit(UART_ID);
	auto iter = std::find(m_handlers.begin(), m_handlers.end(), this);
	if (m_handlers.end() != iter) {
		m_handlers.erase(iter);
	}
	m_is_running = false;
}