#include <stdexcept>
#include <string>

#include "pico/stdlib.h"

#include "stepper_motor.hpp"
#include "string.hpp"
#include "object.hpp"
#include "server_task.hpp"

#include "steps_task.hpp"

using namespace task;
using namespace data;
using namespace hardware;

StepsTask::StepsTask(const std::string& id, StepperMotor& stepper_motor, const Direction& direction, unsigned int number_of_steps, unsigned int step_duration_ms): ServerTask(id), m_stepper_motor(stepper_motor), m_direction(direction), m_number_of_steps(number_of_steps), m_step_duration_ms(step_duration_ms) {

}

void StepsTask::execute() {
	try {
		if (!m_stepper_motor.is_enabled()) {
			m_stepper_motor.enable();
		}
		unsigned int steps = m_number_of_steps;
		while (steps) {
			m_stepper_motor.step(m_direction);
			sleep_ms(m_step_duration_ms);
			--steps;
		}
		m_stepper_motor.disable();
		m_report.insert({"result", std::shared_ptr<IData>(new String("OK"))});
	} catch (const std::exception& e) {
		m_report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
		m_report.insert({"what", std::shared_ptr<IData>(new String(e.what()))});
	}
}

data::Object StepsTask::report() const {
	return m_report;
}