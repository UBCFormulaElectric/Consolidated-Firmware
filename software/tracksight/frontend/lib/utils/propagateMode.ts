type ModeLodBuffer = { counts: Record<number, number>; timestamp: number } | null;

const propagateMode = (
  buffer: ModeLodBuffer[],
  level: number,
  timestamp: number,
  incomingCounts: Record<number, number>,
  callback: (level: number, intervalMs: number, timestamp: number, value: number) => void,
  maxLevels: number
): void => {
  if (level >= maxLevels - 1) return;

  const pending = buffer[level];
  if (!pending) {
    buffer[level] = { counts: incomingCounts, timestamp };
    return;
  }

  const merged: Record<number, number> = { ...pending.counts };
  for (const key in incomingCounts) {
    merged[key] = (merged[key] || 0) + incomingCounts[key];
  }

  let modeValue = 0;
  let maxCount = 0;
  for (const key in merged) {
    if (merged[key] > maxCount) {
      maxCount = merged[key];
      modeValue = Number(key);
    }
  }

  const avgTimestamp = Math.round((pending.timestamp + timestamp) / 2);
  buffer[level] = null;

  callback(level + 1, 2 ** (level + 1), avgTimestamp, modeValue);

  propagateMode(buffer, level + 1, avgTimestamp, merged, callback, maxLevels);
};

export type { ModeLodBuffer };
export default propagateMode;
