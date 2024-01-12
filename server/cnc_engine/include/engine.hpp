#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include <memory>

#include "factory.hpp"
#include "data.hpp"
#include "sender.hpp"
#include "task.hpp"

namespace cnc_engine {
	class Engine {
	public:
		using TaskFactory = Factory<Task<Data> *, Data>;
		using ReportSender = Sender<Data>;

		inline Engine(TaskFactory& task_factory, ReportSender& report_sender);
		Engine(const Engine& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		~Engine() noexcept = default;

		inline void run_task(const Data& task_cfg_data);
	private:
		TaskFactory& m_task_factory;
		ReportSender& m_report_sender;
	};

	inline Engine::Engine(TaskFactory& task_factory, ReportSender& report_sender): m_task_factory(task_factory), m_report_sender(report_sender) {

	}

	void Engine::run_task(const Data& task_cfg_data) {
		std::unique_ptr<Task<Data>> task_ptr(m_task_factory.create(task_cfg_data));
		Treport report = task_ptr->execute();
		m_report_sender.send(report);
	}
}

#endif // ENGINE_HPP