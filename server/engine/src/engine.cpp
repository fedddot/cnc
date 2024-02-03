#include <memory>

#include "data.hpp"
#include "factory.hpp"
#include "sender.hpp"
#include "task.hpp"

#include "engine.hpp"

using namespace engine;
using namespace data;

Engine::Engine(Factory& task_factory, Sender& report_sender): m_task_factory(task_factory), m_report_sender(report_sender) {

}

void Engine::run_task(const Data& cfg) {
	std::unique_ptr<Task> task_ptr(m_task_factory.create(cfg));
	task_ptr->execute();
	m_report_sender.send(task_ptr->report());
}