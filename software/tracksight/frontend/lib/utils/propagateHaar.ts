const propagateHaar = (
    buffer: Array<{ timestamp: number; value: number } | null>,
    level: number,
    timestamp: number,
    value: number,
    callback: (level: number, intervalMs: number, timestamp: number, value: number) => void,
    maxLevels: number
): void => {
  if (level >= maxLevels - 1) return;

  const pending = buffer[level];
  if (!pending) {
    buffer[level] = { timestamp, value };
    return;
  }

  const avgValue = (pending.value + value) / 2;
  const avgTimestamp = Math.round((pending.timestamp + timestamp) / 2);

  buffer[level] = null;

  callback(level + 1, 2**level, avgTimestamp, avgValue);

  propagateHaar(buffer, level + 1, avgTimestamp, avgValue, callback, maxLevels);
}

export default propagateHaar;
