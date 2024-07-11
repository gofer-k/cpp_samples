#include <gtest/gtest.h>
#include "filter_range.h" // Include the header file containing the filter_range function

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
TEST_F(FilterRangeTest, PositiveCase) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> result = filter_range(input, 2, 4);
    std::vector<int> expected = {2, 3, 4};
    ASSERT_EQ(result, expected);
}

// Test case to check the filter_range function with an empty list
TEST_F(FilterRangeTest, EmptyList) {
    std::vector<int> input = {};
    std::vector<int> result = filter_range(input, 2, 4);
    std::vector<int> expected = {};
    ASSERT_EQ(result, expected);
}

// Test case to check the filter_range function with a negative case
TEST_F(FilterRangeTest, NegativeCase) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> result = filter_range(input, 6, 8);
    std::vector<int> expected = {};
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}