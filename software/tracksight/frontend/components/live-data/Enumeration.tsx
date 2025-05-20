'use client'
import React, { useState, useEffect, useCallback, useMemo } from 'react'
import { useSignals } from '@/lib/contexts/SignalContext'

interface StateHistoryItem {
	state: string | number
	startTime: number
}

interface StateBar {
	state: string | number
	startTime: number
	width: number
}

// Colors for the states, mapping to indices
const stateColors: string[] = [
	'#FF3B2F',
	'#FFCC02',
	'#FF9500',
	'#35C759',
	'#007AFF',
	'#5856D6',
	'#AF52DE',
	'#FF2D55'
]

export default function EnumerationGraph({
	signalName
}: {
	signalName: string
}) {
	const { data, activeSignals, subscribeToSignal, unsubscribeFromSignal, currentTime } = useSignals()
	const [stateHistory, setStateHistory] = useState<StateHistoryItem[]>([])
	
	// Subscribe to signal if it's not already active, and unsubscribe on unmount
	useEffect(() => {
		console.log(`EnumerationGraph - Checking subscription for ${signalName}`);
		
		const normalizedSignalName = signalName.trim();
		const isActive = activeSignals.some(
			signal => signal.toLowerCase() === normalizedSignalName.toLowerCase()
		);
		
		if (!isActive) {
			console.log(`EnumerationGraph - Subscribing to ${signalName} as it's not active yet`);
			subscribeToSignal(signalName);
		} else {
			console.log(`EnumerationGraph - ${signalName} is already active, no need to subscribe`);
		}
		
		// Cleanup: unsubscribe when component unmounts
		return () => {
			console.log(`EnumerationGraph - Cleaning up, unsubscribing from ${signalName}`);
			unsubscribeFromSignal(signalName);
		};
	}, [signalName, activeSignals, subscribeToSignal, unsubscribeFromSignal]);
	
	// Debug output
	useEffect(() => {
		console.log(`EnumerationGraph - Data for ${signalName}:`, 
		  data.length > 0 ? data.slice(-1)[0][signalName] : "No data");
	}, [data, signalName]);
	
	// Find all unique enum values from the data
	const enumValues = useMemo(() => {
		const uniqueValues = new Set<string | number>();
		
		// Extract all values for this signal from the data
		data.forEach(item => {
			const value = item[signalName];
			if (value !== undefined) {
				uniqueValues.add(value);
			}
		});
		
		return Array.from(uniqueValues).map(String);
	}, [data, signalName]);
	
	// Get current enum value from latest data point
	const currentEnumVal = useMemo(() => {
		if (data.length > 0) {
			const latestValue = data[data.length - 1][signalName];
			return latestValue !== undefined ? String(latestValue) : null;
		}
		return null;
	}, [data, signalName]);

	// Update state history when currentEnumVal changes
	useEffect(() => {
		if (currentEnumVal !== null) {
			setStateHistory((prevHistory) => {
				const lastState = prevHistory[prevHistory.length - 1];
				if (!lastState || lastState.state !== currentEnumVal) {
					// If state changed, add new state with current time
					return [...prevHistory, { state: currentEnumVal, startTime: currentTime }];
				}
				// State didn't change, return previous history
				return prevHistory;
			});
		}
	}, [currentEnumVal, currentTime]);

	// Visualization parameters
	const timeWindow_ms = 10 * 1000  // 10 seconds window
	const containerWidth = typeof window !== 'undefined' ? window.innerWidth - 100 : 1000;
	const pixelsPerMs = containerWidth / timeWindow_ms;
	
	// Create the state bars for visualization
	const stateBars: StateBar[] = stateHistory.map((state, index) => {
		const nextState = stateHistory[index + 1];
		const endTime = nextState ? nextState.startTime : currentTime;
		const width = (endTime - state.startTime) * pixelsPerMs;
		
		// Limit width to the container width (for very old states)
		return {
			...state,
			width: Math.min(width, containerWidth),
		};
	})
	.filter(bar => {
		// Only include bars that are within the time window
		return (currentTime - bar.startTime) <= timeWindow_ms;
	});

	// Get color for a state
	const getStateColor = useCallback((state: string | number): string => {
		const stateStr = String(state);
		// Try to get a consistent color for each state
		const hashCode = stateStr.split('').reduce((acc, char) => {
			return acc + char.charCodeAt(0);
		}, 0);
		return stateColors[hashCode % stateColors.length];
	}, []);

	// Check if a state is the current active state
	const isActive = useCallback(
		(state: string | number): boolean => {
			return stateHistory.length !== 0 && String(stateHistory[stateHistory.length - 1].state) === String(state);
		},
		[stateHistory]
	);
	
	// If no data yet, show a loading state
	if (data.length === 0 || enumValues.length === 0) {
		return (
			<div className="flex flex-col gap-2">
				<div className='left-4 inline-flex gap-5 items-center sticky'>
					<div className='font-bold my-1 px-2 py-1 text-white bg-blue-500 rounded-full inline-block text-sm'>
						{signalName}
					</div>
					<div className='text-gray-500'>Waiting for enum data...</div>
				</div>
				<div className='h-6 bg-gray-100 min-w-screen inline-block'></div>
			</div>
		);
	}

	return (
		<div className="flex flex-col gap-2">
			{/* Signal Name and Legend */}
			<div className='left-4 inline-flex gap-5 items-center sticky'>
				<div className='font-bold my-1 px-2 py-1 text-white bg-blue-500 rounded-full inline-block text-sm '>
					{signalName}
				</div>
				{/* Legend */}
				<div className='my-1 text-xs flex gap-4'>
					{enumValues.map((state, index) => (
						<div key={index} className='flex items-center mb-1'>
							<span
								className='w-3 h-3 inline-block mr-2'
								style={{ backgroundColor: getStateColor(state) }}
							/>
							<span
								data-active={isActive(state)}
								className='text-gray-500 font-normal data-[active=true]:text-black data-[active=true]:font-bold transition-all duration-300'>
								{state}
							</span>
						</div>
					))}
				</div>
			</div>
			{/* Graph */}
			<div className='h-6 bg-gray-100 min-w-screen inline-block'>
				<div className="flex flex-row flex-nowrap">
					{stateBars.map((bar, index) => {
						return (
							<div
								key={index}
								className='h-6 shrink-0'
								style={{
									width: `${bar.width}px`,
									backgroundColor: getStateColor(bar.state),
								}}
								title={`${bar.state} (${new Date(
									bar.startTime
								).toLocaleTimeString()})`} />
						)
					})}
				</div>
			</div>
		</div>
	)
}
