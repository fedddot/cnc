#include "ilistener.hpp"
#include "message.hpp"

using namespace common;

template <>
IListener<const message::Message&>::~IListener() noexcept {

}