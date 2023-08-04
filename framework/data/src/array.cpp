#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "idata.hpp"
#include "array.hpp"

using namespace data;

Array::Array(const Array& other): ContainerType(other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

Array& Array::operator=(const Array& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	static_cast<ContainerType&>(*this) = other;
	return *this;
}

Array::Array(const IData& other): ContainerType(dynamic_cast<const Array&>(other)) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

Array& Array::operator=(const IData& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	*this = dynamic_cast<const Array&>(other);
	return *this;
}