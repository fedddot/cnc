#include "exception_handler.hpp"

using namespace except;

ExceptionHandler *ExceptionHandler::instance_ptr = nullptr;

ExceptionHandler *ExceptionHandler::getInstance() {
	if (nullptr == instance_ptr) {
		instance_ptr = new ExceptionHandler;
	}
	return instance_ptr;
}

void ExceptionHandler::deleteInstance() {
	delete instance_ptr;
	instance_ptr = nullptr;
}