#include "exception.hpp"
#include "ilistener.hpp"

using namespace common;

template <>
IListener<const except::Exception&>::~IListener() noexcept {

}
