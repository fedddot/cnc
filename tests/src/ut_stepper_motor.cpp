#include "gtest/gtest.h"

#include "gpio.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_state.hpp"
#include "stepper_motor_states.hpp"

using namespace cnc;
using namespace mcu_platform;

using GpioId = int;
using TestMotor = StepperMotor<GpioId>;
using Shoulder = typename TestMotor::Shoulder;
using GpioState = typename Gpio::State;

TEST(ut_stepper_motor, ctor_dtor_sanity) {
	// GIVEN
	const TestMotor::MotorShoulders shoulders {
		{Shoulder::LL, 4},
		{Shoulder::LR, 5},
		{Shoulder::HL, 6},
		{Shoulder::HR, 7}
	};
	const StepperMotorStates states(
		{
			StepperMotorState(
				{
					{Shoulder::LL, GpioState::HIGH},
					{Shoulder::LR, GpioState::LOW},
					{Shoulder::HL, GpioState::LOW},
					{Shoulder::HR, GpioState::LOW}
				}
			),
			StepperMotorState(
				{
					{Shoulder::LL, GpioState::HIGH},
					{Shoulder::LR, GpioState::LOW},
					{Shoulder::HL, GpioState::HIGH},
					{Shoulder::HR, GpioState::LOW}
				}
			)
		}
	);
	// WHEN
	StepperMotor<GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor<GpioId>(
				
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}