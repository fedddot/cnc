#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#include "communication_manager.hpp"
#include "data_receiver.hpp"
#include "client_data_sender.hpp"
#include "client_movement_task.hpp"

using namespace communication;
using namespace cnc;

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

	CommunicationManager comm_manager(
		std::shared_ptr<IReceiver<char, const std::vector<char>&>>(
			new DataReceiver(header, length_field_size, length_max)
		), 
		std::shared_ptr<ISender<const std::vector<char>&>>(
			new ClientDataSender(header, length_field_size)
		)
	);

	ClientMovementTask task(0.155, 0.183, ClientMovementTask::Axis::AX, comm_manager);

	task.execute();

	comm_manager.onEvent('c');

	return 0;
}