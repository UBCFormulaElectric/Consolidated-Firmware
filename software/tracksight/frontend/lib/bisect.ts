/**
 * @param timestamps assumes sorted in ascending order
 * @param targetTime  the timestamp to find the leftmost index for
 * @returns first index where timestamp >= targetTime, or timestamps.length if all timestamps are < targetTime
 */
export function bisect_left(timestamps: number[], targetTime: number): number {
	let left = 0;
	let right = timestamps.length - 1;
	let result = 0;

	while (left <= right) {
		const mid = Math.floor((left + right) / 2);
		if (timestamps[mid] >= targetTime) {
			result = mid;
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}

	if (timestamps[result] < targetTime) {
		result = timestamps.length;
	}
	return result;
}

/**
 * note that this defaults to bisect_left
 * @param timestamps assumes sorted in ascending order
 * @param targetTime the timestamp to find the leftmost index for
 * @returns first index where timestamp >= targetTime, or timestamps.length if all timestamps are < targetTime
 */
export function bisect(timestamps: number[], targetTime: number): number {
	return bisect_left(timestamps, targetTime);
}

/**
 * @param timestamps assumes sorted in ascending order
 * @param targetTime the timestamp to find the rightmost index for
 * @returns index of the last timestamp that is <= targetTime, or timestamps.length - 1 if all timestamps are <= targetTime
 */
export function bisect_right(timestamps: number[], targetTime: number): number {
	let left = 0;
	let right = timestamps.length - 1;
	let result = timestamps.length - 1;

	while (left <= right) {
		const mid = Math.floor((left + right) / 2);
		if (timestamps[mid] <= targetTime) {
			result = mid;
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return result;
}
