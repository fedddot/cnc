#include <cstddef>
#include <vector>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "uart_message_manager.hpp"
#include "json_utils.hpp"

#define BAUD_RATE 		115200
#define UART_TX_PIN 	0
#define UART_RX_PIN 	1

using namespace cnc_system;
using namespace json;

void hardwareSetUp() {
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

int main() {
	hardwareSetUp();

	std::vector<char> sign(stringToVector("sign"));
	std::vector<char> msg(stringToVector("Hello\n\r"));
    UartMessageManager msg_manager(sign, 2UL, BAUD_RATE);
	while (true) {
		msg_manager.send(msg);
	}
	return 0;
}