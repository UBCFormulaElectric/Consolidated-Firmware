'use client'

import React, { useCallback, useMemo } from 'react'

export default function ConnectionStatus() {
  const isConnected = false;
  const isLoadingSignals = false;
  const signalError = null;
  const reconnect = useCallback(() => { }, []);
  const availableSignals = useMemo(() => [], []);

  return (
    <div className="w-full p-4 bg-white dark:bg-gray-800 rounded-lg shadow">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-3">
          <div
            className={`h-3 w-3 rounded-full ${isConnected ? 'bg-green-500' : isLoadingSignals ? 'bg-yellow-500' : 'bg-red-500'
              }`}
          />
          <span className="font-medium">
            {isConnected
              ? `Connected: ${availableSignals.length} signals available`
              : isLoadingSignals
                ? 'Connecting to backend...'
                : 'Disconnected'
            }
          </span>
        </div>

        {signalError && (
          <div className="text-red-500 text-sm max-w-md truncate">
            Error: {signalError}
          </div>
        )}

        <button
          onClick={reconnect}
          disabled={isConnected && !signalError}
          className={`px-4 py-2 rounded ${isConnected && !signalError
            ? 'bg-gray-200 text-gray-500 cursor-not-allowed'
            : 'bg-blue-500 text-white hover:bg-blue-600'
            }`}
        >
          {isConnected ? 'Connected' : 'Connect'}
        </button>
      </div>
    </div>
  )
}
