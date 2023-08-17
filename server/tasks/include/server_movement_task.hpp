#ifndef	__SERVER_MOVEMENT_TASK_HPP__
#define	__SERVER_MOVEMENT_TASK_HPP__

#include <memory>

#include "idata.hpp"
#include "iserver_task.hpp"
#include "movement_task.hpp"

namespace task {
	class ServerMovementTask: public MovementTask, public IServerTask {
	public:
		ServerMovementTask(const Distance& distance, const Speed& speed, const Axis& axis);
		ServerMovementTask(const ServerMovementTask& other) = delete;
		ServerMovementTask& operator=(const ServerMovementTask& other) = delete;
		
		virtual void execute() override;
		virtual inline std::shared_ptr<data::IData> report() const override;
	private:
		std::shared_ptr<data::IData> m_report;
		std::shared_ptr<data::IData> createReport(const std::string& result) const;
	}; // ServerMovementTask

	inline std::shared_ptr<data::IData> ServerMovementTask::report() const {
		return m_report;
	}
} // namespace task

#endif // __SERVER_MOVEMENT_TASK_HPP__