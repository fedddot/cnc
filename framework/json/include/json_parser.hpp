#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <iostream>
#include <memory>
#include <vector>

#include "idata.hpp"
#include "idata_parser.hpp"

namespace json
{
	class JsonParser : public data::IDataParser
	{
	public:
		virtual std::shared_ptr<data::IData> parse(std::istream& data_stream) override;

	private:
		std::shared_ptr<data::IData> parseString(std::istream& data_stream);
		std::shared_ptr<data::IData> parseObject(std::istream& data_stream);
		std::shared_ptr<data::IData> parseArray(std::istream& data_stream);

		static bool isEos(std::istream& data_stream);

		static void skipChars(std::istream& data_stream, const std::vector<char>& chars);
		static void skipBulks(std::istream& data_stream);
		static void skipBetweenFieldNameAndMember(std::istream& data_stream);
		static void skipAfterMember(std::istream& data_stream, char expected_closing_bracket);

	};
}

#endif // __JSON_PARSER_HPP__