#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include <iostream>

#include "exception.hpp"
#include "string.hpp"

using namespace data;
using namespace except;
using namespace testing;

TEST(ut_Exception, Exception_Ctors_sanity) {
	// GIVEN
	Exception test_exception1;
	String test_exception2_what("test_exception2");
	Exception test_exception2(test_exception2_what);

	// THEN
	ASSERT_STREQ(test_exception1.what().c_str(), "");
	ASSERT_STREQ(test_exception2.what().c_str(), test_exception2_what.c_str());
}
