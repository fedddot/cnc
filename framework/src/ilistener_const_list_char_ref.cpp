#include "list.hpp"
#include "ilistener.hpp"

using namespace common;
using namespace data;

template <>
IListener<const List<char>&>::~IListener() noexcept {

}