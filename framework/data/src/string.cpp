#include <string>
#include <stdexcept>
#include "idata.hpp"
#include "string.hpp"

using namespace data;

String::String(const IData& other): UnderlyingClass(dynamic_cast<const UnderlyingClass&>(other)) {

}

String& String::operator=(const IData& other) {
	dynamic_cast<UnderlyingClass&>(*this) = dynamic_cast<const UnderlyingClass&>(other);
	return *this;
}