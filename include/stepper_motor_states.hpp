#ifndef	STEPPER_MOTOR_STATES_HPP
#define	STEPPER_MOTOR_STATES_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "stepper_motor_state.hpp"

namespace cnc {

	class StepperMotorStates {
	public:
		StepperMotorStates(const std::vector<StepperMotorState>& states);
		StepperMotorStates(const StepperMotorStates& other) = default;
		StepperMotorStates& operator=(const StepperMotorStates& other) = default;
		virtual ~StepperMotorStates() noexcept = default;

		void move_back();
		void move_forward();
		StepperMotorState current() const;
	private:
		std::vector<StepperMotorState> m_states;
		std::size_t m_current_index;
	};
	
	inline StepperMotorStates::StepperMotorStates(const std::vector<StepperMotorState>& states): m_states(states), m_current_index(0UL) {
		if (1 > m_states.size()) {
			throw std::invalid_argument("states must contain at list 1 state");
		}
	}

	inline void StepperMotorStates::move_back() {
		if (0 == m_current_index) {
			m_current_index = m_states.size() - 1;
			return;
		}
		--m_current_index;
	}
	
	inline void StepperMotorStates::move_forward() {
		if (m_states.size() - 1 == m_current_index) {
			m_current_index = 0;
			return;
		}
		++m_current_index;
	}
	
	inline StepperMotorState StepperMotorStates::current() const {
		return m_states[m_current_index];
	}
}

#endif // STEPPER_MOTOR_STATES_HPP