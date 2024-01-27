#include "gtest/gtest.h"

#include <memory>
#include <stdexcept>
#include <string>
#include "inventory.hpp"

using namespace inventory;

TEST(ut_inventory, sanity) {
	// WHEN
	Inventory<int, std::string> instance;
	std::string *item_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance.put(0, std::shared_ptr<std::string>(new std::string("0"))));
	ASSERT_THROW(instance.put(0, std::shared_ptr<std::string>(new std::string("1"))), std::invalid_argument);
	ASSERT_TRUE(instance.contains(0));
	ASSERT_NO_THROW(item_ptr = instance.get(0));
	ASSERT_NE(nullptr, item_ptr);
	ASSERT_THROW(instance.get(0), std::invalid_argument);
	ASSERT_THROW(instance.put(1, nullptr), std::invalid_argument);

	// CLEANUP
	delete item_ptr;
}