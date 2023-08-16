#include <stdexcept>
#include <string>
#include <vector>

#include "iserver_task.hpp"
#include "idata.hpp"
#include "string.hpp"
#include "object.hpp"

#include "icreator.hpp"
#include "server_movement_task.hpp"
#include "server_movement_task_creator.hpp"

using namespace task;
using namespace data;
using namespace common;

MovementTaskCreator::MovementTaskCreator(const std::string& distance_field_name, const std::string& speed_field_name, const std::string& axis_field_name): m_distance_field_name(distance_field_name), m_speed_field_name(speed_field_name), m_axis_field_name(axis_field_name) {

}

std::shared_ptr<IServerTask> MovementTaskCreator::create(const IData& config_data) {
	String distance_str(getStringDataField(config_data, m_distance_field_name));
	double distance = std::stod(distance_str);

	String speed_str(getStringDataField(config_data, m_speed_field_name));
	double speed = std::stod(speed_str);

	String axis_str(getStringDataField(config_data, m_axis_field_name));
	MovementTask::Axis axis = static_cast<MovementTask::Axis>(std::stoi(axis_str));

	return std::shared_ptr<IServerTask>(new ServerMovementTask(distance, speed, axis));
}

std::string MovementTaskCreator::getStringDataField(const data::IData& data, const std::string& field_name) {
	const Object& object_const = dynamic_cast<const Object&>(data);
	Object& object = const_cast<Object&>(object_const);
	auto iter = object.find(field_name);
	if (object.end() == iter) {
		throw std::invalid_argument("received data doesn't contain field " + field_name);
	}
	if (nullptr == iter->second) {
		throw std::invalid_argument("field " + field_name + " initialized to nullptr");
	}
	const String& str = dynamic_cast<const String&>(*(iter->second));
	return std::string(str);
}