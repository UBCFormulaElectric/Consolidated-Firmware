const MAX_ARRAY_SIZE = 1 * 1024 * 1024 * 10;
const INITIAL_ARRAY_SIZE = 1024 * 16;
const ARRAY_GROWTH_FACTOR = 2;

// NOTE(evan): Javascript's default array only supports ~285m elements before we get out of memory errors
//             the `ArrayBuffer` with the maxByteLength parameter even though its technically a "resizable" array
//             the engine seems to allocate the full size up front leading to large page load memory usage.
//
//             Using typed arrays with manual resizing should get around this limitation at the cost of
//             some convenience (and v8 optimizations for standard arrays). Typed arrays should also provide
//             slightly better memory usage as well.
//
//             If we end up only needing <100m elements we can safely switch back to the standard array to
//             get some of those v8 optimizations back.
class AutoResizingTypedArray {
  private array: Float64Array;
  private _length: number;

  constructor() {
    this.array = new Float64Array(INITIAL_ARRAY_SIZE);
    this._length = 0;
  }

  push(value: number) {
    if (this.length >= this.array.length) {
      this.resize();
    }

    this.array[this.length] = value;
    this._length += 1;

    if (this.length % 100_000 === 0)
      console.log("%c[TypedArray]: Current length:", "color: lightgreen;", this.length);
  }

  get(index: number): number {
    if (index < 0 || index >= this.length) {
      throw new Error("Index out of bounds");
    }

    return this.array[index];
  }

  get length(): number {
    return this._length;
  }

  private resize() {
    const newSize = Math.min(this.array.length * ARRAY_GROWTH_FACTOR, MAX_ARRAY_SIZE);

    if (newSize <= this.array.length) {
      throw new Error("Exceeded maximum array size");
    }

    const newArray = new Float64Array(newSize);
    newArray.set(this.array);
    this.array = newArray;
  }
}

export default AutoResizingTypedArray;
