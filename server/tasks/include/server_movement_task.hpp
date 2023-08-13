#ifndef	__SERVER_MOVEMENT_TASK_HPP__
#define	__SERVER_MOVEMENT_TASK_HPP__

#include <vector>

#include "movement_task.hpp"
#include "isender.hpp"

namespace cnc {
	class ServerMovementTask: public MovementTask {
	public:
		ServerMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, communication::ISender<const std::vector<char>&>& sender);
		ServerMovementTask(const ServerMovementTask& other) = delete;
		ServerMovementTask& operator=(const ServerMovementTask& other) = delete;
		
		virtual void execute() override;
	private:
		communication::ISender<const std::vector<char>&>& m_sender;
	}; // ClientMovementTask
} // namespace cnc

#endif // __SERVER_MOVEMENT_TASK_HPP__