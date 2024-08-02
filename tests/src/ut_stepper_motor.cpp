#include "gtest/gtest.h"

#include "custom_task_executor.hpp"
#include "gpio.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_state.hpp"
#include "stepper_motor_states.hpp"
#include "task_object_generator.hpp"

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
			)
		}
	);
	// WHEN
	StepperMotor<GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor<GpioId>(
				shoulders,
				states,
				cnc_utl::TaskObjectGenerator(),
				cnc_utl::CustomTaskExecutor<void(const TestMotor::TaskData&)>(
					[](const TestMotor::TaskData& data){
						throw std::runtime_error("NOT IMPLEMENTED");
					}
				)
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST(ut_stepper_motor, steps_sanity) {
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
					{Shoulder::HL, GpioState::LOW},
					{Shoulder::HR, GpioState::LOW}
				}
			)
		}
	);

	// WHEN
	StepperMotor<GpioId> instance(
		shoulders,
		states,
		cnc_utl::TaskObjectGenerator(),
		cnc_utl::CustomTaskExecutor<void(const TestMotor::TaskData&)>(
			[](const TestMotor::TaskData& data){
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);

	// THEN
	using Direction = typename StepperMotor<GpioId>::Direction;
	ASSERT_NO_THROW(instance.steps(Direction::CCW, 10, 30));
}