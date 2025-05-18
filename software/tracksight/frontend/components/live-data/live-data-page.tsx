'use client'

import React, { useState, useEffect } from 'react'
// import LiveFault from './components/LiveFault'
// import Live from './components/Live'
import EnumerationGraph from './Enumeration'
import NumericalGraph from './Numerical'
import NumberOut from './NumberOut'

export default function LiveDataPage() {
  const [currentTime, setCurrentTime] = useState(Date.now())

  const numericalSignals = ['FSM_PAPPS', 'BMS_VOLTAGE']

  useEffect(() => {
    const timeInterval = setInterval(() => {
      setCurrentTime(Date.now())
    }, 100)
    return () => {
      clearInterval(timeInterval)
    }
  }, [])

  return (
    <>
      {/* <Live /> */}
      <div className='overflow-x-scroll pt-14 min-h-screen space-y-6'>
        {/* <MouseTest /> */}
        {/* <Timer /> */}
        {/* <LiveFault /> */}
        <EnumerationGraph currentTime={currentTime} signalName="VC_State" />
        <NumericalGraph numericalSignals={numericalSignals} currentTime={currentTime} />
        <NumberOut numericalSignals={numericalSignals} currentTime={currentTime} />
      </div>
    </>
  )
}
