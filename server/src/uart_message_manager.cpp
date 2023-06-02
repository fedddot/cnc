#include <cstddef>
#include <vector>
#include "message_manager.hpp"
#include "uart_message_manager.hpp"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

using namespace cnc_system;

static UartMessageManager *currentMessageManager = nullptr;
static int chars_rxed = 0;

void on_uart_rx() {
	if (nullptr == currentMessageManager) {
		return;
	}
    while (uart_is_readable(uart0)) {
		currentMessageManager->onEvent((char)uart_getc(uart0));
	}
}

UartMessageManager::UartMessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size, const std::size_t& baud_rate): MessageManager(start_signature, length_field_size), m_baud_rate(baud_rate) {
	uart_init(uart0, (uint)m_baud_rate);
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
	currentMessageManager = this;
}

UartMessageManager::~UartMessageManager() noexcept {
	irq_remove_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, false);
    uart_set_irq_enables(uart0, false, false);
	uart_deinit(uart0);
	currentMessageManager = nullptr;
}

void UartMessageManager::send(const std::vector<char>& message) {
	auto start_signature = getStartSignature();
	for (auto iter = start_signature.begin(); start_signature.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}	
	std::vector<char> size_encoded = sizeToVector(message.size());
	for (auto iter = size_encoded.begin(); size_encoded.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}
	for (auto iter = message.begin(); message.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}
}
