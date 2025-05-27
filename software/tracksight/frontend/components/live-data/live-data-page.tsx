'use client'

import React from 'react'
import EnumerationGraph from './Enumeration'
import NumericalGraph from './Numerical'
// import NumberOut from './NumberOut' // Functionality moved to LiveDataTable
import LiveDataTable from './live-data-table'
import ConnectionStatus from './connection-status'
import DynamicRowManager from '@/components/shared/DynamicRowManager'

export default function LiveDataPage() {
  return (
    <>
      <div className='overflow-x-scroll pt-14 min-h-screen space-y-6 px-6'>
        <h2 className="text-xl font-bold mb-2">Connection Status</h2>
        <ConnectionStatus />
        
        {/* <h2 className="text-xl font-bold mb-2">Live Telemetry Data</h2>
        <LiveDataTable />
        
        <h2 className="text-xl font-bold mb-2">Signal State Timeline</h2>
        <EnumerationGraph/>
        
        <h2 className="text-xl font-bold mb-2">Signal Trends</h2>
        <NumericalGraph /> */}
        {/* NumberOut functionality now integrated into LiveDataTable */}
        {/* <NumberOut /> */}
        
        <h2 className="text-xl font-bold mb-2">Add New Signal Subscriptions</h2>
        <DynamicRowManager />
      </div>
    </>
  )
}
