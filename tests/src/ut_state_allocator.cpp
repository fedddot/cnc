#include <memory>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpo_allocator.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "state_allocator.hpp"
#include "stepper_motor_types.hpp"

#include "cnc_fixture.hpp"
#include "test_gpo.hpp"

using namespace cnc;
using namespace cnc_uts;
using namespace mcu_server_utl;
using namespace mcu_server;

static void report_assertion(const Data& report) {
	const auto& report_obj = Data::cast<Object>(report);
	ASSERT_TRUE(report_obj.contains("result"));
	ASSERT_EQ(0, Data::cast<Integer>(report_obj.access("result")).get());
}

TEST_F(CncFixture, sanity) {
	// GIVEN
	const int task_id(12);
	const Shoulders shoulders {
		{Shoulder::IN1, 4},
		{Shoulder::IN2, 5},
		{Shoulder::IN3, 6},
		{Shoulder::IN4, 7}
	};
	const MotorState state {
		{Shoulder::IN1, GpioState::HIGH},
		{Shoulder::IN2, GpioState::LOW},
		{Shoulder::IN3, GpioState::HIGH},
		{Shoulder::IN4, GpioState::LOW}
	};
	StateAllocator allocator(
		task_id,
		shoulders,
		state
	);

	std::unique_ptr<Data> report(nullptr);
	for (auto [shoulder, gpio_id]: shoulders) {
		GpoAllocator allocator(gpio_id);
		std::unique_ptr<Data> report(run_data(allocator.create_data()));
	}

	ASSERT_NO_THROW(report = std::unique_ptr<Data>(run_data(allocator.create_data())));
	report_assertion(*report);
	
	ASSERT_NO_THROW(report = std::unique_ptr<Data>(run_data(execute_persistent_task_data(task_id))));
	report_assertion(*report);
	
	ASSERT_NO_THROW(report = std::unique_ptr<Data>(run_data(allocator.delete_data())));
	report_assertion(*report);

	for (auto [shoulder, gpio_id]: shoulders) {
		GpoAllocator allocator(gpio_id);
		std::unique_ptr<Data> report(run_data(allocator.delete_data()));
	}
}