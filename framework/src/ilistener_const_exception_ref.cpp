#include "exception.hpp"
#include "ilistener.hpp"

using namespace common;
using namespace except;

template <>
IListener<const Exception&>::~IListener() noexcept {

}
