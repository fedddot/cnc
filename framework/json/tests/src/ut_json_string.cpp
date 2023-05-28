#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "json_string.hpp"
#include "json_utils.hpp"

using namespace testing;
using namespace json;

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
	std::string valid_str1("\"test_string1\"");
	std::string valid_str2("  \"test_string2 \"e");
	std::string invalid_str("\"test_string3");

	// THEN
	{
		JsonString json_str;
		std::vector<char> data = stringToVector(valid_str1);
		auto iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter, data.end()));
		ASSERT_TRUE(iter == data.end());
		ASSERT_STREQ("\"test_string1\"", json_str.getJsonString().c_str());
	}

	// THEN
	{
		JsonString json_str;
		std::vector<char> data = stringToVector(valid_str2);
		auto iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter, data.end()));
		ASSERT_FALSE(iter == data.end());
		ASSERT_STREQ("\"test_string2 \"", json_str.getJsonString().c_str());
		ASSERT_EQ('e', *iter);
	}

	// THEN
	{
		JsonString json_str;
		std::vector<char> data = stringToVector(invalid_str);
		auto iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter, data.end()));
		ASSERT_TRUE(iter == data.end());
		ASSERT_STREQ("\"test_string2 \"", json_str.getJsonString().c_str());
	}
}
