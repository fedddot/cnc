#include "gtest/gtest.h"

#include "json_data_serializer.hpp"

#include "task_data_generator.hpp"

using namespace cnc;
using namespace mcu_server_utl;

TEST(ut_task_data_generator, ctor_dtor_sanity) {
	// GIVEN
	const std::string task_type_field("task_type");
	const std::string gpio_id_field("gpio_id");
	const std::string gpio_dir_field("gpio_dir");
	const std::string gpio_state_field("gpio_state");
	const std::string delay_field("delay_ms");

	// WHEN
	TaskDataGenerator *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TaskDataGenerator(
				JsonDataSerializer(),
				task_type_field,
				gpio_id_field,
				gpio_dir_field,
				gpio_state_field,
				delay_field
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}