#include <string>
#include <vector>
#include <memory>
#include <cstddef>

#include "mcu_resource_manager.hpp"
#include "message_parser.hpp"
#include "task_factory.hpp"
#include "json_object.hpp"
#include "json_string.hpp"

using namespace onboard;

int main(void) {
	McuResourceManager mcu_resources;
	std::vector<char> msg_signature{'a', 'b', 'c'};
	message::MessageParser msg_parser(msg_signature, 1);
	TaskFactory factory;

	mcu_resources.uart_byte_received_dispatcher.subscribe(&msg_parser);
	msg_parser.setMessageListener(&factory);

	json::JsonObject cmd_obj;
	cmd_obj.addObject("type", std::shared_ptr<json::IJsonValue>(new json::JsonString("REPORT_STATUS")));

	std::string msg = cmd_obj.getJsonString();
	std::size_t msg_size = msg.size();

	// send signature
	for (auto iter = msg_signature.begin(); iter != msg_signature.end(); ++iter) {
		mcu_resources.uart_byte_received_dispatcher.onEvent(*iter);
	}

	// send size
	mcu_resources.uart_byte_received_dispatcher.onEvent((char)msg_size);

	// send msg
	for (auto iter = msg.begin(); iter != msg.end(); ++iter) {
		mcu_resources.uart_byte_received_dispatcher.onEvent(*iter);
	}

	return 0;	
}