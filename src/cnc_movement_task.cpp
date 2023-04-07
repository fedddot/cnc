#include <iostream>
#include <sstream>

#include "cnc_movement_task.hpp"

using namespace common;

CncMovementTask::CncMovementTask(const std::string& id_, const Axis& axis_, const std::size_t& steps_num_, const double& steps_per_sec_) : m_id(id_), m_type(Type::MOVEMENT), m_axis(axis_), m_steps_num(steps_num_), m_steps_per_sec(steps_per_sec_), m_priority(Priority::WORKFLOW) {

}

void CncMovementTask::execute() {
	// TODO: implement
	std::cout << "MOVEMENT task: axis = " << (int)m_axis << "; steps_num = " << m_steps_num << "; speed = " << m_steps_per_sec << " steps/sec" << std::endl;
}

CncMovementTask::Priority CncMovementTask::getPriority() const {
	return m_priority;
}
CncMovementTask::Type CncMovementTask::getType() const {
	return m_type;
}

std::string CncMovementTask::serialize() const {
	std::stringstream json_stream;
	json_stream << "{" <<
		"\"id\" : " << "\"" << m_id << "\", " <<
		"\"type\" : " << "\"" << "MOVEMENT" << "\", " <<
		"\"axis\" : " << "\"" << axisToString(m_axis) << "\", " <<
		"\"stepsNum\" : " << "\"" << m_steps_num << "\", " <<
		"\"stepsPerSec\" : " << "\"" << m_steps_per_sec << "\"" << "}";
	return json_stream.str();
}

std::string CncMovementTask::axisToString(const Axis& axis_) {
	switch (axis_)
	{
	case Axis::X:
		return "X";
	case Axis::Y:
		return "Y";
	case Axis::Z:
		return "Z";
	default:
		break;
	}
	return "UNDEFINED";
}
