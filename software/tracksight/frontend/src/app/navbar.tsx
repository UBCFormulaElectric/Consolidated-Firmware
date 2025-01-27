'use client'

import { usePathname, useRouter } from 'next/navigation'
import { useContext } from 'react'
import { Theme, ThemeContext } from './ThemeProvider'
import Link from 'next/link'

export default function NavBar() {
	const { setTheme } = useContext(ThemeContext)
	const path = usePathname()

	return (
		<>
			<nav className='fixed top-0 left-0 right-0 h-11 bg-white dark:bg-white/20 z-10'>
				<div className='flex flex-row items-center gap-x-4 h-full'>
					<Link className="data-[selected=true]:font-bold" data-selected={path == '/'} href="/">Home</Link>
					<Link className="data-[selected=true]:font-bold" data-selected={path == '/visualize'} href="/visualize">Visualize</Link>
					<Link className="data-[selected=true]:font-bold" data-selected={path == '/dashboard'} href="/dashboard">Dashboard</Link>
					<Link className="data-[selected=true]:font-bold" data-selected={path == '/testing'} href="/testing">Testing</Link>
					<button type="button" onClick={() => setTheme(Theme.LIGHT)}>Set Dark Theme</button>
				</div>
			</nav>
			<div className='h-11'></div>
		</>
	)
}
