#include <cstddef>

#include "movement_task.hpp"

using namespace task;

int main(void) {
	MovementTask task(0.2, 12, MovementTask::Axis::AX);
	task.execute();
	return 0;
}