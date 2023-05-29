#include <cstddef>
#include <vector>
#include "message_manager.hpp"
#include "uart_message_manager.hpp"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

using namespace cnc_system;

UartMessageManager::UartMessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size, const std::size_t& baud_rate): MessageManager(start_signature, length_field_size), m_baud_rate(baud_rate) {
	uart_init(uart0, (uint)m_baud_rate);
}

UartMessageManager::~UartMessageManager() noexcept {
	uart_deinit(uart0);
}

void UartMessageManager::send(const std::vector<char>& message) {
	auto start_signature = getStartSignature();
	for (auto iter = start_signature.begin(); start_signature.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}
	
	std::vector<char> size_encoded = sizeToVector(message.size(), getLengthFieldSize());
	for (auto iter = size_encoded.begin(); size_encoded.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}

	for (auto iter = message.begin(); message.end() != iter; ++iter) {
		uart_putc_raw(uart0, *iter);
	}
}

std::vector<char> UartMessageManager::sizeToVector(const std::size_t& msg_size, const std::size_t& length_field_size) {
	static const std::size_t bits_in_byte(8UL);
	static const std::size_t mask(0xFF);
	std::size_t size(msg_size);
	std::vector<char> output;
	for (std::size_t shift = 0; shift < length_field_size; ++shift) {
		char block = static_cast<char>(size >> ((length_field_size - shift - 1) * bits_in_byte)) & mask;
		output.push_back(block);
	}
	return output;
}