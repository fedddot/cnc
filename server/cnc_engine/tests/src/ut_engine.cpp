#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <vector>

#include "engine.hpp"
#include "task.hpp"

using namespace cnc_engine;

class TestTask: public Task<std::string> {
private:
	const std::string m_report;
public:
	TestTask(const std::string& report): m_report(report) {

	}

	virtual std::string execute() override {
		return m_report;
	}
};

class TestFactory: public Engine<std::string, std::string>::TaskFactory {
public:
	virtual Task<std::string> *create(const std::string& cfg) const override {
		return new TestTask(cfg);
	}
};

class TestSender: public Engine<std::string, std::string>::ReportSender {
private:
	std::string m_expected_report;
public:
	void set_expected_report(const std::string& report) {
		m_expected_report = report;
	}

	virtual void send(const std::string& data) const override {
		std::cout << "data received: \"" << data << "\"; expected: \"" << m_expected_report << "\"" << std::endl;
		ASSERT_EQ(data, m_expected_report);
	}
};

TEST(ut_engine, sanity) {
	// GIVEN
	TestFactory factory;
	TestSender sender;
	const std::vector<std::string> test_datas {
		"test1",
		"test2",
		"test3",
		""
	};

	// WHEN
	using TestEngine = Engine<std::string, std::string>;
	TestEngine *engine_ptr(nullptr);
	std::string report("");

	// THEN
	ASSERT_NO_THROW(engine_ptr = new TestEngine(factory, sender));
	for (auto data: test_datas) {
		sender.set_expected_report(data);
		ASSERT_NO_THROW(engine_ptr->run_task(data));
	}
}