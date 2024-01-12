#include "gtest/gtest.h"
#include <string>

#include "string.hpp"

using namespace cnc_engine;

TEST(ut_string, sanity) {
	// GIVEN
	std::string test_str1("test_str1");
	std::string test_str2("test_str2");

	// WHEN
	String *test_instance = nullptr;

	// THEN
	ASSERT_NO_THROW(test_instance = new String(test_str1));
	ASSERT_EQ(test_str1, test_instance->get());
	ASSERT_NO_THROW(test_instance->set(test_str2));
	ASSERT_EQ(test_str2, test_instance->get());
}