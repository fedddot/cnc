#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <memory>

#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "json_parser.hpp"
#include "idata.hpp"

using namespace testing;
using namespace data;
using namespace json;

TEST(ut_json_parser, JsonParser_parse_sanity) {
	// GIVEN
	std::vector<std::string> test_cases {
		"{ \"name1\": \"val1\", \"name2\": {},}",
		"{\"name1\": \"val1\", \"name2\": {  \"name3\": [\"\"]}}C"
	};

	std::for_each(
		test_cases.begin(),
		test_cases.end(),
		[](const auto& test_case) {
			// WHEN
			std::shared_ptr<IData> result(nullptr);
			std::stringstream data_stream;
			data_stream << test_case;
			JsonParser parser;

			// THEN
			ASSERT_NO_THROW(result = parser.parse(data_stream));
			ASSERT_THAT(result, NotNull());
			ASSERT_EQ(result->getDataType(), IData::DataType::COMPOSITE);
		}
	);
}
