'use client'
import React, { useState, useEffect, useCallback } from 'react'

interface StateHistoryItem {
    state: string
    startTime: number
}

interface StateBar {
    state: string
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
    // TODO add a few more colours
]

function useSignalMetadata(signalName: string) {
    // todo logic to fetch signalName
    return {
        enumVals: [
            'VC_INIT_STATE',
            'VC_PCM_STATE',
            'VC_HV_STATE',
            'VC_DRIVE_STATE',
            'VC_VD_STATE',
        ]
    }
}

function useSignal(signalName: string) {
    const enumStates = [
        'VC_INIT_STATE',
        'VC_PCM_STATE',
        'VC_HV_STATE',
        'VC_DRIVE_STATE',
        'VC_VD_STATE',
    ]
    useEffect(() => {
        const stateInterval = setInterval(() => {
            setCurrentEnumVal(() => {
                const nextIndex = Math.floor(Math.random() * enumStates.length)
                return enumStates[nextIndex]
            })
        }, 4000)
        return () => {
            clearInterval(stateInterval)
        }
    }, [])

    const [currentEnumVal, setCurrentEnumVal] = useState(enumStates[0])
    return { currentEnumVal }
}

export default function EnumerationGraph({
    currentTime,
    signalName
}: {
    currentTime: number
    signalName: string
}) {
    const [stateHistory, setStateHistory] = useState<StateHistoryItem[]>([])
    const [containerWidth, setContainerWidth] = useState(800) // Default width
    const { enumVals } = useSignalMetadata(signalName)
    const { currentEnumVal } = useSignal(signalName)

    // Define the total time window displayed on the graph (e.g., last 60 seconds)
    const timeWindow_ms = 5 * 1000

    // Add effect to get window width after component mounts
    useEffect(() => {
        // This code only runs in the browser
        function handleResize() {
            setContainerWidth(window.innerWidth)
        }
        
        // Set initial width
        handleResize()
        
        // Add resize listener
        window.addEventListener('resize', handleResize)
        
        // Clean up
        return () => window.removeEventListener('resize', handleResize)
    }, [])

    useEffect(() => {
        // Update state history when currentState changes
        setStateHistory((prevHistory) => {
            const lastState = prevHistory[prevHistory.length - 1]
            if (!lastState || lastState.state !== currentEnumVal) {
                // If state changed, add new state with current time
                return [...prevHistory, { state: currentEnumVal, startTime: currentTime }]
            }
            // State didn't change, return previous history
            return prevHistory
        })
    }, [currentEnumVal])

    // Calculate pixel values based on time
    const pixelsPerMs = containerWidth / timeWindow_ms
    const stateBars: StateBar[] = stateHistory.map((state, index) => {
        const nextState = stateHistory[index + 1]
        return {
            ...state,
            width: ((nextState ? nextState.startTime : currentTime) - state.startTime) * pixelsPerMs,
        }
    })

    const getStateColor = useCallback((state: string): string => {
        const index = enumVals.indexOf(state)
        return stateColors[index % stateColors.length] // Wrap around if more states
    }, [])

    const isActive = useCallback(
        (state: string): Boolean => {
            return stateBars.length != 0 && stateBars.at(-1)!.state === state
        },
        [stateBars]
    )

    return (
        <>
            {/* Signal Name and Legend */}
            <div className='left-4 inline-flex gap-5 items-center sticky'>
                <div className='font-bold my-1 px-2 py-1 text-white bg-blue-500 rounded-full inline-block text-sm '>
                    {signalName}
                </div>
                {/* Legend */}
                <div className='my-1 text-xs flex gap-4'>
                    {enumVals.map((state, index) => (
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
        </>
    )
}