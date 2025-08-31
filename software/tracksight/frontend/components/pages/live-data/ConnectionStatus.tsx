'use client'

import { useSignals } from '@/hooks/SignalContext'

export default function ConnectionStatus() {
  const { socketConnected: isConnected, reconnectSocket } = useSignals();

  return (
    <div className="w-full p-4 bg-white dark:bg-gray-800 rounded-lg shadow">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-3">
          {/* circle indicator */}
          <div className={`h-3 w-3 rounded-full ${isConnected ? 'bg-green-500' : 'bg-red-500'}`} />

          {/* main text */}
          <span className="font-medium">
            {isConnected ? 'Connected to backend' : 'Connecting to backend...'}
          </span>
        </div>

        <button onClick={reconnectSocket} disabled={isConnected}
          className="px-4 py-2 rounded bg-blue-500 text-white cursor-pointer transition-colors hover:bg-blue-600
            disabled:bg-gray-200 disabled:text-gray-500 disabled:cursor-not-allowed"
        >
          {isConnected ? 'Connected' : 'Connect'}
        </button>
      </div>
    </div>
  )
}
