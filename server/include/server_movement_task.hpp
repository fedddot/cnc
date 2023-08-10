#ifndef	__SERVER_MOVEMENT_TASK_HPP__
#define	__SERVER_MOVEMENT_TASK_HPP__

#include <vector>

#include "movement_task.hpp"
#include "idata_sender.hpp"

namespace cnc {
	class ServerMovementTask: public MovementTask {
	public:
		ServerMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, data::IDataSender<const std::vector<char>&> *sender);
		ServerMovementTask(const ServerMovementTask& other) = default;
		ServerMovementTask& operator=(const ServerMovementTask& other) = default;
		
		virtual void execute() override;
	private:
		data::IDataSender<const std::vector<char>&> *m_sender;
		static data::IDataSender<const std::vector<char>&> *init_sender(data::IDataSender<const std::vector<char>&> *sender);
	}; // ClientMovementTask
} // namespace cnc

#endif // __SERVER_MOVEMENT_TASK_HPP__