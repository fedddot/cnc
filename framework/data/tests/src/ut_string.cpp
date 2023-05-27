#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include <iostream>

#include "string.hpp"

using namespace data;
using namespace testing;

TEST(ut_String, String_Ctors_sanity) {
	// GIVEN
	String test_str1;
	String test_str2("test_str2");
	String test_str3('s');

	// THEN
	ASSERT_STREQ(test_str1.c_str(), "");
	ASSERT_STREQ(test_str2.c_str(), "test_str2");
	ASSERT_STREQ(test_str3.c_str(), "s");
}

TEST(ut_String, String_AssignmentOperator_sanity) {
	// GIVEN
	String test_str1;
	String test_str2("test_str2");
	String test_str3;

	// WHEN
	test_str1 = test_str2;
	test_str3 = "test_str3";

	// THEN
	ASSERT_STREQ(test_str1.c_str(), "test_str2");
	ASSERT_STREQ(test_str1.c_str(), test_str2.c_str());
	ASSERT_STREQ(test_str3.c_str(), "test_str3");
}

TEST(ut_String, String_size_sanity) {
	// GIVEN
	String test_str1;
	String test_str2("test_str2");
	String test_str3('a');

	// THEN
	ASSERT_EQ(test_str1.size(), 0UL);
	ASSERT_EQ(test_str2.size(), 9UL);
	ASSERT_EQ(test_str3.size(), 1UL);
}

TEST(ut_String, String_AddOperator_sanity) {
	// GIVEN
	String test_str1('a');
	String test_str2("bc");
	String test_str3;
	String res;

	// THEN
	res = test_str1 + test_str2;
	ASSERT_STREQ(res.c_str(), "abc");
	res = test_str2 + test_str1;
	ASSERT_STREQ(res.c_str(), "bca");
	res = test_str2 + test_str3;
	ASSERT_STREQ(res.c_str(), "bc");
	res = test_str3 + test_str2;
	ASSERT_STREQ(res.c_str(), "bc");
}
