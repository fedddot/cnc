#include <stdexcept>
#include <string>
#include <memory>

#include "steps_task.hpp"
#include "stepper_motor.hpp"
#include "string.hpp"
#include "object.hpp"
#include "server_task.hpp"
#include "registry.hpp"
#include "icreator.hpp"

#include "steps_task_creator.hpp"

using namespace task;
using namespace data;
using namespace common;
using namespace hardware;

StepsTaskCreator::StepsTaskCreator(StepperMotorRegistry& stepper_motors_registry, const std::string& id_field, const std::string& motor_id_field, const std::string& direction_field, const std::string& steps_number_field, const std::string& step_duration_ms_field): m_stepper_motors_registry(stepper_motors_registry), m_id_field(id_field), m_motor_id_field(motor_id_field), m_direction_field(direction_field), m_steps_number_field(steps_number_field), m_step_duration_ms_field(step_duration_ms_field) {

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

static StepperMotor::Direction strToDirection(const String& str) {
	if ("CW" == str) {
		return StepperMotor::Direction::CW;
	}
	if ("CCW" == str) {
		return StepperMotor::Direction::CCW;
	}
	throw std::invalid_argument("unsupported direction string received (\"" + str + "\")");
}

std::shared_ptr<ServerTask> StepsTaskCreator::create(const Object& config_data) {
	String task_id(getField<String>(config_data, m_id_field));
	String motor_id(getField<String>(config_data, m_motor_id_field));

	StepperMotor& stepper_motor = *(m_stepper_motors_registry.access_member(motor_id));

	String direction_str(getField<String>(config_data, m_direction_field));
	String steps_number_str(getField<String>(config_data, m_steps_number_field));
	String step_duration_ms_str(getField<String>(config_data, m_step_duration_ms_field));


	return std::shared_ptr<ServerTask>(new StepsTask(task_id, stepper_motor, strToDirection(direction_str), std::atoi(steps_number_str.c_str()), std::atoi(step_duration_ms_str.c_str())));
}