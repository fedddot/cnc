#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>

#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "ilistener.hpp"
#include "uart_handler.hpp"

using namespace communication;
using namespace common;

class BytesListener: public IListener<char> {
public:
	BytesListener(UartHandler& uart_handler): m_uart_handler(uart_handler) {}
	virtual void onEvent(const char& event) override {

	}
private:
	UartHandler& m_uart_handler;
};


int main(void) {
	std::vector<char> msg {'A', 'L', 'I', 'V'};
	UartHandler uart_handler(115200);
	BytesListener list(uart_handler);
	uart_handler.set_byte_listener(&list);

	uart_handler.start();

	while (true) {
		sleep_ms(1000);
		uart_handler.send(msg);
	}

	return 0;
}