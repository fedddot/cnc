#include <cstddef>

#include "movement_task.hpp"
#include "data_sender.hpp"

using namespace task;
using namespace data;

int main(void) {
	DataSender sender("CNC_HEADER", 2UL);

	MovementTask task(0.2, 12, MovementTask::Axis::AX, sender);
	task.execute();
	return 0;
}