import React, { useState, useEffect } from 'react'

const TimeBar: React.FC = () => {
	const [times, setTimes] = useState<string[]>([])

	useEffect(() => {
		const interval = setInterval(() => {
			const now = new Date()
			const currentTime = now.toLocaleTimeString().split(' ')[0]

			setTimes((prevTimes) => {
				const updatedTimes = [...prevTimes, currentTime]
				if (updatedTimes.length > 15) {
					updatedTimes.shift()
				}

				return updatedTimes
			})
		}, 1000)

		return () => clearInterval(interval)
	}, [])

	return (
		<div className='bg-gray-100 dark:bg-gray-800 py-2 pr-14 h-10'>
			<div className='flex justify-end gap-10'>
				{times.map((time, index) => (
					<div key={index} className='text-center'>
						{time}
					</div>
				))}
			</div>
		</div>
	)
}

export default TimeBar
