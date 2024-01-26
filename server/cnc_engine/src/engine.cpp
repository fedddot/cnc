#include "data.hpp"
#include "object.hpp"
#include "report.hpp"
#include "string.hpp"
#include "task.hpp"
#include <exception>

#include "engine.hpp"

using namespace cnc_engine;

Engine::Engine(TaskFactory& task_factory, ReportSender& report_sender): m_task_factory(task_factory), m_report_sender(report_sender) {

}

void Engine::run_task(const Data& task_cfg_data) {
	std::unique_ptr<basics::Task> task_ptr(m_task_factory.create(task_cfg_data));
	Report::Result report_result(Report::Result::SUCCESS);
	Object report_data;
	try {
		task_ptr->execute();
	} catch (const std::exception& e) {
		report_data.add("exception", String(e.what()));
		report_result = Report::Result::FAILURE;
	}
	m_report_sender.send(Report(report_result, report_data));
}