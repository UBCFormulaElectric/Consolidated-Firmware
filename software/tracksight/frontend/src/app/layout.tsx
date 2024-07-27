import './globals.css'
import type { Metadata } from 'next'
import { Inter } from 'next/font/google'

const inter = Inter({ subsets: ['latin'] })

export const metadata: Metadata = {
  title: 'Tracksight',
  description: 'Telemetry by UBC Formula Electric',
}


import styles from './page.module.css'; // TODO remove
import NavBar from './navbar';
import { Layout } from 'antd';
const { Header, Content } = Layout;

export default function RootLayout({ children }: {
  children: React.ReactNode
}) {
  return (
    <html lang="en">
      <body className={inter.className}>
        <Layout className={`${styles.main}`}>
          <Header className={styles.header}>
            <NavBar />
          </Header>
          <Content>
            {children}
          </Content>
        </Layout>
      </body>
    </html>
  )
}