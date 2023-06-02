#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "json_object.hpp"
#include "json_array.hpp"
#include "json_string.hpp"
#include "json_utils.hpp"

using namespace testing;
using namespace json;

TEST(SanityTests, JsonObjectMethodParse) {
	// Given
	// JsonObject json_obj;
	std::string test_str1 = "{ \"name1\": \"val1\", \"name2\": {},}";
	std::string test_str2 = "{\"name1\": \"val1\", \"name2\": {  \"name3\": [\"\"]}}C";
	const char *output = nullptr;

	// THEN
	{
		JsonObject json_obj;
		std::vector<char> data = stringToVector(test_str1);
		auto iter = data.begin();
		ASSERT_NO_THROW(iter = json_obj.parse(iter, data.end()));
		ASSERT_TRUE(iter == data.end());
		ASSERT_NE(json_obj.end(), json_obj.find("name1"));
	}

	// THEN
	{
		JsonObject json_obj;
		std::vector<char> data = stringToVector(test_str2);
		auto iter = data.begin();
		ASSERT_NO_THROW(iter = json_obj.parse(iter, data.end()));
		ASSERT_EQ(*iter, 'C');
	}
}
