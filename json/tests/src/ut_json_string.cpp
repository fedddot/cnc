#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "json_string.hpp"
#include "list.hpp"
#include "test_utils.hpp"

using namespace json;
using namespace testing;
using namespace data;

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

	String valid_str1("\"test_string1\"");
	String valid_str2("  \"test_string2 \"e");
	String invalid_str("\"test_string3");

	// THEN
	{
		List<char> data = strToList(valid_str1);
		List<char>::Iter iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter));
		ASSERT_TRUE(iter == data.end());
		ASSERT_STREQ("\"test_string1\"", json_str.getJsonString().c_str());
	}

	// THEN
	{
		List<char> data = strToList(valid_str2);
		List<char>::Iter iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter));
		ASSERT_FALSE(iter == data.end());
		ASSERT_STREQ("\"test_string2 \"", json_str.getJsonString().c_str());
		ASSERT_EQ('e', iter.get());
	}

	// THEN
	{
		List<char> data = strToList(invalid_str);
		List<char>::Iter iter = data.begin();
		ASSERT_NO_THROW(iter = json_str.parse(iter));
		ASSERT_TRUE(iter == data.end());
		ASSERT_STREQ("\"test_string2 \"", json_str.getJsonString().c_str());
	}
}
