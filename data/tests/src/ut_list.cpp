#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include <iostream>

#include "list.hpp"
#include "string.hpp"

using namespace data;
using namespace testing;

TEST(ut_List, push_front_pop_front_SANITY) {
	// GIVEN
	List<int> test_list;
	int test_values[] = { 0, 1, 2, 3 };
	const size_t test_values_size = 4;

	// WHEN
	size_t expected_size = 0;
	
	// THEN
	for (int i = 0; i < test_values_size; ++i) {
		ASSERT_NO_THROW(test_list.push_front(test_values[i]));
		++expected_size;
		ASSERT_EQ(expected_size, test_list.size());
	}

	for (int i = test_values_size - 1; i > 0; --i) {
		int val = 0;
		ASSERT_NO_THROW(val = test_list.pop_front());
		--expected_size;
		ASSERT_EQ(expected_size, test_list.size());
		ASSERT_EQ(val, test_values[i]);
	}
}

TEST(ut_List, push_back_pop_back_SANITY) {
	// GIVEN
	List<int> test_list;
	int test_values[] = { 0, 1, 2, 3 };
	const size_t test_values_size = 4;

	// WHEN
	size_t expected_size = 0;	
	
	// THEN
	for (int i = 0; i < test_values_size; ++i) {
		ASSERT_NO_THROW(test_list.push_back(test_values[i]));
		++expected_size;
		ASSERT_EQ(expected_size, test_list.size());
	}

	for (int i = test_values_size - 1; i > 0; --i) {
		int val = 0;
		ASSERT_NO_THROW(val = test_list.pop_back());
		--expected_size;
		ASSERT_EQ(expected_size, test_list.size());
		ASSERT_EQ(val, test_values[i]);
	}
}

TEST(ut_List, dtor_LEAKTEST) {
	// GIVEN
	List<int> test_list;
	int test_values[] = { 0, 1, 2, 3 };
	const size_t test_values_size = 4;
	
	// THEN
	for (int i = 0; i < test_values_size; ++i) {
		ASSERT_NO_THROW(test_list.push_back(test_values[i]));
	}
}
