#include <vector>
#include "ilistener.hpp"

using namespace common;

template <>
IListener<const std::vector<char>&>::~IListener() noexcept {

}