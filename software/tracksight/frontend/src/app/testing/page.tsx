'use client'

import React, { useState, useEffect } from 'react'
import LiveFault from './components/LiveFault'
import Timer from './components/Timer'
import Live from './components/Live'
import EnumerationGraph from './components/Enumeration'
import NumericalGraph from './components/Numerical'

const TestingPage = () => {
	const [currentTime, setCurrentTime] = useState(Date.now())
	const [currentState, setCurrentState] = useState('VC_INIT_STATE')
	const enumStates = [
		'VC_INIT_STATE',
		'VC_PCM_STATE',
		'VC_HV_STATE',
		'VC_DRIVE_STATE',
		'VC_VD_STATE',
	]

	const numericalSignals = ['FSM_PAPPS', 'BMS_VOLTAGE']

	useEffect(() => {
		const timeInterval = setInterval(() => {
			setCurrentTime(Date.now())
		}, 200)

		const stateInterval = setInterval(() => {
			setCurrentState((prevState) => {
				const nextIndex = Math.floor(Math.random() * enumStates.length)
				return enumStates[nextIndex]
			})
		}, 4000)

		return () => {
			clearInterval(timeInterval)
			clearInterval(stateInterval)
		}
	}, [])

	return (
		<div>
			<Live />
			{/* <MouseTest /> */}
			<Timer />
			<LiveFault />
			<EnumerationGraph
				signalName='VC_STATE'
				currentState={currentState}
				currentTime={currentTime}
				enumStates={enumStates}
			/>
			<NumericalGraph
				numericalSignals={numericalSignals}
				currentTime={currentTime}
			/>
		</div>
	)
}

export default TestingPage
