#ifndef	__MOVEMENT_TASK_HPP__
#define	__MOVEMENT_TASK_HPP__

#include "itask.hpp"

namespace task {
	class MovementTask: public ITask {
	public:
		typedef double Distance;
		typedef double Speed;
		enum class Axis: int {
			AX,
			AY,
			AZ
		};
		MovementTask(const Distance& distance, const Speed& speed, Axis axis);
		virtual void execute() override;
	private:
		const Distance m_distance;
		const Speed m_speed;
		const Axis m_axis;
	}; // MovementTask
} // namespace task

#endif // __MOVEMENT_TASK_HPP__