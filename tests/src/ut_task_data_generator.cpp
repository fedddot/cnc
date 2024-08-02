#include "gtest/gtest.h"

#include "task_data_generator.hpp"

using namespace cnc;

TEST(ut_task_data_generator, ctor_dtor_sanity) {
	// WHEN
	TaskDataGenerator *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TaskDataGenerator(
				parser(),
				serializer(),
				factory(),
				fail_report_creator()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}