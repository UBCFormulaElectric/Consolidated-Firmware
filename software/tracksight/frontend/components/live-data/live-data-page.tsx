'use client'

import React from 'react'
import EnumerationGraph from './Enumeration'
import NumericalGraph from './Numerical'
import NumberOut from './NumberOut'
import LiveDataTable from './live-data-table'
import ConnectionStatus from './connection-status'

export default function LiveDataPage() {
  return (
    <>
      <div className='overflow-x-scroll pt-14 min-h-screen space-y-6'>
        <ConnectionStatus />
        <LiveDataTable />
        <EnumerationGraph signalName="VC_State" />
        <NumericalGraph />
        <NumberOut />
      </div>
    </>
  )
}
