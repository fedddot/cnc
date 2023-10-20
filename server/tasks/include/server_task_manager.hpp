#ifndef	__SERVER_TASK_MANAGER_HPP__
#define	__SERVER_TASK_MANAGER_HPP__

#include <vector>
#include <memory>

#include "package_manager.hpp"
#include "ilistener.hpp"

namespace task {
	class ServerTaskManagerResources;
	class ServerTaskManager: public common::IListener<std::vector<char>> {
	public:
		ServerTaskManager(communication::PackageManager& package_manager);
		ServerTaskManager(const ServerTaskManager& other) = delete;
		ServerTaskManager& operator=(const ServerTaskManager& other) = delete;
		virtual ~ServerTaskManager() noexcept override;

		virtual void on_event(const std::vector<char>& event) override;

		bool is_task_pending() const;
		void run_pending_task();
	private:
		communication::PackageManager& m_package_manager;
		std::shared_ptr<ServerTaskManagerResources> m_resources;
		
		void report_exception(const std::string& where, const std::string& what);
	}; // ServerTaskManager
} // namespace cnc
#endif // __SERVER_TASK_MANAGER_HPP__