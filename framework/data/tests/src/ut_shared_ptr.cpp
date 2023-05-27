#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include <iostream>

#include "shared_ptr.hpp"
#include "string.hpp"

using namespace data;
using namespace testing;

TEST(ut_SharedPtr, SharedPtr_Ctors_sanity) {
	// GIVEN
	SharedPtr<String> shared_ptr1;
	SharedPtr<String> shared_ptr2(new String("test string"));
	SharedPtr<String> shared_ptr3(shared_ptr2);

	// THEN
	ASSERT_THAT(shared_ptr1.get(), IsNull());
	ASSERT_THAT(shared_ptr2.get(), NotNull());
	ASSERT_EQ(shared_ptr2.get(), shared_ptr3.get());
}

TEST(ut_SharedPtr, SharedPtr_AssignmentOperator_sanity) {
	// GIVEN
	SharedPtr<String> shared_ptr1;
	SharedPtr<String> shared_ptr2(new String("test string"));

	// WHEN
	shared_ptr2 = shared_ptr2;
	shared_ptr1 = shared_ptr2;

	// THEN
	ASSERT_THAT(shared_ptr1.get(), NotNull());
	ASSERT_THAT(shared_ptr2.get(), NotNull());
	ASSERT_EQ(shared_ptr1.get(), shared_ptr2.get());
	ASSERT_STREQ(shared_ptr1.get()->c_str(), shared_ptr2.get()->c_str());
}

// TEST(ut_String, String_size_sanity) {
// 	// GIVEN
// 	String test_str1;
// 	String test_str2("test_str2");
// 	String test_str3('a');

// 	// THEN
// 	ASSERT_EQ(test_str1.size(), 0UL);
// 	ASSERT_EQ(test_str2.size(), 9UL);
// 	ASSERT_EQ(test_str3.size(), 1UL);
// }

// TEST(ut_String, String_AddOperator_sanity) {
// 	// GIVEN
// 	String test_str1('a');
// 	String test_str2("bc");
// 	String test_str3;
// 	String res;

// 	// THEN
// 	res = test_str1 + test_str2;
// 	ASSERT_STREQ(res.c_str(), "abc");
// 	res = test_str2 + test_str1;
// 	ASSERT_STREQ(res.c_str(), "bca");
// 	res = test_str2 + test_str3;
// 	ASSERT_STREQ(res.c_str(), "bc");
// 	res = test_str3 + test_str2;
// 	ASSERT_STREQ(res.c_str(), "bc");
// }
