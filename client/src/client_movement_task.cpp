#include <vector>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include "isender.hpp"
#include "idata.hpp"
#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "json_serializer.hpp"

#include "movement_task.hpp"
#include "client_movement_task.hpp"

using namespace communication;
using namespace cnc;
using namespace data;
using namespace json;

ClientMovementTask::ClientMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, communication::ISender<const std::vector<char>&>& sender): MovementTask(distance, speed, axis), m_sender(sender) {
}

void ClientMovementTask::execute() {
	Object task_config;
	task_config["type"] = std::shared_ptr<IData>(new String("movement"));
	task_config["distance"] = std::shared_ptr<IData>(new String(std::to_string(getDistance())));
	task_config["speed"] = std::shared_ptr<IData>(new String(std::to_string(getSpeed())));
	task_config["axis"] = std::shared_ptr<IData>(new String(std::to_string(static_cast<int>(getAxis()))));

	JsonSerializer serializer;
	auto task_config_serial = serializer.serialize(task_config);

	std::vector<char> serial_data;
	std::for_each(
		task_config_serial.begin(),
		task_config_serial.end(),
		[&](const auto& iter) {
			serial_data.push_back(iter);
		}
	);
	
	m_sender.send(serial_data);
}