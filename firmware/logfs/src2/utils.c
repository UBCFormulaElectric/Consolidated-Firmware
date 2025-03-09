#include "utils.h"

#include "stdio.h"

static bool findNearestValidItem(
    void *context,
    int   left,
    int   right,
    int  *idx,
    int   dir,
    bool (*is_item_corrupted)(void *context, size_t idx))
{
    while (*idx >= left && *idx <= right)
    {
        if (!is_item_corrupted(context, *idx))
        {
            // Found a valid item!
            return true;
        }

        *idx += dir;
    }

    return false;
}

int utils_findMaxInSortedCircularBuffer(
    void *context,
    int   size,
    bool (*compare_items)(void *context, size_t idx1, size_t idx2),
    bool (*is_item_corrupted)(void *context, size_t idx))
{
    if (size == 0)
    {
        return -1;
    }
    else if (size == 1)
    {
        return !is_item_corrupted(context, 0) ? 0 : -1;
    }

    // Sliding window bounds.
    int left  = 0;
    int right = size - 1;

    while (left < right)
    {
        // Step left bound rightwards until we find a valid item. Likewise, step right bound leftwards until we find a
        // valid item.
        if (!findNearestValidItem(context, left, right, &left, 1, is_item_corrupted))
        {
            return -1;
        }
        if (!findNearestValidItem(context, left, right, &right, -1, is_item_corrupted))
        {
            return -1;
        }

        // Estimate the midpoint as the average. Find midpoints that are the closest valid items to the estimate,
        // stepped left and right.
        const int mid_guess     = (left + right) / 2;
        int       mid_step_left = mid_guess;
        if (!findNearestValidItem(context, left, right, &mid_step_left, -1, is_item_corrupted))
        {
            return -1;
        }
        int mid_step_right = mid_guess;
        if (!findNearestValidItem(context, left, right, &mid_step_right, 1, is_item_corrupted))
        {
            return -1;
        }
        const bool mid_guess_valid = mid_step_left == mid_guess && mid_step_right == mid_guess;

        if (mid_step_left == left && (mid_guess_valid || mid_step_right == right))
        {
            // This means there are only 2 valid items left. Compare them.
            return compare_items(context, left, right) ? left : right;
        }

        if (!mid_guess_valid && compare_items(context, mid_step_left, mid_step_right))
        {
            // If there is an invalid chunk in the middle, and the left of it is greater than the right of it, then the
            // left item is the max.
            return mid_step_left;
        }
        else if (compare_items(context, left, mid_step_left))
        {
            // Max is on in the left side if the left bound is greater than the midpoint.
            right = mid_step_left;
        }
        else
        {
            // Otherwise, its on the right side.
            left = mid_step_right;
        }
    }

    return left;
}
