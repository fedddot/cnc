#include "exception.hpp"

using namespace except;

Exception::Exception(const ExceptionType& what): m_what(what) {

}

Exception::ExceptionType Exception::what() const {
	return m_what;
}