import { useRef, useCallback } from 'react';

const INITIAL_CAPACITY = 1_000_000; 

type SeriesStore = {
  timestamps: Float64Array;         
  series: Record<string, Float32Array>; 
  cursor: number;                   
  capacity: number;                 
};

export const useTimeSeriesStore = (signalNames: string[]) => {
  const store = useRef<SeriesStore | null>(null);

  if (!store.current) {
    store.current = {
      timestamps: new Float64Array(INITIAL_CAPACITY),
      series: {},
      cursor: 0,
      capacity: INITIAL_CAPACITY,
    };

    signalNames.forEach(name => {
      store.current!.series[name] = new Float32Array(INITIAL_CAPACITY);
    });
  }

  // i don't know if this actually works
  // TODO: test this
  const ensureCapacity = (requiredDetails: number) => {
    const s = store.current!;
    if (s.cursor + requiredDetails >= s.capacity) {
      const newCapacity = s.capacity * 2;
      console.log(`[Memory] Resizing buffer: ${s.capacity} -> ${newCapacity}`);

      const newTime = new Float64Array(newCapacity);
      newTime.set(s.timestamps); 
      s.timestamps = newTime;

      Object.keys(s.series).forEach(key => {
        const newVal = new Float32Array(newCapacity);
        newVal.set(s.series[key]);
        s.series[key] = newVal;
      });

      s.capacity = newCapacity;
    }
  };

  const addPoint = useCallback((time: number, values: Record<string, number>) => {
    const s = store.current!;
    
    if (s.cursor >= s.capacity) {
      ensureCapacity(1);
    }

    // Direct write (O(1))
    s.timestamps[s.cursor] = time;
    
    Object.entries(values).forEach(([key, val]) => {
      // Ensure the series array exists (handling dynamic signal addition)
      if (!s.series[key]) {
        s.series[key] = new Float32Array(s.capacity);
        // Fill previous slots with NaN or 0 if strictly needed, 
        // though usually 0 is fine for Float32
      }
      s.series[key][s.cursor] = val;
    });

    s.cursor++;
  }, []);
  

  // reads from the store, returns a view of the data
  const getSnapshot = useCallback(() => {
    const s = store.current!;
    return {
      timestamps: s.timestamps.subarray(0, s.cursor),
      series: Object.keys(s.series).reduce((acc, key) => {
        // create a view of the series array (don't copy, v fast)
        acc[key] = s.series[key].subarray(0, s.cursor);
        return acc;
      }, {} as Record<string, Float32Array>)
    };
  }, []);

  return {
    addPoint,
    getSnapshot,
    debugInfo: () => ({ cursor: store.current?.cursor, cap: store.current?.capacity })
  };
};