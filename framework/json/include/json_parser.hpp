#ifndef	__JSON_PARSER_HPP__
#define	__JSON_PARSER_HPP__

#include <iostream>
#include <memory>

#include "idata.hpp"
#include "idata_parser.hpp"

namespace json {
	class JsonParser: public data::IDataParser {
	public:
		virtual std::shared_ptr<data::IData> parse(std::istream& data_stream) override;
	private:
		std::shared_ptr<data::IData> parseString(std::istream& data_stream);

		static char peekChar(std::istream& data_stream);
		static char popChar(std::istream& data_stream);
	};
}

#endif // __JSON_PARSER_HPP__