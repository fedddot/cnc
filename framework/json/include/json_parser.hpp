#ifndef	__JSON_PARSER_HPP__
#define	__JSON_PARSER_HPP__

#include <cstddef>
#include <vector>
#include <string>
#include <memory>

#include "idata.hpp"
#include "idata_parser.hpp"

namespace json {
	class JsonParser: public IDataParser {
	public:
		virtual std::shared_ptr<IData> parse(std::istream& data_stream) override;
	};
}

#endif // __JSON_PARSER_HPP__