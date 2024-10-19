'use client'

import { usePathname, useRouter } from 'next/navigation'
import { useContext } from 'react'
import { Menu, Switch } from 'antd'
import { Theme, ThemeContext } from './ThemeProvider'

export default function NavBar() {
	const { theme, setTheme } = useContext(ThemeContext)
	const router = useRouter()
	const path = usePathname()

	return (
		<>
			<nav className='p-0 fixed top-0 left-0 right-0'>
				<Menu
					theme={theme ? 'dark' : 'light'}
					mode='horizontal'
					defaultSelectedKeys={[path.split('/')[1] || 'home']}
					onClick={(e) => {
						switch (e.key) {
							case 'home':
								return router.push('/')
							case 'visualize':
								return router.push('/visualize')
							case 'dashboard':
								return router.push('/dashboard')
							case 'testing':
								return router.push('/testing')
							case 'theme':
								break
						}
					}}
					className='flex-grow'>
					<Menu.Item key='home'>Home</Menu.Item>
					<Menu.Item key='visualize'>Visualize</Menu.Item>
					<Menu.Item key='dashboard'>Dashboards</Menu.Item>
					<Menu.Item key='testing'>Testing</Menu.Item>
					<div className='order-10'>
						<Switch
							checked={theme === Theme.DARK}
							onChange={(checked: boolean) =>
								setTheme(checked ? Theme.DARK : Theme.LIGHT)
							}
							checkedChildren='Dark'
							unCheckedChildren='Light'
						/>
					</div>
				</Menu>
			</nav>
			<div className='h-14'></div>
		</>
	)
}
