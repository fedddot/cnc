#include <string>
#include <memory>
#include <iostream>

#include "object.hpp"
#include "string.hpp"
#include "json_serializer.hpp"

#include "movement_task.hpp"

using namespace task;
using namespace data;
using namespace json;

MovementTask::MovementTask(const Distance& distance, const Speed& speed, Axis axis): m_distance(distance), m_speed(speed), m_axis(axis) {

}



void MovementTask::execute() {
	std::string distance = std::to_string(m_distance);
	Object task_config_obj;
	task_config_obj["dist"] = std::shared_ptr<IData>(new String(std::to_string(m_distance)));
	task_config_obj["speed"] = std::shared_ptr<IData>(new String(std::to_string(m_speed)));
	task_config_obj["axis"] = std::shared_ptr<IData>(new String(std::to_string(static_cast<int>(m_axis))));

	JsonSerializer serializer;
	std::string json_config_obj = serializer.serialize(task_config_obj);
	std::cout << json_config_obj << std::endl;

	// TODO: Implement communication protocol and sent this config obj to the server
}