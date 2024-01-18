#include "gtest/gtest.h"
#include <iostream>
#include <map>
#include <memory>

#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "report.hpp"
#include "task.hpp"

using namespace basics;
using namespace cnc_engine;

template <class T>
const T& castData(const Data& data) {
	return dynamic_cast<const T&>(data);
}

class TestTask: public Task<Report> {
private:
	std::unique_ptr<Data> m_report_data;
public:
	TestTask(const Data& cfg): m_report_data(cfg.copy()) {
	}

	virtual Report execute() override {
		Integer result(castData<Integer>(*m_report_data));
		Report::Result report_result = (result.get() == 0) ? Report::Result::SUCCESS : Report::Result::FAILURE;
		return Report(report_result, *m_report_data);
	}
};

class TestFactory: public Engine::TaskFactory {
public:
	virtual Task<Report> *create(const Data& cfg) const override {
		return new TestTask(cfg);
	}
};

class TestSender: public Engine::ReportSender {
private:
	Report::Result m_expected_result;
public:
	TestSender(const Report::Result& expected_result): m_expected_result((expected_result)) {

	}

	virtual void send(const Report& data) const override {
		std::cout << "received report with result = ";
		switch (data.result()) {
		case Report::Result::FAILURE:
			std::cout << "FAILURE";
			break;
		case Report::Result::SUCCESS:
			std::cout << "SUCCESS";
			break;
		}
		std::cout << std::endl;
		ASSERT_EQ(m_expected_result, data.result());
	}
};

TEST(ut_engine, sanity) {
	// GIVEN
	TestFactory factory;
	const std::map<Report::Result, Integer> test_cases {
		{Report::Result::SUCCESS, Integer(0)},
		{Report::Result::FAILURE, Integer(-1)}
	};

	for (auto test_case: test_cases) {
		// WHEN
		TestSender sender(test_case.first);
		std::unique_ptr<Engine> engine_ptr(nullptr);

		// THEN
		ASSERT_NO_THROW(engine_ptr = std::unique_ptr<Engine>(new Engine(factory, sender)));
		ASSERT_NO_THROW(engine_ptr->run_task(test_case.second));
	}
}