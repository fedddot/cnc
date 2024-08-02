#include "gtest/gtest.h"
#include <memory>
#include <string>

#include "data.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"

#include "mcu_factory.hpp"
#include "object.hpp"
#include "task_data_generator.hpp"

using namespace cnc;
using namespace mcu_server;
using namespace mcu_server_utl;

using TaskType = typename mcu_factory::McuFactory<int>::TaskType;

TEST(ut_task_data_generator, ctor_dtor_sanity) {
	// GIVEN
	const std::string task_type_field("task_type");
	const std::string gpio_id_field("gpio_id");
	const std::string gpio_dir_field("gpio_dir");
	const std::string gpio_state_field("gpio_state");
	const std::string delay_field("delay_ms");
	const std::string tasks_field("tasks");

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
				delay_field,
				tasks_field
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

void assert_result_contains_int(const std::string& result, const std::string& field_name, const int value) {
	std::unique_ptr<Data> parsed_result(JsonDataParser().parse(result));
	ASSERT_EQ(Data::Type::OBJECT, parsed_result->type());
	auto& result_object = Data::cast<Object>(*parsed_result);
	ASSERT_TRUE(result_object.contains(field_name));
	ASSERT_EQ(value, Data::cast<Integer>(result_object.access(field_name)).get());
}

TEST(ut_task_data_generator, generate_sanity) {
	// GIVEN
	const std::string task_type_field("task_type");
	const std::string gpio_id_field("gpio_id");
	const std::string gpio_dir_field("gpio_dir");
	const std::string gpio_state_field("gpio_state");
	const std::string delay_field("delay_ms");
	const std::string tasks_field("tasks");

	// WHEN
	TaskDataGenerator instance(
		JsonDataSerializer(),
		task_type_field,
		gpio_id_field,
		gpio_dir_field,
		gpio_state_field,
		delay_field,
		tasks_field
	);
	TaskDataGenerator::TaskData result("");

	// THEN
	ASSERT_NO_THROW(result = instance.generate_create_gpio_data(10, TaskDataGenerator::GpioDirection::OUT));
	assert_result_contains_int(result, task_type_field, static_cast<int>(TaskType::CREATE_GPIO));
	assert_result_contains_int(result, gpio_id_field, 10);
	assert_result_contains_int(result, gpio_dir_field, static_cast<int>(TaskDataGenerator::GpioDirection::OUT));

	ASSERT_NO_THROW(result = instance.generate_delete_gpio_data(10));
	assert_result_contains_int(result, task_type_field, static_cast<int>(TaskType::DELETE_GPIO));
	assert_result_contains_int(result, gpio_id_field, 10);

	ASSERT_NO_THROW(result = instance.generate_set_gpio_data(10, TaskDataGenerator::GpioState::HIGH));
	assert_result_contains_int(result, task_type_field, static_cast<int>(TaskType::SET_GPIO));
	assert_result_contains_int(result, gpio_id_field, 10);
	assert_result_contains_int(result, gpio_state_field, static_cast<int>(TaskDataGenerator::GpioState::HIGH));

	ASSERT_NO_THROW(result = instance.generate_delay_data(1304));
	assert_result_contains_int(result, task_type_field, static_cast<int>(TaskType::DELAY));
	assert_result_contains_int(result, delay_field, 1304);
}