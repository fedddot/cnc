#include "gtest/gtest.h"

#include "create_inventory_item_task.hpp"
#include "creator.hpp"
#include "inventory.hpp"
#include "report.hpp"

using namespace basics;
using namespace inventory;
using namespace tasks;
using namespace cnc_engine;

class TestItemCreator: public Creator<int *, int> {
public:
	virtual int *create(const int& cfg) const override {
		return new int(cfg);
	}
};

TEST(ut_tasks, sanity) {
	// GIVEN
	TestItemCreator creator;
	Inventory<int, int> test_inventory;

	// WHEN
	CreateInventoryItemTask<int, int, int> instance(
		test_inventory,
		11,
		2,
		creator
	);
	Report *report = nullptr;
	
	// THEN
	ASSERT_NO_THROW(report = new Report(instance.execute()));
	ASSERT_EQ(Report::Result::SUCCESS, report->result());
	ASSERT_TRUE(test_inventory.contains(11));
	int *item = test_inventory.get(11);
	ASSERT_NE(nullptr, item);
	ASSERT_FALSE(test_inventory.contains(11));
	ASSERT_EQ(2, *item);

	// CLEANUP
	delete item;
	delete report;
}