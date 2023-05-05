#include "ilistener.hpp"

using namespace common;

template <>
common::IListener<const char&>::~IListener() noexcept {
	
}