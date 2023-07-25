#include <cstddef>
#include <vector>
#include <memory>
#include <stdexcept>
#include "idata.hpp"
#include "array.hpp"

using namespace data;

Array::Accessor::Accessor(const std::size_t& index): m_index(index) {

}

Array::Array(const IData& other): UnderlyingClass(dynamic_cast<const UnderlyingClass&>(other)) {

}

Array& Array::operator=(const IData& other) {
	dynamic_cast<UnderlyingClass&>(*this) = dynamic_cast<const UnderlyingClass&>(other);
	return *this;
}

IData& Array::accessAt(const IAccessor& where) {
	auto name = dynamic_cast<const Accessor&>(where);
	auto member = at(name.getIndex());
	if (nullptr == member) {
		throw std::runtime_error("invalid member access");
	}
	return *member;
}

IData& Array::access(const IAccessor& where) {
	return accessAt(where);
}

const IData& Array::access(const IAccessor& where) const {
	return const_cast<Array&>(*this).accessAt(where);
}