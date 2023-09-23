#include <stdexcept>
#include <string>
#include <memory>

#include "string.hpp"
#include "object.hpp"
#include "server_task.hpp"
#include "register_gpio_task.hpp"
#include "icreator.hpp"

#include "register_gpio_task_creator.hpp"

using namespace task;
using namespace data;
using namespace common;

RegisterGpioTaskCreator::RegisterGpioTaskCreator(RegisterGpioTask::GpioRegistry& registry, const std::string& id_field, const std::string& pin_number_field, const std::string& direction_field): m_registry(registry), m_id_field(id_field), m_pin_number_field(pin_number_field), m_direction_field(direction_field) {

}

template <class T>
T getField(const Object& data_object, const std::string& field_name) {
	auto iter = data_object.find(field_name);
	if (data_object.end() == iter) {
		throw std::invalid_argument("received field not found");
	}
	if (nullptr == iter->second) {
		throw std::invalid_argument("nullptr is stored under specified field");
	}
	return T(*(iter->second));
}

RegisterGpioTask::Direction getDirection(const String& str_data) {
	if ("IN" == str_data) {
		return RegisterGpioTask::Direction::IN;
	}
	if ("OUT" == str_data) {
		return RegisterGpioTask::Direction::OUT;
	}
	throw std::invalid_argument("failed to cast direction from str data");
}

RegisterGpioTask::PinNumber getPinNumber(const String& str_data) {
	return static_cast<RegisterGpioTask::PinNumber>(std::atoi(str_data.c_str()));
}

std::shared_ptr<ServerTask> RegisterGpioTaskCreator::create(const Object& config_data) {
	String task_id = getField<String>(config_data, m_id_field);
	String pin_number_str = getField<String>(config_data, m_pin_number_field);
	String direction_str = getField<String>(config_data, m_direction_field);

	return std::shared_ptr<ServerTask>(new RegisterGpioTask(task_id, getPinNumber(pin_number_str), getDirection(direction_str), m_registry));
}