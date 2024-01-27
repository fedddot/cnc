#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include "factory.hpp"
#include "data.hpp"
#include "report.hpp"
#include "sender.hpp"
#include "task.hpp"

namespace cnc_engine {
	class Engine {
	public:
		using TaskFactory = basics::Factory<basics::Task *, data::Data>;
		using ReportSender = basics::Sender<Report>;

		Engine(TaskFactory& task_factory, ReportSender& report_sender);
		Engine(const Engine& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		~Engine() noexcept = default;

		void run_task(const data::Data& task_cfg_data);
	private:
		TaskFactory& m_task_factory;
		ReportSender& m_report_sender;
	};
}

#endif // ENGINE_HPP