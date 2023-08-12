#include <stdexcept>
#include <vector>
#include <memory>
#include <algorithm>

#include "idata.hpp"
#include "isender.hpp"
#include "object.hpp"
#include "string.hpp"
#include "json_serializer.hpp"

#include "server_movement_task.hpp"

using namespace cnc;
using namespace data;
using namespace json;

ServerMovementTask::ServerMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, communication::ISender<const std::vector<char>&>& sender): MovementTask(distance, speed, axis), m_sender(sender) {
}
#include <iostream>
void ServerMovementTask::execute() {
	std::cout << "ServerMovementTask is being executed" << std::endl;

	Object task_report;
	task_report["result"] = std::shared_ptr<IData>(new String("success"));

	JsonSerializer serializer;
	auto task_config_serial = serializer.serialize(task_report);

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