import React from 'react'

const Live: React.FC = () => {
	return (
		<div className='absolute flex flex-row z-10 top-0 bottom-0 right-12'>
			<div className='flex flex-row items-center bg-green-500 m-2 px-2 py-1 h-fit rounded-lg'>
				<div className='bg-red-500 w-3 h-3 rounded-full mr-1' />
				<p className='font-semibold'>LIVE</p>
			</div>
			<div className='bg-green-500 w-1.5 h-full' />
		</div>
	)
}

export default Live

// TODO: Make this Live bar movable in the future
