'use client'
import React, { useState, useEffect, useCallback } from 'react'
import { useSignals, SignalType } from '@/lib/contexts/SignalContext'

const NumberOut: React.FC = () => {
  const {
    data,
    activeSignals,
    availableSignals,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isEnumSignal
  } = useSignals();
  
  const [selectedSignal, setSelectedSignal] = useState<string>("");
  // Add a state for hidden signals - signals that are subscribed but not shown in this component
  const [hiddenSignals, setHiddenSignals] = useState<string[]>([]);
  
  // Debug active signals changes
  useEffect(() => {
    console.log("NumberOut - Active signals updated:", activeSignals);
  }, [activeSignals]);
  
  // Handle subscription to a new signal
  const handleSubscribe = useCallback(() => {
    if (selectedSignal) {
      // Check if the signal is an enumeration type
      if (isEnumSignal(selectedSignal)) {
        console.log(`NumberOut - Skipping subscription to ${selectedSignal} as it's an enumeration (should be handled by Enumeration component)`)
        alert(`The signal "${selectedSignal}" is an enumeration type. Please use the Enumeration component to subscribe to it instead.`)
        setSelectedSignal("")
        return
      }
      
      console.log(`NumberOut - Subscribing to: ${selectedSignal}`)
      // If signal was previously hidden, simply unhide it
      if (hiddenSignals.some(s => s.toLowerCase() === selectedSignal.toLowerCase())) {
        setHiddenSignals(prev => prev.filter(s => s.toLowerCase() !== selectedSignal.toLowerCase()));
      } else {
        // Otherwise subscribe to it
        subscribeToSignal(selectedSignal, SignalType.Numerical); // Explicitly specify as numerical
      }
      setSelectedSignal("");
    }
  }, [selectedSignal, subscribeToSignal, hiddenSignals, isEnumSignal]);
  
  // Handle unsubscribing from a signal
  const removeSignal = useCallback((signalName: string) => {
    console.log(`NumberOut - Removing from view: ${signalName}`);
    
    // Check if this is VC_State which is hardcoded in the EnumerationGraph
    if (signalName.toLowerCase() === "vc_state") {
      // For VC_State, don't unsubscribe (since EnumerationGraph needs it)
      // Instead, just add it to hidden signals
      setHiddenSignals(prev => [...prev, signalName]);
      console.log(`NumberOut - Added ${signalName} to hidden signals list`);
    } else {
      // For other signals, unsubscribe as normal
      unsubscribeFromSignal(signalName);
    }
  }, [unsubscribeFromSignal]);
  
  // Get the latest value for all active signals efficiently
  const latestValues = data.length > 0 ? data[data.length - 1] : { time: Date.now() };
  
  // Ensure the active signals array has no duplicates and filter out hidden signals and enumeration signals
  const uniqueActiveSignals = [...new Set(activeSignals)]
    .filter(signal => !hiddenSignals.some(s => s.toLowerCase() === signal.toLowerCase()))
    .filter(signal => !isEnumSignal(signal)); // Filter out enumeration signals
  
  return (
    <div className="w-full p-4 bg-white dark:bg-gray-800 rounded-lg shadow">
      <h3 className="text-lg font-semibold mb-4">Current Signal Values</h3>
      
      <div className="space-y-3 mb-6">
        {uniqueActiveSignals.length === 0 ? (
          <div className="text-gray-500">No active numerical signals. Add signals below.</div>
        ) : (
          // Using uniqueActiveSignals and adding index to ensure keys are unique
          uniqueActiveSignals.map((signalName, index) => (
            <div key={`${signalName}-${index}`} className="flex justify-between items-center bg-gray-100 dark:bg-gray-700 p-3 rounded">
              <div className="flex items-center">
                <span className="font-medium">{signalName}:</span>
                <span className="ml-2">
                  {latestValues[signalName] !== undefined 
                    ? latestValues[signalName] 
                    : "No data"}
                </span>
              </div>
              <button
                onClick={() => removeSignal(signalName)}
                className="text-red-500 hover:text-red-700"
                title={`Unsubscribe from ${signalName}`}
              >
                Remove
              </button>
            </div>
          ))
        )}
      </div>

      <div className="mb-2 text-sm text-gray-600 dark:text-gray-400">
        💡 Note: Enumeration signals (like state, mode, status) are handled by the Enumeration component above.
      </div>

      <div className="flex flex-wrap gap-2">
        <select
          value={selectedSignal}
          onChange={(e) => setSelectedSignal(e.target.value)}
          className="flex-grow p-2 bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 rounded"
          disabled={isLoadingSignals}
        >
          <option value="">Select a signal...</option>
          {/* Hidden signals first with a marker */}
          {hiddenSignals
            .filter(signalName => !isEnumSignal(signalName)) // Only show non-enum hidden signals
            .map((signalName, index) => (
              <option key={`hidden-${signalName}-${index}`} value={signalName}>
                {signalName} (hidden)
              </option>
            ))}
          {/* Available signals that aren't already displayed and aren't enumeration signals */}
          {availableSignals
            .filter(signal => 
              !uniqueActiveSignals.some(active => 
                active.toLowerCase() === signal.name.toLowerCase()
              ) && 
              !hiddenSignals.some(hidden => 
                hidden.toLowerCase() === signal.name.toLowerCase()
              ) &&
              !isEnumSignal(signal.name) // Filter out enumeration signals
            )
            .map((signal, index) => (
              <option key={`${signal.name}-${index}`} value={signal.name}>
                {signal.name}
              </option>
            ))}
        </select>
        <button
          onClick={handleSubscribe}
          className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600 disabled:bg-gray-400"
          disabled={!selectedSignal}
        >
          Add Signal
        </button>
      </div>
    </div>
  );
};

export default NumberOut;