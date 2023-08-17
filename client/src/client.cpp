#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "client_receiver.hpp"
#include "client_sender.hpp"
#include "object.hpp"
#include "string.hpp"
#include "json_serializer.hpp"
// #include "client_movement_task.hpp"

using namespace common;
using namespace data;
using namespace json;
using namespace communication;

static inline std::vector<char> strToVector(const std::string& str) {
	std::vector<char> result;
	std::for_each(
		str.begin(),
		str.end(), 
		[&](const auto& iter) { 
			result.push_back(iter); 
		}
	);
	return result;
}

class MsgListener: public IListener<IData> {
public:
	virtual void onEvent(const IData& event);
};

void MsgListener::onEvent(const IData& event) {
	JsonSerializer serializer;
	auto serial_data = serializer.serialize(event);
	for (auto iter = serial_data.begin(); serial_data.end() != iter; ++iter) {
		std::cout << *iter;
	}
	std::cout << std::endl;
}

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::size_t length_field_size(2UL);
	const std::size_t length_max(200UL);
	const std::string port_path("/dev/ttyUSB0");

	ClientSender sender(header, length_field_size, port_path);
	ClientReceiver receiver(header, length_field_size, length_max, port_path);
	MsgListener listener;
	receiver.set_data_listener(&listener);

	Object task_conf;
	task_conf["type"] = std::shared_ptr<IData>(new String("movement"));
	task_conf["axis"] = std::shared_ptr<IData>(new String("1"));
	task_conf["distance"] = std::shared_ptr<IData>(new String("2.14"));
	task_conf["speed"] = std::shared_ptr<IData>(new String("8.74"));
	while (true) {
		sender.send(task_conf);
		sleep(1);
	}

	return 0;
}