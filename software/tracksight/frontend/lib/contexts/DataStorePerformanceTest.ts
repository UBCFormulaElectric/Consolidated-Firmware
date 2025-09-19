// Performance test file to demonstrate the improvements
import { SignalDataStore, DataPoint } from './SignalConfig';

// Test the performance improvements
export function performanceTest() {
    console.log('Testing optimized data storage performance...');

    const store = new SignalDataStore();
    const startTime = performance.now();

    // Add 8000 data points (your current problem size)
    for (let i = 0; i < 8000; i++) {
        const point: DataPoint = {
            time: Date.now() + i * 100, // 100ms intervals
            name: `signal_${i % 10}`, // 10 different signals
            value: Math.random() * 100
        };
        store.addDataPoint(point);
    }

    const addTime = performance.now() - startTime;
    console.log(`Added 8000 points in ${addTime.toFixed(2)}ms`);

    // Test time range queries (O(log n) instead of O(n))
    const queryStart = performance.now();
    const now = Date.now();
    const recentData = store.getDataInTimeRange(now, now + 500000); // Last 500 seconds
    const queryTime = performance.now() - queryStart;

    console.log(`Time range query (${recentData.length} points) in ${queryTime.toFixed(2)}ms`);

    // Test signal-specific queries
    const signalQueryStart = performance.now();
    const signal0Data = store.getSignalData('signal_0');
    const signalQueryTime = performance.now() - signalQueryStart;

    console.log(`Signal-specific query (${signal0Data.length} points) in ${signalQueryTime.toFixed(2)}ms`);

    console.log(`Total data points in store: ${store.getDataCount()}`);

    return {
        addTime,
        queryTime,
        signalQueryTime,
        totalPoints: store.getDataCount(),
        recentDataPoints: recentData.length,
        signalDataPoints: signal0Data.length
    };
}

// Compare with old array-based approach
export function compareWithArrays() {
    console.log('Comparing with old array-based approach...');

    const arrayData: DataPoint[] = [];
    const startTime = performance.now();

    // Add 8000 data points using old approach
    for (let i = 0; i < 8000; i++) {
        const point: DataPoint = {
            time: Date.now() + i * 100,
            name: `signal_${i % 10}`,
            value: Math.random() * 100
        };
        arrayData.push(point);
    }

    const addTime = performance.now() - startTime;
    console.log(`Array approach - Added 8000 points in ${addTime.toFixed(2)}ms`);

    // Test time range queries with arrays (O(n))
    const queryStart = performance.now();
    const now = Date.now();
    const recentData = arrayData.filter(point => {
        const time = typeof point.time === 'string' ? parseFloat(point.time) : point.time;
        return time >= now && time <= now + 500000;
    });
    const queryTime = performance.now() - queryStart;

    console.log(`Array time range query (${recentData.length} points) in ${queryTime.toFixed(2)}ms`);

    // Test signal-specific queries with arrays (O(n))
    const signalQueryStart = performance.now();
    const signal0Data = arrayData.filter(point => point.name === 'signal_0');
    const signalQueryTime = performance.now() - signalQueryStart;

    console.log(`Array signal-specific query (${signal0Data.length} points) in ${signalQueryTime.toFixed(2)}ms`);

    return {
        addTime,
        queryTime,
        signalQueryTime,
        totalPoints: arrayData.length
    };
}
