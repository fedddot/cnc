#include <stdexcept>

#include "exception.hpp"
#include "exception_handler.hpp"

using namespace except;

void ExceptionHandler::onEvent(const Exception& event) {
	throw std::runtime_error(event.what().c_str());
}
