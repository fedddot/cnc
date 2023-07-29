#ifndef	__IDATA_PARSER_HPP__
#define	__IDATA_PARSER_HPP__

#include <memory>
#include <iostream>
#include "idata.hpp"

namespace data {
	class IDataParser {
	public:
		virtual std::shared_ptr<IData> parse(std::istream& data_stream) = 0;
		virtual ~IDataParser() noexcept = 0;
	}; // IData
} // namespace data

#endif // __IDATA_PARSER_HPP__