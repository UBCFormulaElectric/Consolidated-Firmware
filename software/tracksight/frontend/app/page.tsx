"use client"

import { useEffect } from "react"
import { useRouter } from "next/navigation"

export default function Home() {
  const router = useRouter()

  useEffect(() => {
    // Redirect to live-data page on load
    router.push("/live-data")
  }, [router])

  return (
    <div className="flex items-center justify-center min-h-screen">
      <div className="text-center">
        <h1 className="text-2xl font-bold mb-4">Tracksight Dashboard</h1>
        <p className="text-gray-600 dark:text-gray-400">Redirecting to Live Data...</p>
      </div>
    </div>
  )
}
