#include "shared_ptr.hpp"
#include "exception_handler.hpp"

using namespace except;

ExceptionHandler *ExceptionHandler::getInstance() {
	static data::SharedPtr<ExceptionHandler> instance(new ExceptionHandler);
	return instance.get();
}