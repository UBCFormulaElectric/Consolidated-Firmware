'use client'

import LiveFault from './components/LiveFault'
import Timer from './components/Timer'
import Live from './components/Live'

// import MouseTest from './components/MouseTest'

const TestingPage = () => {
	return (
		<div>
			<Live />
			{/* <MouseTest /> */}
			<Timer />
			<LiveFault />
		</div>
	)
}

export default TestingPage

// TODO: Consider using ScrollArea https://ui.shadcn.com/docs/components/scroll-area
