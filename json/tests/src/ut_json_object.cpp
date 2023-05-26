#include <stdexcept>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "json_object.hpp"
#include "list.hpp"
#include "string.hpp"
#include "test_utils.hpp"

using namespace json;
using namespace data;
using namespace testing;

TEST(SanityTests, JsonObjectMethodParse) {
	// Given
	// JsonObject json_obj;
	String test_str1 = "{ \"name1\": \"val1\", \"name2\": {},}";
	String test_str2 = "{\"name1\": \"val1\", \"name2\": {  \"name3\": [\"\"]}}C";
	const char *output = nullptr;

	// THEN
	{
		JsonObject json_obj;
		List<char> data = strToList(test_str1);
		List<char>::Iter iter = data.begin();
		ASSERT_NO_THROW(iter = json_obj.parse(iter));
		ASSERT_TRUE(iter == data.end());
	}

	// THEN
	{
		JsonObject json_obj;
		List<char> data = strToList(test_str2);
		List<char>::Iter iter = data.begin();
		ASSERT_NO_THROW(iter = json_obj.parse(iter));
		ASSERT_EQ(iter.get(), 'C');
	}
}
