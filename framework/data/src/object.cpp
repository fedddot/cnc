#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include "idata.hpp"
#include "object.hpp"

using namespace data;

Object::Object(const Object& other): ContainerType(other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

Object& Object::operator=(const Object& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	static_cast<ContainerType&>(*this) = other;
	return *this;
}

Object::Object(const IData& other): ContainerType(dynamic_cast<const Object&>(other)) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

Object& Object::operator=(const IData& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	*this = dynamic_cast<const Object&>(other);
	return *this;
}