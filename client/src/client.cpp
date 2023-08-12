#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "communication_manager.hpp"
#include "client_data_receiver.hpp"
#include "client_data_sender.hpp"
#include "client_movement_task.hpp"

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

class MsgListener: public IListener<const std::vector<char>&> {
public:
	virtual void onEvent(const std::vector<char>& event);
};

void MsgListener::onEvent(const std::vector<char>& event) {
	std::for_each(
		event.begin(),
		event.end(),
		[&](const auto& iter) {
			std::cout << iter;
		}
	);
	std::cout << std::endl;
}

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::size_t length_field_size(2UL);
	const std::size_t length_max(200UL);
	const std::string port_path("/dev/pts/5");

	ClientDataSender sender(header, length_field_size, port_path);
	ClientDataReceiver receiver(header, length_field_size, length_max, port_path);
	MsgListener listener;
	receiver.set_data_listener(&listener);

	CommunicationManager comm_manager(receiver, sender);

	ClientMovementTask task(0.155, 0.183, ClientMovementTask::Axis::AX, comm_manager);

	task.execute();

	while (true) {
		sleep(1);
	}

	return 0;
}