"use client"

import { ThemeProvider } from "@/components/shared/ThemeProvider"
import { PausePlayProvider } from "@/components/shared/PausePlayControl"
import { QueryClient, QueryClientProvider } from "@tanstack/react-query"

const queryClient = new QueryClient()
export default function Providers({ children }: { children: React.ReactNode }) {
	return (
		<QueryClientProvider client={queryClient}>
			<ThemeProvider attribute="class" defaultTheme="dark" enableSystem>
				<PausePlayProvider>
					{children}
				</PausePlayProvider>
			</ThemeProvider>
		</QueryClientProvider>
	)
}