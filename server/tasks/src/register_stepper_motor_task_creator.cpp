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

RegisterStepperMotorTaskCreator::RegisterStepperMotorTaskCreator(StepperMotorRegistry& stepper_motors_registry, GpioRegistry& gpios_registry, const ControlGpiosFieldNamesMapping& gpios_field_names_mapping, const std::string& id_field, const std::string& motor_id_field, const std::string& control_gpios_field): m_stepper_motors_registry(stepper_motors_registry), m_gpios_registry(gpios_registry), m_gpios_field_names_mapping(gpios_field_names_mapping), m_id_field(id_field), m_motor_id_field(motor_id_field), m_control_gpios_field(control_gpios_field) {

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

StepperMotor::ControlGpios RegisterStepperMotorTaskCreator::init_control_gpios(const Object& config_data) {
	StepperMotor::ControlGpios result;
	Object control_gpios_mapping = getField<Object>(config_data, m_control_gpios_field);
	std::for_each(
		m_gpios_field_names_mapping.begin(),
		m_gpios_field_names_mapping.end(),
		[&](const auto& item) {
			const StepperMotor::BridgeShoulder shoulder(item.first);
			const std::string field_name(item.second);

			String gpio_number_str(getField<String>(control_gpios_mapping, field_name));
			int gpio_number = std::atoi(gpio_number_str.c_str());
			Gpio *gpio_ptr = m_gpios_registry.access_member(gpio_number).get();
			if (Gpio::Direction::OUT != gpio_ptr->get_direction()) {
				throw std::runtime_error("wrong gpio direction for control " + field_name);
			}
			OutputGpio *output_gpio_ptr = dynamic_cast<OutputGpio *>(gpio_ptr);
			result.insert({shoulder, output_gpio_ptr});
		}
	);
	return result;
}

std::shared_ptr<ServerTask> RegisterStepperMotorTaskCreator::create(const Object& config_data) {
	String task_id = getField<String>(config_data, m_id_field);
	String motor_id = getField<String>(config_data, m_motor_id_field);
	
	StepperMotor::ControlGpios control_gpios(init_control_gpios(config_data));

	return std::shared_ptr<ServerTask>(new RegisterStepperMotorTask(task_id, motor_id, control_gpios, m_stepper_motors_registry));
}