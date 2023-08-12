#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "communication_manager.hpp"
#include "server_data_receiver.hpp"
#include "server_data_sender.hpp"
#include "server_task_manager.hpp"
#include "server_movement_task_creator.hpp"

using namespace cnc;
using namespace common;
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

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::size_t length_field_size(2UL);
	const std::size_t length_max(200UL);
	const std::string port_path("/dev/pts/3");

	ServerDataSender sender(header, length_field_size, port_path);
	ServerDataReceiver receiver(header, length_field_size, length_max, port_path);
	CommunicationManager comm_manager(receiver, sender);

	TaskFactory factory;
	factory.register_creator("movement", std::shared_ptr<TaskFactory::ITaskCreator>(new MovementTaskCreator(sender)));

	ServerTaskManager task_manager(factory);
	receiver.set_data_listener(&task_manager);

	while (true) {
		sleep(1);
	}

	return 0;
}