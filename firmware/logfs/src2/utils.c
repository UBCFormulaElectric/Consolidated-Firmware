#include "utils.h"

#include "logfs2.h"
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

LogFsErr utils_findMaxInSortedCircularBuffer(
    void *context,
    int   size,
    LogFsErr (*compare_items)(void *context, size_t idx1, size_t idx2, bool *out),
    bool (*is_item_corrupted)(void *context, size_t idx),
    uint32_t *out)
{
    if (size == 0)
    {
        return -1;
    }

    int  left  = 0;
    int  right = size - 1;
    bool comparison;

    while (true)
    {
        // Step left bound rightwards until we find a valid item. Likewise, step right bound leftwards until we find a
        // valid item.
        if (!findNearestValidItem(context, left, right, &left, 1, is_item_corrupted) ||
            !findNearestValidItem(context, left, right, &right, -1, is_item_corrupted))
        {
            return -1;
        }

        // Estimate the midpoint as the average.
        const int  mid_guess       = (left + right) / 2;
        const bool mid_guess_valid = !is_item_corrupted(context, mid_guess);

        if (mid_guess_valid)
        {
            if (mid_guess == left)
            {
                // This means there are only 2 valid items left. Compare them.
                RET_ERR(compare_items(context, left, right, &comparison));
                *out = comparison ? left : right;
                return LOGFS_ERR_OK;
            }

            // Max is on in the left side if the left bound is greater than the midpoint. Otherwise, its on the right
            // side.
            RET_ERR(compare_items(context, left, mid_guess, &comparison));
            if (comparison)
            {
                right = mid_guess;
            }
            else
            {
                left = mid_guess;
            }
        }
        else
        {
            // Midpoint is invalid. Find midpoints that are the closest valid items to the estimate,
            // stepped left and right.
            int mid_step_left  = mid_guess;
            int mid_step_right = mid_guess;
            if (!findNearestValidItem(context, left, right, &mid_step_left, -1, is_item_corrupted) ||
                !findNearestValidItem(context, left, right, &mid_step_right, 1, is_item_corrupted))
            {
                return LOGFS_ERR_CORRUPT;
            }

            if (mid_step_left == left && mid_step_right == right)
            {
                // This means there are only 2 valid items left. Compare them.
                RET_ERR(compare_items(context, left, right, &comparison));
                *out = comparison ? left : right;
                return LOGFS_ERR_OK;
            }

            // If there is an invalid chunk in the middle, and the left of it is greater than the right of it, then
            // the left item is the max.
            RET_ERR(compare_items(context, mid_step_left, mid_step_right, &comparison));
            if (comparison)
            {
                *out = mid_step_left;
                return LOGFS_ERR_OK;
            }

            // Max is on in the left side if the left bound is greater than the midpoint. Otherwise, its on the right
            // side.
            RET_ERR(compare_items(context, left, mid_step_left, &comparison));
            if (comparison)
            {
                right = mid_step_left;
            }
            else
            {
                left = mid_step_right;
            }
        }
    }
}
