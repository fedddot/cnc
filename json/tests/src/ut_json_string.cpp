#include <stdexcept>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "json_string.hpp"

using namespace json;
using namespace testing;

TEST(SanityTests, JsonStringMethodParse) {
	// Given
	JsonString json_str;
	const char * const test_str1 = "\"param1\"";
	const char * const test_str2 = " \"param2\"C  ";
	const char *output = nullptr;

	// Then
	ASSERT_NO_THROW(output = json_str.parse(test_str1));
	ASSERT_THAT(output, NotNull());
	ASSERT_THAT(*output, Eq('\0'));
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"param1\"");

	ASSERT_NO_THROW(output = json_str.parse(test_str2));
	ASSERT_THAT(output, NotNull());
	ASSERT_THAT(*output, Eq('C'));
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"param2\"");
}

TEST(NegativeTests, JsonStringMethodParse) {
	// Given
	JsonString json_str;
	const char * const test_str1 = "param1";
	const char * const test_str2 = "param2\"C  ";
	const char * const test_str3 = " \"param3  ";
	const char *output = nullptr;

	// Then
	ASSERT_THROW(output = json_str.parse(nullptr), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"\"");

	ASSERT_THROW(output = json_str.parse(test_str1), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"\"");

	ASSERT_THROW(output = json_str.parse(test_str2), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"\"");
}