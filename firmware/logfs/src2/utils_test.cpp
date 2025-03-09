#include "utils.h"
#include <vector>
#include <gtest/gtest.h>

static int findMaxInSortedCircularArray(std::vector<int> &vec)
{
    auto compare_items = [](void *buf, size_t idx1, size_t idx2)
    {
        int *vec = reinterpret_cast<int *>(buf);
        return vec[idx1] > vec[idx2];
    };
    auto is_corrupted = [](void *buf, size_t idx)
    {
        int *vec = reinterpret_cast<int *>(buf);
        return vec[idx] < 0;
    };
    return utils_findMaxInSortedCircularBuffer(vec.data(), vec.size(), compare_items, is_corrupted);
}

TEST(UtilsTest, test_find_max_no_corruption)
{
    std::vector<int> arr1 = { 4, 5, 6, 7, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr1), 3);

    std::vector<int> arr2 = { 7, 8, 9, 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr2), 2);

    std::vector<int> arr3 = { 1, 2, 3, 4, 5, 6, 7 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr3), 6);

    std::vector<int> arr4 = { 9 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr4), 0);

    std::vector<int> arr5 = { 3, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr5), 0);

    std::vector<int> arr6 = { 5, 6 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr6), 1);

    std::vector<int> arr7 = { 6, 5 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr7), 0);
}

TEST(UtilsTest, test_find_max_corruption)
{
    std::vector<int> arr1 = { 4, 5, 6, -1, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr1), 2);

    std::vector<int> arr2 = { 4, -1, -1, -1, -1, -1, 3 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr2), 0);

    std::vector<int> arr3 = { 3, -1, -1, -1, -1, -1, 4 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr3), 6);

    std::vector<int> arr4 = { 7, -1, 8, -1, 9, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5, -1, 6 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr4), 4);

    std::vector<int> arr5 = { 3, 1, 2, -1, -1 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr5), 0);

    std::vector<int> arr6 = { -1, -1, 3, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr6), 2);

    std::vector<int> arr7 = { -1, -1, -1, -1, -1, -1 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr7), -1);

    std::vector<int> arr8 = { 3, -1, -1, -1, -1, -1, 4, 5, 2 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr8), 7);

    std::vector<int> arr9 = { 3, 4, 5 - 1, -1, -1, -1, -1, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr9), 2);

    std::vector<int> arr10 = { 3, -1, -1, -1, -1, -1, 4, -1, -1, 5, -1, 2 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr10), 9);

    std::vector<int> arr11 = { 4, 5, -1, 6, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularArray(arr11), 3);
}
