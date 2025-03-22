#include "logfs2.h"
#include "utils.h"
#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

static int findMaxInSortedCircularVec(std::vector<int> &vec)
{
    auto compare_items = [](void *buf, size_t idx1, size_t idx2, bool* out)
    {
        int *vec = reinterpret_cast<int *>(buf);
        if(vec[idx1] < 0) {
            return LOGFS_ERR_CORRUPT;
        }
        if(vec[idx2] < 0) {
            return LOGFS_ERR_CORRUPT;
        }
        *out = vec[idx1] > vec[idx2];
        return LOGFS_ERR_OK;
    };
    auto is_corrupted = [](void *buf, size_t idx)
    {
        int *vec = reinterpret_cast<int *>(buf);
        return vec[idx] < 0;
    };

    uint32_t out;
    if (utils_findMaxInSortedCircularBuffer(vec.data(), vec.size(), compare_items, is_corrupted, &out) == LOGFS_ERR_OK) {
        return out;
    }
    else {
        return -1;
    }
}

TEST(UtilsTest, test_find_max_no_corruption)
{
    std::vector<int> arr1 = { 4, 5, 6, 7, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr1), 3);

    std::vector<int> arr2 = { 7, 8, 9, 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr2), 2);

    std::vector<int> arr3 = { 1, 2, 3, 4, 5, 6, 7 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr3), 6);

    std::vector<int> arr4 = { 9 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr4), 0);

    std::vector<int> arr5 = { 3, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr5), 0);

    std::vector<int> arr6 = { 5, 6 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr6), 1);

    std::vector<int> arr7 = { 6, 5 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr7), 0);
}

TEST(UtilsTest, test_find_max_corruption)
{
    std::vector<int> arr1 = { 4, 5, 6, -1, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr1), 2);

    std::vector<int> arr2 = { 4, -1, -1, -1, -1, -1, 3 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr2), 0);

    std::vector<int> arr3 = { 3, -1, -1, -1, -1, -1, 4 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr3), 6);

    std::vector<int> arr4 = { 7, -1, 8, -1, 9, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5, -1, 6 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr4), 4);

    std::vector<int> arr5 = { 3, 1, 2, -1, -1 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr5), 0);

    std::vector<int> arr6 = { -1, -1, 3, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr6), 2);

    std::vector<int> arr7 = { -1, -1, -1, -1, -1, -1 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr7), -1);

    std::vector<int> arr8 = { 3, -1, -1, -1, -1, -1, 4, 5, 2 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr8), 7);

    std::vector<int> arr9 = { 3, 4, 5 - 1, -1, -1, -1, -1, 1, 2 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr9), 2);

    std::vector<int> arr10 = { 3, -1, -1, -1, -1, -1, 4, -1, -1, 5, -1, 2 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr10), 9);

    std::vector<int> arr11 = { 4, 5, -1, 6, 0, 1, 2, 3 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr11), 3);

    std::vector<int> arr12 = { -10 };
    ASSERT_EQ(findMaxInSortedCircularVec(arr12), -1);
}
