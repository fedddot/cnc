#ifndef	__CLIENT_MOVEMENT_TASK_HPP__
#define	__CLIENT_MOVEMENT_TASK_HPP__

#include <vector>

#include "movement_task.hpp"
#include "idata_sender.hpp"

namespace cnc {
	class ClientMovementTask: public MovementTask {
	public:
		ClientMovementTask(const Distance& distance, const Speed& speed, const Axis& axis, data::IDataSender<const std::vector<char>&> *sender);
		ClientMovementTask(const ClientMovementTask& other) = default;
		ClientMovementTask& operator=(const ClientMovementTask& other) = default;
		
		virtual void execute() override;
	private:
		data::IDataSender<const std::vector<char>&> *m_sender;
		static data::IDataSender<const std::vector<char>&> *init_sender(data::IDataSender<const std::vector<char>&> *sender);
	}; // ClientMovementTask
} // namespace cnc

#endif // __CLIENT_MOVEMENT_TASK_HPP__