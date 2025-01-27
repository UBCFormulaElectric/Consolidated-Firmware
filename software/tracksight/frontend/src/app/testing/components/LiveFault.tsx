"use client"
import React, { useState, useEffect } from 'react'
// import Draggable from 'react-draggable'
// import { Checkbox } from '@/components/ui/checkbox'

// const boardNames = ['b1', 'b2', 'b3', 'b4', 'b5', 'b6', 'b7']

function FaultDisplay({ width, tag }: { width: number, tag: "f" | "w" | "a" }) {
	switch (tag) {
		case 'f':
			return (
				<p
					className='bg-red-500 px-6 py-2 h-1 '
					style={{ width: `${width}px` }} />
			)
		case 'w':
			return (
				<p
					className='bg-orange-500 px-6 py-2 h-1 '
					style={{ width: `${width}px` }} />
			)
		case 'a':
			return (
				<p
					className='bg-yellow-500-500 px-6 py-2 h-1 '
					style={{ width: `${width}px` }} />
			)
		default:
			return (
				<p className='px-6 py-2 h-1 ' style={{ width: `${width}px` }} />
			)
	}
}

function FaultBar({ boards, count }: { boards: { timestamp: number, name: string }[], count: number }) {
	return (
		<div className='flex justify-center w-full h-4 my-2'>
			{boards.map((fault, index) => {
				const tag = fault.name.substring(0, 1) as "f" | "w" | "a"
				if (index < boards.length - 1) {
					return (
						<div>
							<FaultDisplay width={97 * (boards[index + 1].timestamp - fault.timestamp)} tag={tag} key={index} />
						</div>
					)
				} else {
					return (
						<FaultDisplay width={97 * (count - boards[boards.length - 1].timestamp)} tag={tag} key={index} />
					)
				}
			})}
		</div>
	)
}

export default function LiveFault() {
	const [boards, setBoards] = useState([{ timestamp: 0, name: '' }])
	const [active, setActive] = useState<boolean[]>([false, false, false, false, false, false, false])
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

	// const FilterFaults: React.FC<{ fault: boolean; index: number }> = ({
	// 	fault,
	// 	index,
	// }) => {
	// 	return (
	// 		<button
	// 			className='bg-cyan-200 dark:bg-cyan-800 pr-2 rounded-md px-2 mx-1'
	// 			onClick={() => {
	// 				toggleFault(index)
	// 			}}>
	// 			{index + ': ' + active[index]}
	// 		</button>
	// 	)
	// }
	return (
		<div className='bg-gray-200 dark:bg-gray-700 w-full h-18 overflow-hidden'>
			<div className='flex flex-col justify-end'>
				{/* <Draggable cancel='.non-draggable'> */}
				<div className='flex flex-col pr-9'>
					{/* Below is the actual faults graph */}
					{active.map((fault, k) => {
						return <FaultBar boards={boards} count={count} key={k} />
					})}
				</div>
				{/* </Draggable> */}
			</div>
		</div>
	)
}
