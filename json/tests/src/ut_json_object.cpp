#include <stdexcept>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "json_object.hpp"

using namespace json;
using namespace testing;

TEST(SanityTests, JsonObjectMethodParse) {
	// Given
	JsonObject json_obj;
	const char * const test_str1 = "{ \"name1\": \"val1\", \"name2\": {},}";
	const char * const test_str2 = "{\"name1\": \"val1\", \"name2\": {  \"name3\": [\"\"]}}C";
	// const char * const test_str2 = " \"param2\"C  ";
	const char *output = nullptr;

	// Then
	ASSERT_NO_THROW(output = json_obj.parse(test_str1));
	ASSERT_THAT(output, NotNull());
	ASSERT_THAT(*output, Eq('\0'));
	ASSERT_STREQ(json_obj.getJsonString().c_str(), "{ \"name1\": \"val1\", \"name2\": {},}");

	ASSERT_NO_THROW(output = json_obj.parse(test_str2));
	ASSERT_THAT(output, NotNull());
	ASSERT_THAT(*output, Eq('C'));
	ASSERT_STREQ(json_obj.getJsonString().c_str(), "{ \"name1\": \"val1\", \"name2\": { \"name3\": [ \"\",],},}");
}

TEST(NegativeTests, JsonObjectMethodParse) {
	// Given
	JsonObject json_obj;
	// <<TODO>>: make test_strs more interesting
	const char * const test_str1 = "param1";
	const char * const test_str2 = "param2\"C  ";
	const char * const test_str3 = " \"param3  ";
	const char *output = nullptr;

	// Then
	ASSERT_THROW(output = json_obj.parse(nullptr), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_obj.getJsonString().c_str(), "{}");

	ASSERT_THROW(output = json_obj.parse(test_str1), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_obj.getJsonString().c_str(), "{}");

	ASSERT_THROW(output = json_obj.parse(test_str2), std::invalid_argument);
	ASSERT_THAT(output, IsNull());
	ASSERT_STREQ(json_obj.getJsonString().c_str(), "{}");
}