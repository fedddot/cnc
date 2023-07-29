#include <iostream>
#include <memory>

#include "idata.hpp"
#include "idata_parser.hpp"

#include "json_common.hpp"
#include "json_parser.hpp"

using namespace json;
using namespace data;

std::shared_ptr<IData> JsonParser::parse(std::istream& data_stream) {

}

std::shared_ptr<data::IData> parseString(std::istream& data_stream) {
	if (JsonSpecialChar::STRING_START != peekChar(data_stream)) {
		throw
	}
}