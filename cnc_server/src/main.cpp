
#include <cstddef>
#include <iostream>
#include <algorithm>

#include "mcu_events.hpp"
#include "message_parser.hpp"
#include "ilistener.hpp"

using namespace common;
using namespace cnc_server;

class MessageListener: public IListener<MessageParser::Message> {
	virtual void onEvent(const MessageParser::Message& event) {
		for (std::size_t i = 0; i < event.size(); ++i) {
			std::cout << event[i];
		}
		std::cout << std::endl;
	}
};

int main()
{
	McuEvents mcu_events;
	std::vector<Byte> signature = {'a', 'b', 'c'};
	std::vector<Byte> signature_bad = {'a', 'a', 'b', 'c'};
	std::vector<Byte> msg = {(Byte)0x00, (Byte)0x06, 's', 'u', 'k', 'a', ' ', 'b'};
	MessageListener msg_listener;
	
	MessageParser parser(msg_listener, signature, 2UL);
	
	mcu_events.uart_byte_received_dispatcher.subscribe(&parser);

	std::for_each(
		signature_bad.begin(), 
		signature_bad.end(),
		[&](const Byte& byte) {
			mcu_events.uart_byte_received_dispatcher.onEvent(byte);
		}
	);

	std::for_each(
		msg.begin(), 
		msg.end(),
		[&](const Byte& byte) {
			mcu_events.uart_byte_received_dispatcher.onEvent(byte);
		}
	);

	return 0;
}
