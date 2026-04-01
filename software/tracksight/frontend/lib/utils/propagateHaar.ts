type HaarLodBuffer = { timestamp: number; value: number }[] | null;

const propagateHaar = (
    buffer: HaarLodBuffer[],
    level: number,
    timestamp: number,
    value: number,
    callback: (level: number, intervalMs: number, timestamp: number, value: number) => void,
    maxLevels: number
): void => {
  if (level >= maxLevels - 1) return;

  const pending = buffer[level];
  if (!pending) {
    buffer[level] = [{ timestamp, value }];
    return;
  }

  const avgValue = (pending[0].value + value) / 2;
  const avgTimestamp = Math.round((pending[0].timestamp + timestamp) / 2);

  buffer[level] = null;

  callback(level + 1, 2**(level+1), avgTimestamp, avgValue);

  propagateHaar(buffer, level + 1, avgTimestamp, avgValue, callback, maxLevels);
}

export type { HaarLodBuffer };
export default propagateHaar;
