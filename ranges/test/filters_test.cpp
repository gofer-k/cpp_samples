#include <gtest/gtest.h>
#include "src/filters.hpp" // Include the header file containing the filter_range function

// Test fixture for the filter_range function
class FilterRangeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code that runs before each test
    }

    void TearDown() override {
        // Code that runs after each test
    }
};

// Test case to check the filter_range function with a positive case
TEST_F(FilterRangeTest, EvenCase) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> result = ranges_filters::filter_range(input, [](int arg) {
        return arg % 2 == 0;
    });
    std::vector<int> expected = {2, 4};
    ASSERT_EQ(result, expected);
}

TEST_F(FilterRangeTest, OddCase) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> result = ranges_filters::filter_range(input, [](int arg) {
        return arg % 2 != 0;
    });
    std::vector<int> expected = {1, 3, 5};
    ASSERT_EQ(result, expected);
}

// Test case to check the filter_range function with an empty list
TEST_F(FilterRangeTest, EmptyList) {
    std::vector<int> input = {};
    std::vector<int> result = ranges_filters::filter_range(input, [](int) { return true; });
    std::vector<int> expected = {};
    ASSERT_EQ(result, expected);
}

// Test case to check the filter_range function with a negative case
TEST_F(FilterRangeTest, NegativeCase) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> result = ranges_filters::filter_range(input, [](int) { return false; });
    std::vector<int> expected = {};
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}