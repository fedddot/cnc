#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "integer.hpp"
#include "string.hpp"
#include "task_factory.hpp"
#include "object.hpp"
#include "task.hpp"

using namespace task_factory;
using namespace data;
using namespace engine;

static inline std::string retrieve_type(const Data& data) {
	return Data::cast<String>(Data::cast<Object>(data).access("task_type")).get();
}

TEST(ut_task_factory, ctor_dtor_sanity) {
	// WHEN
	TaskFactory *instance_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TaskFactory(retrieve_type));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_task_factory, set_creator_remove_creator_sanity) {
	// GIVEN
	const std::map<std::string, TaskFactory::TaskCreator> test_cases {
		{"ctor1", [&](const Data& cfg)-> Task * {}},
		{"ctor2", [&](const Data& cfg)-> Task * {}},
		{"ctor3", [&](const Data& cfg)-> Task * {}}
	};

	// WHEN
	TaskFactory factory(retrieve_type);

	// THEN
	for (auto test_case: test_cases) {
		ASSERT_NO_THROW(factory.set_creator(test_case.first, test_case.second));
	}
	for (auto test_case: test_cases) {
		ASSERT_NO_THROW(factory.remove_creator(test_case.first));
	}
}

TEST(ut_task_factory, create_sanity) {
	// GIVEN
	const int expected_reported_val = 18;

	class TestTask: public Task {
		virtual void execute() override {
			m_report.add("result", Integer(expected_reported_val));
		}
		virtual const data::Data& report() const override {
			return m_report;
		}
	private:
		Object m_report;
	};

	const std::string ctor_id("test_ctor");
	auto test_ctor = [&](const Data& cfg)-> Task * {
		return new TestTask;
	};

	// WHEN
	TaskFactory factory(retrieve_type);
	factory.set_creator(ctor_id, test_ctor);
	Object cfg;
	cfg.add("task_type", String(ctor_id));
	std::unique_ptr<Task> task_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(factory.create(cfg)));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(task_ptr->execute());
	ASSERT_EQ(expected_reported_val, Data::cast<Integer>(Data::cast<Object>(task_ptr->report()).access("result")).get());
}