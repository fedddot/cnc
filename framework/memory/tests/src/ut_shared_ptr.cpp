#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include <iostream>

#include "shared_ptr.hpp"
#include "string.hpp"

using namespace data;
using namespace memory;
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
