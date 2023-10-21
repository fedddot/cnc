#include <stdexcept>
#include <string>
#include <memory>

#include "gpio.hpp"
#include "input_gpio.hpp"
#include "output_gpio.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "string.hpp"
#include "server_task.hpp"

#include "steps_task.hpp"

using namespace task;
using namespace data;
using namespace hardware;

RegisterGpioTask::RegisterGpioTask(const std::string& id, const PinNumber& pin_number, const Direction& direction, GpioRegistry& registry): ServerTask(id), m_pin_number(pin_number), m_direction(direction), m_registry(registry) {

}

void RegisterGpioTask::execute() {
	if (m_registry.is_registered(m_pin_number)) {
		m_report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
		m_report.insert({"what", std::shared_ptr<IData>(new String("already registered"))});
		return;
	}
	std::shared_ptr<Gpio> gpio(nullptr);
	switch (m_direction)
	{
	case Direction::IN:
		gpio = std::shared_ptr<Gpio>(new InputGpio(m_pin_number));
		break;
	case Direction::OUT:
		gpio = std::shared_ptr<Gpio>(new OutputGpio(m_pin_number));
		break;
	}
	m_registry.register_member(m_pin_number, gpio);
	m_report.insert({"result", std::shared_ptr<IData>(new String("OK"))});
}

data::Object RegisterGpioTask::report() const {
	return m_report;
}