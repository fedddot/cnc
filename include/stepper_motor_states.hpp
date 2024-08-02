#ifndef	STEPPER_MOTOR_STATES_HPP
#define	STEPPER_MOTOR_STATES_HPP

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
		
		using StateIter = typename std::vector<StepperMotorState>::iterator;
		StateIter m_current_iter;
	};
	
	inline StepperMotorStates::StepperMotorStates(const std::vector<StepperMotorState>& states): m_states(states) {
		if (1 > m_states.size()) {
			throw std::invalid_argument("states must contain at list 1 state");
		}
		m_current_iter = m_states.begin();
	}

	inline void StepperMotorStates::move_back() {
		if (m_states.begin() == m_current_iter) {
			m_current_iter = m_states.begin() + m_states.size() - 1;
			return;
		}
		m_current_iter = m_current_iter - 1;
	}
	
	inline void StepperMotorStates::move_forward() {
		if (m_states.end() == m_current_iter + 1) {
			m_current_iter = m_states.begin();
			return;
		}
		m_current_iter = m_current_iter + 1;
	}
	
	inline StepperMotorState StepperMotorStates::current() const {
		return *m_current_iter;
	}
}

#endif // STEPPER_MOTOR_STATES_HPP