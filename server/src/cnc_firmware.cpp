#include <cstddef>
#include <vector>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "uart_message_manager.hpp"
#include "json_utils.hpp"
#include "ilistener.hpp"

#include "json_object.hpp"
#include "json_string.hpp"

#define BAUD_RATE 		9600
#define UART_TX_PIN 	0
#define UART_RX_PIN 	1

using namespace cnc_system;
using namespace json;
using namespace common;

void hardwareSetUp() {
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

class MessageListener: public IListener<const std::vector<char>&> {
public:
	MessageListener(UartMessageManager& msg_manager): m_msg_manager(msg_manager), m_led_pin(PICO_DEFAULT_LED_PIN) {
		gpio_init(m_led_pin);
		gpio_set_dir(m_led_pin, GPIO_OUT);
	}
	virtual void onEvent(const std::vector<char>& event) override {
		JsonObject object;
		std::vector<char>& non_const_event = const_cast<std::vector<char>&>(event);
		object.parse(non_const_event.begin(), non_const_event.end());
		m_msg_manager.send(stringToVector("\n\nevent received: "));
		m_msg_manager.send(event);
		m_msg_manager.send(stringToVector("\n\nobject parsed: "));
		m_msg_manager.send(stringToVector(object.getJsonString()));
		m_msg_manager.send(stringToVector("\n\n"));
		auto iter = object.find("led");
		if (object.end() != iter) {
			JsonString *action = (JsonString *)(object["led"].get());
			if ("on" == *action) {
				gpio_put(m_led_pin, 1);
				return;
			}
			if ("off" == *action) {
				gpio_put(m_led_pin, 0);
				return;
			}
		}
	}
private:
	UartMessageManager& m_msg_manager;
	const uint m_led_pin;
};

int main() {
	hardwareSetUp();

	std::vector<char> sign(stringToVector("sign"));
    UartMessageManager msg_manager(sign, 2UL, BAUD_RATE);
	MessageListener msg_listener(msg_manager);
	msg_manager.setMessageListener(&msg_listener);
	JsonObject start_up_msg;
	start_up_msg["msg"] = std::shared_ptr<IJsonValue>(new JsonString("the server is up"));
	msg_manager.send(stringToVector(start_up_msg.getJsonString()));
	while (1)
        tight_loop_contents();
	return 0;
}