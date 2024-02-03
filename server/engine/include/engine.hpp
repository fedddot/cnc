#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include "factory.hpp"
#include "data.hpp"
#include "sender.hpp"

namespace engine {
	class Engine {
	public:
		Engine(Factory& task_factory, Sender& report_sender);
		Engine(const Engine& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		~Engine() noexcept = default;

		void run_task(const data::Data& cfg);
	private:
		Factory& m_task_factory;
		Sender& m_report_sender;
	};
}

#endif // ENGINE_HPP