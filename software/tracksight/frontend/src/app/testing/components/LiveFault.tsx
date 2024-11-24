import React, { useState, useEffect } from 'react'
import { DropdownMenuCheckboxes } from './FaultFilters'

const initData = { timestamp: 0, name: 'e' }

const LiveFault: React.FC = () => {
	// TODO change the TS type late
	const [board1, setBoard1] = useState([initData])
	const [faults, setFaults] = useState<boolean[]>([false, false, false])
	const [warnings, setWarnings] = useState<boolean[]>([false, false, false])
	const [count, setCount] = useState<number>(-1)

	useEffect(() => {
		const updateCount = () => {
			setCount((prev) => ++prev)
		}

		const interval = setInterval(updateCount, 1000)
		return () => {
			clearInterval(interval)
		}
	}, [])

	const FaultBar: React.FC = () => {
		const FaultDisplay: React.FC<{
			width: number
			isFault: Boolean
			isWarning: Boolean
		}> = ({ width, isFault, isWarning }) => {
			if (isFault) {
				return (
					<p
						className='bg-red-500 px-6 py-2 h-1 rounded-full'
						style={{ width: `${width}px` }}
					/>
				)
			} else if (isWarning) {
				return (
					<p
						className='bg-orange-500 px-6 py-2 h-1 rounded-full'
						style={{ width: `${width}px` }}
					/>
				)
			} else {
				return (
					<p
						className='px-6 py-2 h-1 rounded-full'
						style={{ width: `${width}px` }}
					/>
				)
			}
		}

		return (
			<div className='flex justify-center w-full h-4 my-2'>
				{board1.map((fault, index) => {
					const hasFault = fault.name.substring(0, 1) === 'f'
					const hasWarning = fault.name.substring(0, 1) === 'w'

					if (index < board1.length - 1) {
						return (
							<div>
								<FaultDisplay
									width={96 * (board1[index + 1].timestamp - fault.timestamp)}
									isFault={hasFault}
									isWarning={hasWarning}
								/>
							</div>
						)
					} else {
						return (
							<FaultDisplay
								width={96 * count}
								isFault={hasFault}
								isWarning={hasWarning}
							/>
						)
					}
				})}
			</div>
		)
	}

	// ! DEBUGGING: REMOVE ========================
	const toggleFault = (key: number) => {
		var newFaults = [...faults]
		newFaults[key] = !newFaults[key]
		setFaults(newFaults)

		setBoard1((prev) => {
			const object = {
				timestamp: count,
				name: newFaults[key] ? 'fault1' : 'e',
			}
			return [...prev, object]
		})
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

	// ! DEBUGGING: REMOVE
	const toggleWarning = (key: number) => {
		var newWarnings = [...warnings]
		newWarnings[key] = !newWarnings[key]
		setWarnings(newWarnings)

		setBoard1((prev) => {
			const object = {
				timestamp: count,
				name: newWarnings[key] ? 'warning1' : 'e',
			}
			return [...prev, object]
		})
	}

	// ! DEBUGGING: REMOVE ================
	const WarningButton: React.FC<{ warning: boolean; index: number }> = ({
		warning,
		index,
	}) => {
		return (
			<button
				className='bg-cyan-200 dark:bg-cyan-800 pr-2 rounded-md px-2 mx-1'
				onClick={() => {
					toggleWarning(index)
				}}>
				{index + ': ' + warnings[index]}
			</button>
		)
	}

	const ResetButton = () => {
		return (
			<button
				className='bg-cyan-200 dark:bg-cyan-800 pr-2 rounded-md px-2 mx-1'
				onClick={() => {
					console.log(board1)
					setBoard1([initData])
					setFaults([false, false, false])
					setWarnings([false, false, false])
					setCount(-1)
				}}>
				Reset
			</button>
		)
	}

	// TODO vvv to be worked on.

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
				<p className='pr-2'>Toggle Warnings: </p>
				{warnings.map((button, index) => {
					return <WarningButton warning={button} index={index} key={index} />
				})}
				<p className='pr-2'>Reset: </p>
				<ResetButton />
			</div>
		</div>
	)
}

export default LiveFault
