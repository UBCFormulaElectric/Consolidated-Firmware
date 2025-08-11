import { useEffect, useRef } from 'react'
import { useDisplayControl } from '@/components/shared/PausePlayControl'
import { useSignals } from '@/lib/contexts/SignalContext'

export function useAutoscroll() {
    const { isAutoscrollEnabled, isPaused } = useDisplayControl()
    const { numericalData, enumData } = useSignals()
    const lastDataLength = useRef(0)
    const scrollContainerRef = useRef<HTMLElement | null>(null)

    // Find and store reference to scroll container
    useEffect(() => {
        const findScrollContainer = () => {
            // Try multiple selectors to find the main scroll container
            const selectors = [
                '.overflow-x-scroll',
                '[class*="overflow-x-scroll"]',
                '.inline-block.min-w-\\[calc\\(100vw-48px\\)\\]'
            ]

            for (const selector of selectors) {
                const container = document.querySelector(selector) as HTMLElement
                if (container && container.scrollWidth > container.clientWidth) {
                    return container
                }
            }

            return null
        }

        const container = findScrollContainer()
        scrollContainerRef.current = container

        // Re-check periodically in case the container is created dynamically
        const intervalId = setInterval(() => {
            if (!scrollContainerRef.current) {
                const newContainer = findScrollContainer()
                if (newContainer) {
                    scrollContainerRef.current = newContainer
                }
            }
        }, 1000)

        return () => clearInterval(intervalId)
    }, [])

    // Auto-scroll to the right when new data arrives
    useEffect(() => {
        if (!isAutoscrollEnabled || isPaused) {
            return
        }

        const currentDataLength = numericalData.length + enumData.length

        // Only scroll if we have new data
        if (currentDataLength > lastDataLength.current && lastDataLength.current > 0) {
            // Try to find container again if we don't have one
            if (!scrollContainerRef.current) {
                const container = document.querySelector('.overflow-x-scroll') as HTMLElement
                scrollContainerRef.current = container
            }

            const container = scrollContainerRef.current
            if (container) {
                // Scroll to the far right with smooth behavior
                container.scrollTo({
                    left: container.scrollWidth,
                    behavior: 'smooth'
                })
            }
        }

        lastDataLength.current = currentDataLength
    }, [numericalData.length, enumData.length, isAutoscrollEnabled, isPaused])

    // Reset data length tracking when autoscroll is enabled/disabled
    useEffect(() => {
        if (isAutoscrollEnabled) {
            lastDataLength.current = numericalData.length + enumData.length
        }
    }, [isAutoscrollEnabled, numericalData.length, enumData.length])
}
