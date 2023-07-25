#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include "idata.hpp"
#include "object.hpp"

using namespace data;

Object::Accessor::Accessor(const std::string& name): m_name(name) {

}

Object::Object(const IData& other): UnderlyingClass(dynamic_cast<const UnderlyingClass&>(other)) {

}

Object& Object::operator=(const IData& other) {
	dynamic_cast<UnderlyingClass&>(*this) = dynamic_cast<const UnderlyingClass&>(other);
	return *this;
}

IData& Object::accessAt(const IAccessor& where) {
	auto name = dynamic_cast<const Accessor&>(where);
	auto member = at(name.getName());
	if (nullptr == member) {
		throw std::runtime_error("invalid member access");
	}
	return *member;
}

IData& Object::access(const IAccessor& where) {
	return accessAt(where);
}

const IData& Object::access(const IAccessor& where) const {
	return const_cast<Object&>(*this).accessAt(where);
}