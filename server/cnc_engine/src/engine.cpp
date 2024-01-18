#include "data.hpp"
#include "report.hpp"
#include "task.hpp"

#include "engine.hpp"

using namespace cnc_engine;

Engine::Engine(TaskFactory& task_factory, ReportSender& report_sender): m_task_factory(task_factory), m_report_sender(report_sender) {

}

void Engine::run_task(const Data& task_cfg_data) {
	std::unique_ptr<basics::Task<Report>> task_ptr(m_task_factory.create(task_cfg_data));
	Report report = task_ptr->execute();
	m_report_sender.send(report);
}