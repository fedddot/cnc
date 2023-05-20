#include <string.h>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "json_string.hpp"

using namespace json;
using namespace testing;

TEST(ut_JsonString, JsonString_Ctors_sanity) {
	// GIVEN
	JsonString json_str1;
	JsonString json_str2("abc");

	// THEN
	ASSERT_STREQ(json_str1.getJsonString().c_str(), "\"\"");
	ASSERT_STREQ(json_str2.getJsonString().c_str(), "\"abc\"");
}

TEST(ut_JsonString, JsonString_parse_sanity) {
	// GIVEN
	JsonString json_str;
	const char * const test_str1 = "\"param1\"";
	const char * const test_str2 = " \"param2\"C  ";
	const char * const test_str3 = " param3\"C  ";
	const char *output = nullptr;

	// THEN
	ASSERT_NO_THROW(output = json_str.parse(test_str1));
	ASSERT_THAT(output, Eq(test_str1 + strlen(test_str1)));
	ASSERT_THAT(*output, Eq('\0'));
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"param1\"");

	ASSERT_NO_THROW(output = json_str.parse(test_str2));
	ASSERT_THAT(output, Eq(test_str2 + strlen(test_str2) - 3));
	ASSERT_THAT(*output, Eq('C'));
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"param2\"");

	ASSERT_NO_THROW(output = json_str.parse(test_str3));
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_str.getJsonString().c_str(), "\"param2\"");
}
