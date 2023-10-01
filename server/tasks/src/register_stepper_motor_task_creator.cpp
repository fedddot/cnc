#include <stdexcept>
#include <algorithm>
#include <string>
#include <map>
#include <memory>

#include "object.hpp"
#include "string.hpp"
#include "server_task.hpp"
#include "registry.hpp"
#include "gpio.hpp"
#include "stepper_motor.hpp"
#include "register_stepper_motor_task.hpp"
#include "icreator.hpp"

#include "register_stepper_motor_task_creator.hpp"

using namespace task;
using namespace data;
using namespace common;
using namespace hardware;

RegisterStepperMotorTaskCreator::RegisterStepperMotorTaskCreator(StepperMotorRegistry& stepper_motors_registry, GpioRegistry& gpios_registry, const std::string& id_field = "id", const std::string& motor_id_field = "motor_id", const std::string& control_gpios_field = "control_gpios"): m_stepper_motors_registry(stepper_motors_registry), m_gpios_registry(gpios_registry), m_id_field(id_field), m_motor_id_field(motor_id_field), m_control_gpios_field(control_gpios_field) {

}

template <class T>
static T getField(const Object& data_object, const std::string& field_name) {
	auto iter = data_object.find(field_name);
	if (data_object.end() == iter) {
		throw std::invalid_argument("received field not found");
	}
	if (nullptr == iter->second) {
		throw std::invalid_argument("nullptr is stored under specified field");
	}
	return T(*(iter->second));
}

static StepperMotor::ControlGpios init_gpios(const Object& config_data, RegisterStepperMotorTaskCreator::GpioRegistry& gpios_registry) {
	const std::map<StepperMotor::BridgeShoulder, std::string> shoulders_mapping {
		{StepperMotor::BridgeShoulder::A_LEFT,		"a_left_pin_number"},
		{StepperMotor::BridgeShoulder::A_RIGHT,		"a_right_pin_number"},
		{StepperMotor::BridgeShoulder::B_LEFT,		"b_left_pin_number"},
		{StepperMotor::BridgeShoulder::B_RIGHT,		"b_right_pin_number"},
	};
	StepperMotor::ControlGpios result;
	std::for_each(
		shoulders_mapping.begin(),
		shoulders_mapping.end(),
		[&](const auto& mapping) {
			String pin_number_str = getField<String>(config_data, mapping.second);
			Gpio::PinNumber pin_number = static_cast<Gpio::PinNumber>(std::atoi(pin_number_str.c_str()));
			if (!gpios_registry.is_registered(pin_number)) {
				throw std::runtime_error("required pin (" + mapping.second + ") was not registered");
			}
			OutputGpio *output_gpio_ptr = dynamic_cast<OutputGpio *>(gpios_registry.access_member(pin_number).get());
			if (nullptr == output_gpio_ptr) {
				throw std::runtime_error("required pin (" + mapping.second + ") has incorrect configuration");
			}
			result[mapping.field] = output_gpio_ptr;
		}
	);
	return result;
}

std::shared_ptr<ServerTask> RegisterStepperMotorTaskCreator::create(const Object& config_data) {
	String task_id = getField<String>(config_data, m_id_field);
	String motor_id = getField<String>(config_data, m_motor_id_field);
	Object control_gpios_config_data = getField<Object>(config_data, m_control_gpios_field);
	
	StepperMotor::ControlGpios control_gpios = init_gpios(control_gpios_config_data, m_gpios_registry);

	return std::shared_ptr<ServerTask>(new RegisterStepperMotorTask(task_id, motor_id, control_gpios, m_stepper_motors_registry));
}