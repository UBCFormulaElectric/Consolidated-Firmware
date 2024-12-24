import React, { useState, useEffect } from 'react'
import Draggable from 'react-draggable'
import { DropdownMenuCheckboxes } from './FaultFilters'

const initData = { timestamp: 0, name: 'f' }

const LiveFault: React.FC = () => {
	// TODO change the TS type late
	const [board1, setBoard1] = useState([initData])
	const [faults, setFaults] = useState<boolean[]>([
		false,
		false,
		false,
		false,
		false,
		false,
		false,
	])
	const [warnings, setWarnings] = useState<boolean[]>([
		false,
		false,
		false,
		false,
		false,
		false,
		false,
	])
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
						className='bg-red-500 px-6 py-2 h-1 '
						style={{ width: `${width}px` }}
					/>
				)
			} else if (isWarning) {
				return (
					<p
						className='bg-orange-500 px-6 py-2 h-1 '
						style={{ width: `${width}px` }}
					/>
				)
			} else {
				return <p className='px-6 py-2 h-1 ' style={{ width: `${width}px` }} />
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
									width={97 * (board1[index + 1].timestamp - fault.timestamp)}
									isFault={hasFault}
									isWarning={hasWarning}
								/>
							</div>
						)
					} else {
						return (
							<FaultDisplay
								width={97 * (count - board1[board1.length - 1].timestamp)}
								isFault={hasFault}
								isWarning={hasWarning}
							/>
						)
					}
				})}
			</div>
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
		// TODO: Figure out how to cap the height of this to 30% and have endpoints
		<div className='bg-gray-200 dark:bg-gray-700 w-full h-18 overflow-hidden'>
			<div className='flex flex-col justify-end'>
				<Draggable cancel='.non-draggable'>
					<div className='flex flex-col pr-9'>
						{/* Below is the actual faults graph */}
						{faults.map((fault, index) => {
							return <FaultBar fault={fault} index={index} key={index} />
						})}
					</div>
				</Draggable>
				<div className='p-2'>TBD{/* <DropdownMenuCheckboxes /> */}</div>
			</div>
		</div>
	)
}

export default LiveFault
