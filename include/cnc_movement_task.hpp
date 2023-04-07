#ifndef	__CNC_MOVEMENT_TASK_HPP__
#define	__CNC_MOVEMENT_TASK_HPP__

#include <cstddef>
#include <string>

#include "common_types.hpp"
#include "icnc_task.hpp"

namespace common {
	class CncMovementTask: public ICncTask {
	public:
		enum class Axis: Byte {
			X = 0,
			Y = 1,
			Z = 2,
			UNDEFINED = (Byte)(-1)
		};
		
		CncMovementTask(const std::string& id_, const Axis& axis_, const std::size_t& steps_num_, const double& steps_per_sec_);
		virtual void execute() override;
		virtual Priority getPriority() const override;
		virtual Type getType() const override;
		virtual std::string serialize() const override;
	private:
		std::string m_id;
		Type m_type;
		Axis m_axis;
		std::size_t m_steps_num;
		double m_steps_per_sec;
		Priority m_priority;
		static std::string axisToString(const Axis& axis_);
	};
}

#endif // __CNC_MOVEMENT_TASK_HPP__