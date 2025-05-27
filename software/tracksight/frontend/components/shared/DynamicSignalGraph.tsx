'use client';

import React, { useEffect, useState, useCallback } from 'react';
import { SignalProvider, useSignals, SignalType } from '@/lib/contexts/SignalContext';
import { PlusButton } from '@/components/shared/PlusButton';

// Import the GraphInstance from Numerical component
import { AreaChart, Area, XAxis, YAxis, Tooltip } from 'recharts';
import { usePausePlay } from '@/components/shared/pause-play-control';

// Define a palette for signal lines
const signalColors = [
  '#ff4d4f', '#ffa940', '#36cfc9', '#597ef7', '#73d13d',
  '#ff85c0', '#9254de', '#ffc069', '#69c0ff', '#ffec3d'
];

// Colors for enumeration states
const stateColors = [
  '#FF3B2F',
  '#FFCC02',
  '#FF9500',
  '#35C759',
  '#007AFF',
  '#5856D6',
  '#AF52DE',
  '#FF2D55',
];

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

// Numerical Graph Component - supports multiple signals like the original
const NumericalGraphComponent: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const { isPaused } = usePausePlay();
  const {
    availableSignals,
    activeSignals,
    numericalData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isNumericalSignal
  } = useSignals();
  
  const [selectedSignal, setSelectedSignal] = useState<string>('');
  const [chartHeight, setChartHeight] = useState<number>(256);
  const [scaleFactor, setScaleFactor] = useState<number>(100);

  // Search state for inline searchbar
  const [searchTerm, setSearchTerm] = useState('');
  const [isSearchOpen, setIsSearchOpen] = useState(false);

  // Subscribe to the initial signal when component mounts
  useEffect(() => {
    if (signalName && !activeSignals.includes(signalName)) {
      subscribeToSignal(signalName, SignalType.Numerical);
    }
  }, [signalName, activeSignals, subscribeToSignal]);

  // Aggregate numerical data by timestamp for all active numerical signals
  const chartData = React.useMemo(() => {
    const filtered = numericalData
      .filter(d => activeSignals.includes(d.name as string) && isNumericalSignal(d.name as string));
    if (!filtered.length) return [];
    
    // Group timestamps into time windows to handle slight time differences
    const timeWindow = 100; // 100ms window
    const map: Record<number, any> = {};
    
    filtered.forEach(d => {
      const t = typeof d.time === 'number' ? d.time : new Date(d.time).getTime();
      // Round timestamp to nearest time window
      const roundedTime = Math.floor(t / timeWindow) * timeWindow;
      if (!map[roundedTime]) map[roundedTime] = { time: roundedTime };
      map[roundedTime][d.name as string] = d.value;
    });
    
    return Object.values(map).sort((a, b) => a.time - b.time);
  }, [numericalData, activeSignals, isNumericalSignal]);

  // Compute which active signals are numerical
  const numericalSignals = React.useMemo(
    () => activeSignals.filter(sig => isNumericalSignal(sig)),
    [activeSignals, isNumericalSignal]
  );

  // Options for subscription dropdown - filter out already subscribed signals and non-numerical signals
  const availableOptions = React.useMemo(
    () => availableSignals.filter(s => 
      !activeSignals.includes(s.name) && isNumericalSignal(s.name)
    ),
    [availableSignals, activeSignals, isNumericalSignal]
  );

  // Filtered signals based on search term
  const filteredSignals = React.useMemo(() => {
    if (!searchTerm) return availableOptions;
    return availableOptions.filter(signal =>
      signal.name.toLowerCase().includes(searchTerm.toLowerCase())
    );
  }, [availableOptions, searchTerm]);

  const pixelPerDataPoint = 50;
  const chartWidth = Math.max(chartData.length * pixelPerDataPoint, 100);
  const finalChartWidth = chartWidth * (scaleFactor / 100);

  const handleSignalSelect = useCallback((signalName: string) => {
    subscribeToSignal(signalName, SignalType.Numerical);
    setSearchTerm('');
    setIsSearchOpen(false);
  }, [subscribeToSignal]);

  const handleUnsubscribe = useCallback(
    (sig: string) => unsubscribeFromSignal(sig),
    [unsubscribeFromSignal]
  );

  const handleDeleteGraph = useCallback(() => {
    // Unsubscribe from all numerical signals when deleting the graph
    numericalSignals.forEach(sig => unsubscribeFromSignal(sig));
    onDelete();
  }, [numericalSignals, unsubscribeFromSignal, onDelete]);

  return (
    <div className="mb-6 p-4 relative">
      {/* Pause indicator */}
      {isPaused && (
        <div className="absolute top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10">
          PAUSED
        </div>
      )}
      
      <div className="flex items-center gap-2 mb-4">
        <h3 className="font-semibold">Numerical Graph</h3>
        <button
          onClick={handleDeleteGraph}
          className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
          title="Remove this graph and unsubscribe from all signals"
        >
          ×
        </button>
      </div>

      {/* Controls for sizing */}
      <div className="flex flex-wrap gap-4 mb-4">
        <div className="flex flex-col">
          <label className="text-sm">Vertical Scale: {chartHeight}px</label>
          <input
            type="range"
            min={100}
            max={600}
            step={50}
            value={chartHeight}
            onChange={e => setChartHeight(Number(e.target.value))}
          />
        </div>
        <div className="flex flex-col">
          <label className="text-sm">Horizontal Scale: {scaleFactor}%</label>
          <input
            type="range"
            min={1}
            max={1000}
            value={scaleFactor}
            onChange={e => setScaleFactor(Number(e.target.value))}
          />
        </div>
      </div>

      {/* Subscribed signals and add signal searchbar */}
      <div className="flex flex-wrap items-center gap-3 mb-4">
        {/* Show subscribed signals as removable pills with colored dots */}
        {numericalSignals.map((sig, idx) => {
          const signalMeta = availableSignals.find(s => s.name === sig);
          const color = signalColors[idx % signalColors.length];
          // Create a fainter version of the signal color for background
          const faintColor = color + '20'; // Add alpha for transparency
          
          return (
            <button
              key={sig}
              onClick={() => handleUnsubscribe(sig)}
              className="flex items-center gap-2 px-3 py-1.5 h-8 rounded-full border border-gray-200 dark:border-gray-600 hover:opacity-80 transition-opacity cursor-pointer"
              style={{ backgroundColor: faintColor }}
              title={`Click to unsubscribe from ${sig}`}
            >
              {/* Color dot */}
              <div 
                className="w-3 h-3 rounded-full flex-shrink-0"
                style={{ backgroundColor: color }}
              />
              {/* Signal name and unit */}
              <div className="flex items-center gap-1 text-sm">
                <span className="font-medium text-gray-900 dark:text-gray-100">{sig}</span>
                {signalMeta?.unit && (
                  <span className="text-gray-500 dark:text-gray-400">({signalMeta.unit})</span>
                )}
              </div>
              {/* Remove indicator */}
              <span className="text-red-600 hover:text-red-800 ml-1 flex-shrink-0 font-semibold">
                ×
              </span>
            </button>
          );
        })}
        
        {/* Add signal button - only show if there are available signals */}
        {availableOptions.length > 0 && (
          <div className="relative">
            <PlusButton
              onClick={() => setIsSearchOpen(!isSearchOpen)}
              variant="rowSide"
            />
            
            {isSearchOpen && (
              <div className="absolute top-full left-0 mt-1 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-600 rounded shadow-lg w-64 p-2 z-10">
                <input
                  type="text"
                  placeholder="Search numerical signals..."
                  className="w-full mb-2 px-2 py-1 border rounded bg-white dark:bg-gray-700 text-black dark:text-white"
                  value={searchTerm}
                  onChange={e => setSearchTerm(e.target.value)}
                  autoFocus
                />
                <div className="max-h-32 overflow-y-auto">
                  {isLoadingSignals ? (
                    <div className="px-2 py-1 text-gray-500">Loading signals...</div>
                  ) : filteredSignals.length === 0 ? (
                    <div className="px-2 py-1 text-gray-500">No signals found</div>
                  ) : (
                    filteredSignals.map(signal => (
                      <div
                        key={signal.name}
                        onClick={() => handleSignalSelect(signal.name)}
                        className="px-2 py-1 hover:bg-gray-100 dark:hover:bg-gray-700 cursor-pointer text-black dark:text-white"
                      >
                        <div className="font-medium">{signal.name}</div>
                        <div className="text-xs text-gray-500 dark:text-gray-400">{signal.unit}</div>
                      </div>
                    ))
                  )}
                </div>
              </div>
            )}
          </div>
        )}
      </div>

      {/* Chart area */}
      {chartData.length === 0 ? (
        <div
          className="w-full flex items-center justify-center text-gray-500"
          style={{ height: chartHeight }}
        >
          No numerical data to display
        </div>
      ) : (
        <div
          style={{
            width: finalChartWidth,
            height: chartHeight,
            transition: 'width 100ms ease-out'
          }}
        >
          <AreaChart width={finalChartWidth} height={chartHeight} data={chartData}>
            <XAxis
              dataKey="time"
              tickFormatter={t => new Date(t).toLocaleTimeString()}
              interval={Math.ceil(chartData.length / 10)}
            />
            <YAxis domain={[0, 'auto']} />
            <Tooltip 
              isAnimationActive={false} 
              animationDuration={0}
              labelFormatter={(time) => new Date(time).toLocaleString()}
            />
            {numericalSignals.map((sig, idx) => (
              <Area
                key={sig}
                type="monotone"
                dataKey={sig}
                stroke={signalColors[idx % signalColors.length]}
                fill={signalColors[idx % signalColors.length]}
                strokeWidth={1}
                isAnimationActive={false}
                animationDuration={0}
              />
            ))}
          </AreaChart>
        </div>
      )}
    </div>
  );
};

// Enumeration Graph Component
const EnumerationGraphComponent: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const { isPaused } = usePausePlay();
  const {
    enumData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isEnumSignal,
    getEnumValues,
    mapEnumValue,
  } = useSignals();

  const [pausedTime, setPausedTime] = useState<number | null>(null);

  // Subscribe to the signal when component mounts
  useEffect(() => {
    if (signalName) {
      subscribeToSignal(signalName, SignalType.Enumeration);
    }
  }, [signalName, subscribeToSignal]);

  // Update paused time when pause state changes
  useEffect(() => {
    if (isPaused && pausedTime === null) {
      setPausedTime(Date.now());
    } else if (!isPaused) {
      setPausedTime(null);
    }
  }, [isPaused, pausedTime]);

  // Build history for this specific signal
  const history = React.useMemo(() => {
    const raw = enumData
      .filter(dp => dp.name === signalName && dp.value != null)
      .map(dp => ({
        state: String(dp.value),
        startTime: typeof dp.time === 'number' ? dp.time : new Date(dp.time).getTime(),
      }));

    const items: { state: string; startTime: number }[] = [];
    raw.forEach(pt => {
      const last = items[items.length - 1];
      if (!last || last.state !== pt.state) {
        items.push(pt);
      }
    });
    return items;
  }, [enumData, signalName]);

  // Compute bars for timeline visualization
  const computeBars = React.useCallback((history: { state: string; startTime: number }[]) => {
    const timeWindowMs = 5_000;
    const containerWidth = typeof window !== 'undefined' ? window.innerWidth : 1000;
    const pxPerMs = containerWidth / timeWindowMs;
    const now = isPaused && pausedTime ? pausedTime : Date.now();

    const rawBars = history
      .map((item, i) => {
        const next = history[i + 1];
        const end = next ? next.startTime : now;
        return {
          state: item.state,
          startTime: item.startTime,
          width: (end - item.startTime) * pxPerMs,
        };
      })
      .filter(bar => bar.width > 0);

    let offset = 0;
    return rawBars.map(bar => {
      const barWithOffset = {
        ...bar,
        xOffset: offset,
      };
      offset += bar.width;
      return barWithOffset;
    });
  }, [isPaused, pausedTime]);

  const bars = computeBars(history);
  const enumVals = getEnumValues(signalName);
  const lastCode = history.length > 0 ? history[history.length - 1].state : undefined;
  const lastLabel = lastCode != null ? mapEnumValue(signalName, lastCode) ?? lastCode : undefined;

  const handleUnsubscribe = useCallback(() => {
    unsubscribeFromSignal(signalName);
    onDelete();
  }, [signalName, unsubscribeFromSignal, onDelete]);

  return (
    <div className="mb-6 p-4 relative">
      {/* Pause indicator */}
      {isPaused && (
        <div className="absolute top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10">
          PAUSED
        </div>
      )}
      
      <div className="flex items-center gap-2 mb-4">
        <h3 className="font-semibold">Enumeration: {signalName}</h3>
        <button
          onClick={handleUnsubscribe}
          className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
          title="Remove this graph and unsubscribe from signal"
        >
          ×
        </button>
      </div>

      {/* Legend */}
      <div className="flex flex-wrap gap-4 text-xs mb-4">
        {enumVals.map((stateLabel, i) => {
          const color = stateColors[i % stateColors.length];
          const isActive = stateLabel === lastLabel;
          return (
            <div key={stateLabel} className="inline-flex items-center gap-1">
              <span className="w-3 h-3 block rounded" style={{ backgroundColor: color }} />
              <span className={`${isActive ? 'font-bold text-black' : 'text-gray-500'} transition`}>
                {stateLabel}
              </span>
            </div>
          );
        })}
      </div>

      {/* Timeline blocks */}
      <div className="inline-block">
        <div className="relative">
          <div className="h-6 flex flex-row flex-nowrap">
            {bars.map((bar, idx) => {
              const label = mapEnumValue(signalName, bar.state) ?? bar.state;
              const enumIndex = enumVals.indexOf(label);
              const historyIndex = history.findIndex(item => (mapEnumValue(signalName, item.state) ?? item.state) === label);
              const idxForColor = enumIndex >= 0 ? enumIndex : historyIndex;
              const color = stateColors[idxForColor % stateColors.length];
              return (
                <div
                  key={idx}
                  className="h-6 shrink-0"
                  style={{ width: `${bar.width}px`, backgroundColor: color }}
                  title={`${label} @ ${new Date(bar.startTime).toLocaleTimeString()}`}
                />
              );
            })}
          </div>
          <div className="absolute top-full left-0 h-4">
            {bars.map((bar, idx) => (
              <div
                key={idx}
                className="absolute whitespace-nowrap text-xs"
                style={{ left: `${bar.xOffset}px` }}
              >
                {new Date(bar.startTime).toLocaleTimeString()}
              </div>
            ))}
          </div>
        </div>
      </div>

      {history.length === 0 && (
        <div className="text-gray-500 mt-4">
          Waiting for enumeration data from {signalName}...
        </div>
      )}
    </div>
  );
};

// Main Dynamic Component that decides which type to render
const DynamicSignalGraphInner: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const { isEnumSignal } = useSignals();
  
  // Determine signal type and render appropriate component
  const isEnum = isEnumSignal(signalName);
  
  if (isEnum) {
    return <EnumerationGraphComponent signalName={signalName} onDelete={onDelete} />;
  } else {
    return <NumericalGraphComponent signalName={signalName} onDelete={onDelete} />;
  }
};

// Export the main component wrapped in SignalProvider
const DynamicSignalGraph: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  return (
    <SignalProvider>
      <DynamicSignalGraphInner signalName={signalName} onDelete={onDelete} />
    </SignalProvider>
  );
};

export default DynamicSignalGraph;
