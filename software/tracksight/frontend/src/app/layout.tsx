import './globals.css'
import styles from './page.module.css'; // TODO remove
import type { Metadata } from 'next'
import { Inter } from 'next/font/google'
import NavBar from './navbar';
import ThemeProvider, { ThemeContext } from './ThemeProvider';

const inter = Inter({ subsets: ['latin'] })

export const metadata: Metadata = {
  title: 'Tracksight',
  description: 'Telemetry by UBC Formula Electric',
}

export default function RootLayout({ children }: {
  children: React.ReactNode
}) {
  return (
    <html lang="en">
      <body className={inter.className}>
        <ThemeProvider>
          <div className={styles.main}>
              <NavBar />
              {/* <div> */}
                {children}
              {/* </div> */}
          </div>
        </ThemeProvider>
      </body>
    </html>
  )
}