export class NumericRingBuffer {
  readonly capacity: number;
  readonly timestamps: Float64Array;
  readonly values: Float32Array;

  private head = 0;
  private size = 0;

  constructor(capacity: number) {
    if (!Number.isFinite(capacity) || capacity <= 0) {
      throw new Error(`Invalid ring buffer capacity: ${capacity}`);
    }
    this.capacity = Math.floor(capacity);
    this.timestamps = new Float64Array(this.capacity);
    this.values = new Float32Array(this.capacity);
  }

  get length() {
    return this.size;
  }

   // appends a point (timestampMs, value)  & overwrite the oldest when full
  push(timestampMs: number, value: number) {
    const i = this.head;
    this.timestamps[i] = timestampMs;
    this.values[i] = value;

    this.head = (i + 1) % this.capacity;
    if (this.size < this.capacity) {
      this.size += 1;
    }
  }

  /**
   * Logical index 0..length-1 (oldest..newest) -> physical index in typed arrays.
   */
  private logicalToPhysicalIndex(logicalIndex: number) {
    const n = this.size;
    if (logicalIndex < 0 || logicalIndex >= n) {
      return -1;
    }
    // oldest element lives at (head - size)
    const start = (this.head - n + this.capacity) % this.capacity;
    return (start + logicalIndex) % this.capacity;
  }

  timestampAt(logicalIndex: number): number {
    const p = this.logicalToPhysicalIndex(logicalIndex);
    return p < 0 ? NaN : this.timestamps[p];
  }

  valueAt(logicalIndex: number): number {
    const p = this.logicalToPhysicalIndex(logicalIndex);
    return p < 0 ? NaN : this.values[p];
  }
}

function isNumericIndexProp(prop: PropertyKey): prop is string {
  return typeof prop === "string" && /^[0-9]+$/.test(prop);
}

/**
 * Creates a cheap array-like view over the ring buffer that:
 * - exposes a dynamic `length`
 * - supports `arr[i]` indexing (0..length-1)
 * - supports `for..of` iteration
 *
 * This is intentionally minimalist: it exists to feed the existing CanvasChart
 * and render logic without forcing React rerenders or data copies.
 */
export function makeRingArrayView(
  ring: NumericRingBuffer,
  which: "timestamps" | "values"
): any {
  return new Proxy(
    {},
    {
      get(_target, prop) {
        if (prop === "length") {
          return ring.length;
        }

        if (prop === Symbol.iterator) {
          return function* iterator() {
            const n = ring.length;
            for (let i = 0; i < n; i++) {
              yield which === "timestamps" ? ring.timestampAt(i) : ring.valueAt(i);
            }
          };
        }

        if (prop === "at") {
          return (index: number) => {
            if (index < 0) index = ring.length + index;
            return which === "timestamps" ? ring.timestampAt(index) : ring.valueAt(index);
          };
        }

        if (isNumericIndexProp(prop)) {
          const i = Number(prop);
          if (which === "timestamps") {
            return ring.timestampAt(i);
          }
          return ring.valueAt(i);
        }

        return undefined;
      },

      // Make `prop in view` behave a bit more sanely
      has(_target, prop) {
        if (prop === "length" || prop === Symbol.iterator || prop === "at") return true;
        if (isNumericIndexProp(prop)) {
          const i = Number(prop);
          return i >= 0 && i < ring.length;
        }
        return false;
      },
    }
  );
}
