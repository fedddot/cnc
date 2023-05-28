#include <cstddef>
#include <vector>
#include "message_manager.hpp"
#include "uart_message_manager.hpp"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_TX_PIN 	0
#define UART_RX_PIN 	1

using namespace cnc_system;

UartMessageManager::UartMessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size, const std::size_t& baud_rate): MessageManager(start_signature, length_field_size), m_baud_rate(baud_rate) {
	uart_init(uart0, (uint)m_baud_rate);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

UartMessageManager::~UartMessageManager() noexcept {
	uart_deinit(uart0);
}

void UartMessageManager::send(const std::vector<char>& message) {
	for (auto iter = message.begin(); message.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}
}