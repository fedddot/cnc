#include "gtest/gtest.h"

#include "stepper_motor.hpp"

using namespace cnc;
using GpioId = int;

TEST(ut_stepper_motor, ctor_dtor_sanity) {
	// GIVEN

	// WHEN
	StepperMotor<GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor<GpioId>(
				...
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}