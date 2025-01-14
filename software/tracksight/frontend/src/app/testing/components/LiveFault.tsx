import React, { useState, useEffect } from 'react'
import Draggable from 'react-draggable'
import { Checkbox } from '@/components/ui/checkbox'

const initData = { timestamp: 0, name: 'f' }
const boardNames = ['b1', 'b2', 'b3', 'b4', 'b5', 'b6', 'b7']

const LiveFault: React.FC = () => {
	// TODO change the TS type late
	const [boards, setBoards] = useState([initData])
	const [active, setActive] = useState<boolean[]>([
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
				{boards.map((fault, index) => {
					const hasFault = fault.name.substring(0, 1) === 'f'
					const hasWarning = fault.name.substring(0, 1) === 'w'

					if (index < boards.length - 1) {
						return (
							<div>
								<FaultDisplay
									width={97 * (boards[index + 1].timestamp - fault.timestamp)}
									isFault={hasFault}
									isWarning={hasWarning}
								/>
							</div>
						)
					} else {
						return (
							<FaultDisplay
								width={97 * (count - boards[boards.length - 1].timestamp)}
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
				{index + ': ' + active[index]}
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
						{active.map((fault, index) => {
							return <FaultBar fault={fault} index={index} key={index} />
						})}
					</div>
				</Draggable>
				{/* map for easier sync with checkboxes, then use an array of names to match the names too */}
				{active.map((fault, index) => {
					return (
						<div>
							<p>{boardNames[index]}</p>
							<Checkbox />
						</div>
					)
				})}
			</div>
		</div>
	)
}

export default LiveFault
