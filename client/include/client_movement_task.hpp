#ifndef	__CLIENT_MOVEMENT_TASK_HPP__
#define	__CLIENT_MOVEMENT_TASK_HPP__

#include <vector>

#include "movement_task.hpp"
#include "isender.hpp"

namespace task {
	class ClientMovementTask: public MovementTask {
	public:
		ClientMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, communication::ISender<const std::vector<char>&>& sender);
		ClientMovementTask(const ClientMovementTask& other) = default;
		ClientMovementTask& operator=(const ClientMovementTask& other) = default;
		
		virtual void execute() override;
	private:
		communication::ISender<const std::vector<char>&>& m_sender;
	}; // ClientMovementTask
} // namespace cnc

#endif // __CLIENT_MOVEMENT_TASK_HPP__