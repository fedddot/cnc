#include "gtest/gtest.h"
#include <functional>
#include <map>
#include <memory>

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "task.hpp"
#include "task_factory.hpp"

using namespace task_factory;
using namespace data;
using namespace basics;

class TestTask: public Task {
public:
	TestTask(const Data& cfg): m_cfg(cfg.copy()) {}
	virtual void execute() override {
		
	}
private:
	std::unique_ptr<Data> m_cfg;
};

TEST(ut_engine, sanity) {
	// GIVEN
	auto creator = [&](const Data& data)-> Task * {
		return new TestTask(data);
	};
	Object test_cfg;
	test_cfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::CREATE_INVENTORY_ITEM)));

	// WHEN
	TaskFactory *instance_ptr = nullptr;
	Task *task_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TaskFactory());
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(instance_ptr->set_creator(TaskFactory::TaskType::CREATE_INVENTORY_ITEM, creator));
	ASSERT_NO_THROW(task_ptr = instance_ptr->create(test_cfg));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(task_ptr->execute());
}