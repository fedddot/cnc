#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "data.hpp"
#include "engine.hpp"
#include "factory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "sender.hpp"
#include "task.hpp"

using namespace engine;
using namespace data;

class TestTask: public Task {
public:
	TestTask(const Data& cfg): m_report_data(Data::cast<Object>(cfg)) {
	}
	virtual void execute() override {
		if (Data::cast<Integer>(m_report_data.access("result")).get()) {
			throw std::runtime_error("failed to perform the task!");
		}
	}

	virtual const data::Data& report() const override {
		return m_report_data;
	}
private:
	Object m_report_data;
};

class TestFactory: public Factory {
public:
	virtual Task *create(const Data& cfg) const override {
		return new TestTask(cfg);
	}
};

class TestSender: public Sender {
public:
	TestSender(const Integer& expected_result): m_expected_result(expected_result) {

	}

	virtual void send(const Data& data) const override {
		std::cout << "received report with result = ";
		Integer received_result(Data::cast<Integer>(Data::cast<Object>(data).access("result")));
		switch (received_result.get()) {
		case 0:
			std::cout << "SUCCESS";
			break;
		default:
			std::cout << "FAILURE";
		}
		std::cout << std::endl;
		ASSERT_EQ(m_expected_result.get(), received_result.get());
	}
private:
	Integer m_expected_result;
};

TEST(ut_engine, sanity) {
	// GIVEN
	TestFactory factory;
	const std::vector<Integer> test_cases {Integer(0), Integer(-1)};

	for (auto test_case: test_cases) {
		// WHEN
		TestSender sender(test_case);
		TestFactory factory;
		std::unique_ptr<Engine> engine_ptr(nullptr);

		// THEN
		ASSERT_NO_THROW(engine_ptr = std::unique_ptr<Engine>(new Engine(factory, sender)));
		Object task_cfg;
		task_cfg.add("result", test_case);
		if (test_case.get()) {
			ASSERT_ANY_THROW(engine_ptr->run_task(task_cfg));
		} else {
			ASSERT_NO_THROW(engine_ptr->run_task(task_cfg));
		}
	}
}