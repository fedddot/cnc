#include <stdexcept>

#include "exception.hpp"
#include "exception_handler.hpp"

using namespace except;

void ExceptionHandler::onEvent(const Exception& event) {
	switch (event.what()) {
	case Exception::ExceptionType::BAD_ALLOC:
		throw std::runtime_error("BAD_ALLOC");
	case Exception::ExceptionType::BAD_ARGUMENT:
		throw std::invalid_argument("BAD_ARGUMENT");
	default:
		throw std::runtime_error("OTHER");
	}
}
