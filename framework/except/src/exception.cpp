#include "exception.hpp"

using namespace except;

Exception::Exception(const data::String& what): m_what(what) {

}

data::String Exception::what() const {
	return m_what;
}