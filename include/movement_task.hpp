#ifndef	__MOVEMENT_TASK_HPP__
#define	__MOVEMENT_TASK_HPP__

#include <string>

#include "itask.hpp"
#include "idata_sender.hpp"

namespace task {
	class MovementTask: public common::ITask {
	public:
		typedef double Distance;
		typedef double Speed;
		enum class Axis: int {
			AX,
			AY,
			AZ
		};
		MovementTask(const Distance& distance, const Speed& speed, Axis axis, data::IDataSender<std::string>& sender);
		virtual void execute() override;
	private:
		const Distance m_distance;
		const Speed m_speed;
		const Axis m_axis;
		data::IDataSender<std::string>& m_sender;
	}; // MovementTask
} // namespace task

#endif // __MOVEMENT_TASK_HPP__