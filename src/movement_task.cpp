#include "movement_task.hpp"

using namespace cnc;

MovementTask::MovementTask(const Distance& distance, const Speed& speed, const Axis& axis): m_distance(distance), m_speed(speed), m_axis(axis) {

}