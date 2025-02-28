'use client'

import React, { useState, useEffect } from 'react'
import LiveFault from './components/LiveFault'
import Live from './components/Live'
import EnumerationGraph from './components/Enumeration'
import NumericalGraph from './components/Numerical'
import NumberOut from './components/NumberOut'

export default function TestingPage() {
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
			<div className='overflow-x-scroll pt-14 min-h-screen'>
				{/* <MouseTest /> */}
				{/* <Timer /> */}
				{/* <LiveFault /> */}
				<EnumerationGraph currentTime={currentTime} signalName="VC_State" />
				{/* <NumericalGraph
                    numericalSignals={numericalSignals}
                    currentTime={currentTime}
                /> */}
				{/* <NumericalGraph numericalSignals={numericalSignals} currentTime={currentTime} /> */}
				<NumberOut numericalSignals={numericalSignals} currentTime={currentTime} />
				
			</div>
		</>
	)
}
