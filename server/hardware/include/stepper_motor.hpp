#ifndef	__STEPPER_MOTOR_HPP__
#define	__STEPPER_MOTOR_HPP__

#include <array>
#include <string>
#include <algorithm>
#include <cstddef>

#include "output_gpio.hpp"

namespace hardware {
	class StepperMotor {
	public:
		enum class Direction: int {
			CW = 0,
			CCW = 1
		};

		enum class Outputs: int {
			A_LEFT,		// bridge A, left shoulder
			A_RIGHT,	// bridge A, right shoulder
			B_LEFT,		// bridge B, left shoulder
			B_RIGHT,	// bridge B, right shoulder
		};
		StepperMotor(OutputGpio& out1, OutputGpio& out2, OutputGpio& out3, OutputGpio& out4);
		~StepperMotor() noexcept;

		void enable() override;
		void disable() override;
		bool is_enabled() const override;
		virtual std::string& get_id() const override;
		
		void step(Direction direction);
	private:
		const std::string m_id;
		const Outputs m_outputs;
		const States m_states;
		bool m_is_enabled;
		States::const_iterator m_current_state_iter;

		void apply_state(const State& state);
		States::const_iterator next_state_iter(Direction direction);
	}; // StepperMotor

	template <std::size_t Moutputs, std::size_t Nstates>
	StepperMotor<Moutputs, Nstates>::StepperMotor(const std::string& id, const Outputs& outputs, const States& states): m_id(id), m_outputs(outputs), m_states(states), m_is_enabled(false), m_current_state_iter(m_states.begin()) {
		disable();
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	StepperMotor<Moutputs, Nstates>::~StepperMotor() noexcept {
		disable();
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	void StepperMotor<Moutputs, Nstates>::enable() {
		m_current_state_iter = m_states.begin();
		apply_state(*m_current_state_iter);
		m_is_enabled = true;
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	void StepperMotor<Moutputs, Nstates>::disable() {
		const State disable_state {Gpio::Value::LOW};
		apply_state(disable_state);
		m_is_enabled = false;
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	void StepperMotor<Moutputs, Nstates>::step(Direction direction) {
		if (!is_enabled()) {
			throw std::runtime_error("the motor is disabled");
		}
		m_current_state_iter = next_state_iter(direction);
		apply_state(*m_current_state_iter);
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	void StepperMotor<Moutputs, Nstates>::apply_state(const State& state) {
		for (std::size_t i = 0; Moutputs > i; ++i) {
			m_outputs[i].write_value(state[i])
		}
	}

	template <std::size_t Moutputs, std::size_t Nstates>
	StepperMotor<Moutputs, Nstates>::States::const_iterator StepperMotor<Moutputs, Nstates>::next_state_iter(const States::const_iterator& current_state_iter, Direction direction) {
		auto proceed_state_iter = [&](const States::const_iterator& current_state_iter) {
			if (m_states.end() == current_state_iter) {
				throw std::invalid_argument("invalid iter received");
			}
			if (m_states.end() == current_state_iter + 1) {
				return m_states.begin();
			}
			return current_state_iter + 1;
		};

		auto preceed_state_iter = [&](const States::const_iterator& current_state_iter) {
			if (m_states.end() == current_state_iter) {
				throw std::invalid_argument("invalid iter received");
			}
			if (m_states.begin() == current_state_iter) {
				return m_states.begin() + m_states.size() - 1;
			}
			return std::prev(current_state_iter, 1);
		};

		switch (direction)
		{
		case Direction::CW:
			return proceed_state_iter(current_state_iter);
		case Direction::CCW:
			return preceed_state_iter(current_state_iter);
		default:
			break;
		}
		throw std::invalid_argument("received direction is not supported");
	}


} // namespace hardware
#endif // __STEPPER_MOTOR_HPP__