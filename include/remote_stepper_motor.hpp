#ifndef	REMOTE_STEPPER_MOTOR_HPP
#define	REMOTE_STEPPER_MOTOR_HPP

#include <map>
#include <memory>
#include <vector>

#include "array.hpp"
#include "data.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "task_executor.hpp"
#include "stepper_motor_tasks_factory.hpp"

namespace cnc {
	class RemoteStepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class Shoulder: int { IN0, IN1, IN2, IN3 };
		using GpoId = int;
		using StepperId = int;
		using Shoulders = std::map<Shoulder, GpoId>;
		using State = std::map<Shoulder, mcu_platform::Gpo::State>;
		using States = std::vector<State>;
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;

		RemoteStepperMotor(const StepperId& id, const Shoulders& shoulders, const States& states, const Executor& executor);
		RemoteStepperMotor(const RemoteStepperMotor& other) = delete;
		RemoteStepperMotor& operator=(const RemoteStepperMotor& other) = delete;
		virtual ~RemoteStepperMotor() noexcept;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms);
	private:
		using TaskType = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpoId>::TaskType;
		const StepperId m_id;
		const std::unique_ptr<Executor> m_executor;
		static std::string shoulder_to_str(const Shoulder& shoulder) {
			const std::string prefix("IN");
			return prefix + std::to_string(static_cast<int>(shoulder));
		}
	};

	inline RemoteStepperMotor::RemoteStepperMotor(const StepperId& id, const Shoulders& shoulders, const States& states, const Executor& executor): m_id(id), m_executor(executor.clone()) {
		using namespace mcu_server;
		Object shoulders_data;
		for (auto [shoulder, gpio_id]: shoulders) {
			shoulders_data.add(shoulder_to_str(shoulder), Integer(static_cast<GpoId>(gpio_id)));
		}
		Array states_data;
		for (auto state: states) {
			Object shoulders_states;
			for (auto [shoulder, gpio_state]: state) {
				shoulders_states.add(shoulder_to_str(shoulder), Integer(static_cast<int>(gpio_state)));
			}
			states_data.push_back(shoulders_states);
		}
		Object create_data;
		create_data.add("task_type", Integer(static_cast<int>(TaskType::CREATE_STEPPER_MOTOR)));
		create_data.add("shoulders", shoulders_data);
		create_data.add("states", states_data);
		create_data.add("stepper_id", Integer(m_id));
		m_executor->execute(create_data);
	}

	inline RemoteStepperMotor::~RemoteStepperMotor() noexcept {
		using namespace mcu_server;
		Object delete_data;
		delete_data.add("task_type", Integer(static_cast<int>(TaskType::DELETE_STEPPER_MOTOR)));
		delete_data.add("stepper_id", Integer(m_id));
		m_executor->execute(delete_data);
	}

	inline void RemoteStepperMotor::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) {
		using namespace mcu_server;
		Object steps_data;
		steps_data.add("task_type", Integer(static_cast<int>(TaskType::STEPS)));
		steps_data.add("stepper_id", Integer(m_id));
		steps_data.add("direction", Integer(static_cast<int>(direction)));
		steps_data.add("steps_number", Integer(steps_num));
		steps_data.add("step_duration_ms", Integer(step_duration_ms));
		m_executor->execute(steps_data);
	}
}

#endif // REMOTE_STEPPER_MOTOR_HPP