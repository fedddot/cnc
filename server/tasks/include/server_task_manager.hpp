#ifndef	__SERVER_TASK_MANAGER_HPP__
#define	__SERVER_TASK_MANAGER_HPP__

#include <string>
#include <vector>
#include <list>
#include <memory>

#include "factory.hpp"
#include "object.hpp"
#include "package_manager.hpp"
#include "ilistener.hpp"
#include "server_task.hpp"

namespace task {
	class ServerTaskManager: public common::IListener<std::vector<char>> {
	public:
		ServerTaskManager(communication::PackageManager& package_manager);
		ServerTaskManager(const ServerTaskManager& other) = delete;
		ServerTaskManager& operator=(const ServerTaskManager& other) = delete;

		virtual void on_event(const std::vector<char>& event) override;

		bool is_task_pending() const;
		void run_pending_task();
	private:
		communication::PackageManager& m_package_manager;
		
		typedef common::Factory<std::string, std::shared_ptr<ServerTask>, data::Object> TasksFactory;

		typedef common::ICreator<std::shared_ptr<ServerTask>, data::Object> TasksCreator;

		common::Registry<hardware::Gpio::PinNumber, std::shared_ptr<hardware::Gpio>> m_gpio_registry;

		TasksFactory m_task_factory;

		std::list<std::shared_ptr<ServerTask>> m_tasks;

		void init_creators();
		static data::Object parse_raw_data(const std::vector<char>& raw_data);
		static std::string get_task_type(const data::Object& config_data);

		void report_exception(const std::string& where, const std::string& what);
	}; // ServerTaskManager
} // namespace cnc
#endif // __SERVER_TASK_MANAGER_HPP__