#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include <memory>

#include "factory.hpp"
#include "sender.hpp"
#include "task.hpp"

namespace cnc_engine {
	template <class Ttask_cfg_data, class Treport>
	class Engine {
	public:
		using TaskFactory = Factory<Task<Treport> *, Ttask_cfg_data>;
		using ReportSender = Sender<Ttask_cfg_data>;

		Engine(TaskFactory& task_factory, ReportSender& report_sender);
		Engine(const Engine& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		~Engine() noexcept = default;

		void run_task(const Ttask_cfg_data& task_cfg_data);
	private:
		TaskFactory& m_task_factory;
		ReportSender& m_report_sender;
	};

	template <class Ttask_cfg_data, class Treport>
	Engine<Ttask_cfg_data, Treport>::Engine(TaskFactory& task_factory, ReportSender& report_sender): m_task_factory(task_factory), m_report_sender(report_sender) {

	}

	template <class Ttask_cfg_data, class Treport>
	void Engine<Ttask_cfg_data, Treport>::run_task(const Ttask_cfg_data& task_cfg_data) {
		std::unique_ptr<Task<Treport>> task_ptr(m_task_factory.create(task_cfg_data));
		Treport report = task_ptr->execute();
		m_report_sender.send(report);
	}
}

#endif // ENGINE_HPP