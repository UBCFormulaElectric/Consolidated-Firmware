import React, { useState, useEffect } from 'react'
import { DropdownMenuCheckboxes } from './FaultFilters'

const LiveFault: React.FC = () => {
	// ! TO Be Replaced
	const [faults, setFaults] = useState<boolean[]>([false, false, false])
	const [width, setWidth] = useState<number>(9) // Can't set 0, so buffering

	useEffect(() => {
		const updateScale = () => {
			setWidth((prev) => prev + 103)
		}

		const interval = setInterval(updateScale, 1000)
		return () => clearInterval(interval)
	}, [])

	const FaultBar: React.FC<{ fault: boolean; index: number }> = ({
		fault,
		index,
	}) => {
		return (
			<div className='flex justify-center w-full h-4 my-2'>
				{fault ? (
					<p
						className='bg-blue-500 px-6 py-2 h-6 rounded-full text-white text-center font-bold '
						style={{ width: `${width}px` }}>
						{fault.toString() + index}
					</p>
				) : (
					<span
						className='bg-black px-6 py-2 h-1 rounded-full'
						style={{ width: `${width}px` }}></span>
				)}
			</div>
		)
	}

	// ! DEBUGGING: REMOVE
	const toggleFault = (key: number) => {
		var newFaults = [...faults]
		newFaults[key] = !newFaults[key]

		setFaults(newFaults)
	}

	// ! DEBUGGING: REMOVE
	const FaultButton: React.FC<{ fault: boolean; index: number }> = ({
		fault,
		index,
	}) => {
		return (
			<button
				className='bg-cyan-200 dark:bg-cyan-800 pr-2 rounded-md px-2 mx-1'
				onClick={() => {
					toggleFault(index)
				}}>
				{index + ': ' + faults[index]}
			</button>
		)
	}

	const FilterFaults: React.FC<{ fault: boolean; index: number }> = ({
		fault,
		index,
	}) => {
		return (
			<button
				className='bg-cyan-200 dark:bg-cyan-800 pr-2 rounded-md px-2 mx-1'
				onClick={() => {
					toggleFault(index)
				}}>
				{index + ': ' + faults[index]}
			</button>
		)
	}

	return (
		<div className='bg-gray-200 dark:bg-gray-700 w-full'>
			<div className='flex flex-row justify-end'>
				<div className='flex flex-col pr-9'>
					{/* Below is the actual faults graph */}
					{faults.map((fault, index) => {
						return <FaultBar fault={fault} index={index} key={index} />
					})}
				</div>
				<div className='p-2'>TBD{/* <DropdownMenuCheckboxes /> */}</div>
			</div>
			{/* //! TODO: REMOVE AFTER TESTING */}
			<div className='flex justify-start'>
				<p className='pr-2'>Toggle Faults: </p>
				{faults.map((button, index) => {
					return <FaultButton fault={button} index={index} key={index} />
				})}
				<p className='pr-2'>Reset: </p>
			</div>
		</div>
	)
}

export default LiveFault
