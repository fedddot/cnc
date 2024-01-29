#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task_factory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "task.hpp"

using namespace task_factory;
using namespace inventory;
using namespace data;
using namespace basics;
using namespace tasks;

using TestInventoryTaskFactory = InventoryTaskFactory<int, std::string>;
using TaskType = TestInventoryTaskFactory::TaskType;

static TaskType retrieve_type(const Data& data) {
	const Object& data_object = Data::cast<Object>(data);
	auto int_type = Data::cast<Integer>(data_object.access("task_type")).get();
	switch (int_type) {
	case static_cast<int>(TaskType::CREATE_INVENTORY_ITEM):
		return TaskType::CREATE_INVENTORY_ITEM;
	case static_cast<int>(TaskType::DELETE_INVENTORY_ITEM):
		return TaskType::DELETE_INVENTORY_ITEM;
	case static_cast<int>(TaskType::USE_INVENTORY_ITEM):
		return TaskType::USE_INVENTORY_ITEM;
	default:
		throw std::invalid_argument("failed to retrieve task type");
	}
}

TEST(inventory_task_factory, ctor_dtor_sanity) {
	// GIVEN
	Inventory<int, std::string> inventory;

	// WHEN
	TestInventoryTaskFactory *instance_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TestInventoryTaskFactory(&inventory, retrieve_type));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(inventory_task_factory, set_create_sanity) {
	// GIVEN
	Inventory<int, std::string> inventory;
	TestInventoryTaskFactory instance(&inventory, retrieve_type);

	// THEN
	ASSERT_NO_THROW(
		instance.set_creator(
			TaskType::CREATE_INVENTORY_ITEM,
			[](Inventory<int, std::string> *inv, const Data& cfg)-> Task * {
				return nullptr;
			}
		)
	);
}