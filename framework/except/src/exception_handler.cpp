#include "shared_ptr.hpp"
#include "exception_handler.hpp"

using namespace except;
using namespace memory;

ExceptionHandler *ExceptionHandler::getInstance() {
	static SharedPtr<ExceptionHandler> instance(new ExceptionHandler);
	return instance.get();
}