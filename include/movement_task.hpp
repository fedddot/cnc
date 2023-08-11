#ifndef	__MOVEMENT_TASK_HPP__
#define	__MOVEMENT_TASK_HPP__

#include <string>

#include "itask.hpp"

namespace cnc {
	class MovementTask: public common::ITask {
	public:
		typedef double Distance;
		typedef double Speed;
		enum class Axis: int {
			AX,
			AY,
			AZ
		};
		MovementTask(const Distance& distance, const Speed& speed, const Axis& axis);
		MovementTask(const MovementTask& other) = default;
		MovementTask& operator=(const MovementTask& other) = default;

		virtual void execute() = 0;
		
		inline Distance getDistance() const;
		inline Speed getSpeed() const;
		inline Axis getAxis() const;
	private:
		const Distance m_distance;
		const Speed m_speed;
		const Axis m_axis;
	}; // MovementTask

	inline MovementTask::Distance MovementTask::getDistance() const {
		return m_distance;
	}
	inline MovementTask::Speed MovementTask::getSpeed() const {
		return m_speed;
	}
	inline MovementTask::Axis MovementTask::getAxis() const {
		return m_axis;
	}
} // namespace cnc

#endif // __MOVEMENT_TASK_HPP__