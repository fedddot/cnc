#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>

#include "client_movement_task.hpp"
#include "client_data_sender.hpp"

using namespace cnc;
using namespace data;
using namespace common;


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

	ClientDataSender sender(header, length_field_size);
	
	double distance(0.01);
	double speed(3.158);
	ClientMovementTask::Axis axis(ClientMovementTask::Axis::AX);

	ClientMovementTask task(distance, speed, axis, &sender);
	task.execute();

	return 0;
}