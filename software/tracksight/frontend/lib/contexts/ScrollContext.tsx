"use client";

import {
    createContext,
    useContext,
    useRef,
    ReactNode,
    RefObject,
    useEffect,
} from "react";

/**
 * Default duration of time (in milliseconds) that fits within the full view of the scrollable area.
 */
const FULL_VIEW_DURATION_MS = 2_000;
const SCROLL_SENSITIVITY = 1;

const ZOOM_SENSITIVITY = 0.01;
const MIN_ZOOM_LEVEL = 0.1;

// TODO(evan): Make this dynamic based on data range
const MAX_ZOOM_LEVEL = 10;

interface ScrollContextValue {
    scrollableRef: RefObject<HTMLDivElement | null>;
    childRef: RefObject<HTMLDivElement | null>;

    leftEdgeTimestampRef: RefObject<number | null>;
    rightEdgeTimestampRef: RefObject<number | null>;
}

const ScrollContext = createContext<ScrollContextValue | null>(null);

interface ScrollProviderProps {
    children: ReactNode;
    initialTimestamp: number;

    // TODO(evan): Add a end timestamp to limit the maximum scroll range for historical data
}

const resetScrollPosition = (scrollableElement: HTMLDivElement, childElement: HTMLDivElement) => {
    const scrollableWidth = scrollableElement.clientWidth;
    const childWidth = childElement.scrollWidth;

    const centerPosition = (childWidth - scrollableWidth) / 2;
    scrollableElement.scrollLeft = centerPosition;
};

export function ScrollProvider({ children, initialTimestamp }: ScrollProviderProps) {
    const scrollableRef = useRef<HTMLDivElement | null>(null);
    const childRef = useRef<HTMLDivElement | null>(null);
    const animationFrame = useRef<number | null>(null);

    const hasReachedRightEdgeRef = useRef<boolean>(false);
    const autoFollowRef = useRef<boolean>(false);

    const leftEdgeTimestampRef = useRef<number>(initialTimestamp);
    const rightEdgeTimestampRef = useRef<number>(initialTimestamp + FULL_VIEW_DURATION_MS);
    const maximumTimestampRef = useRef<number>(initialTimestamp + FULL_VIEW_DURATION_MS);

    const zoomLevelRef = useRef<number>(1);

    const value: ScrollContextValue = {
        scrollableRef,
        childRef,

        leftEdgeTimestampRef,
        rightEdgeTimestampRef,
    };

    useEffect(() => {
        let lastTimestamp: number | null = null;

        const handleTick = (timestamp: number) => {
            const currentTimestamp = performance.timeOrigin + timestamp;
            const deltaMs = lastTimestamp !== null ? currentTimestamp - lastTimestamp : 0;

            if (!hasReachedRightEdgeRef.current && rightEdgeTimestampRef.current < currentTimestamp) {
                autoFollowRef.current = true;
                hasReachedRightEdgeRef.current = true;
            }

            if (autoFollowRef.current) {
                leftEdgeTimestampRef.current += deltaMs;
                rightEdgeTimestampRef.current += deltaMs;
                maximumTimestampRef.current += deltaMs;
            }

            lastTimestamp = currentTimestamp;
            animationFrame.current = requestAnimationFrame(handleTick);
        }

        animationFrame.current = requestAnimationFrame(handleTick);

        return () => {
            if (!animationFrame.current) return;

            cancelAnimationFrame(animationFrame.current);
        };
    }, []);

    useEffect(() => {
        const scrollableElement = scrollableRef.current;
        if (!scrollableElement) return;

        const handleScroll = () => {
            if (!scrollableRef.current || !childRef.current) return;

            if (!hasReachedRightEdgeRef.current) return;

            const delta = scrollableRef.current.scrollLeft - (scrollableRef.current.scrollWidth - scrollableRef.current.clientWidth) / 2;
            const deltaMs = (delta / scrollableRef.current.clientWidth) * (FULL_VIEW_DURATION_MS * zoomLevelRef.current) * SCROLL_SENSITIVITY;

            autoFollowRef.current = false;

            leftEdgeTimestampRef.current += deltaMs;
            rightEdgeTimestampRef.current += deltaMs;

            if (leftEdgeTimestampRef.current < initialTimestamp) {
                leftEdgeTimestampRef.current = initialTimestamp;
                rightEdgeTimestampRef.current = initialTimestamp + (FULL_VIEW_DURATION_MS * zoomLevelRef.current);
            }

            if (rightEdgeTimestampRef.current >= maximumTimestampRef.current) {
                autoFollowRef.current = true;
                leftEdgeTimestampRef.current = maximumTimestampRef.current - (FULL_VIEW_DURATION_MS * zoomLevelRef.current);
                rightEdgeTimestampRef.current = maximumTimestampRef.current;
            }

            resetScrollPosition(scrollableRef.current, childRef.current!);
        };

        scrollableElement.addEventListener("scroll", handleScroll);

        return () => {
            scrollableElement.removeEventListener("scroll", handleScroll);
        };
    }, []);

    useEffect(() => {
        const handleWheel = (event: WheelEvent) => {
            if (Math.abs(event.deltaY) <= Math.abs(event.deltaX)) return;

            event.preventDefault();

            const mousePositionX = event.clientX;
            const scrollableElement = scrollableRef.current;
            if (!scrollableElement) return;

            const scrollableRect = scrollableElement.getBoundingClientRect();
            const relativeMouseX = mousePositionX - scrollableRect.left;
            const mouseXRatio = relativeMouseX / scrollableRect.width;

            autoFollowRef.current = false;

            const zoomFactor = 1 + ZOOM_SENSITIVITY;
            const previousZoomLevel = zoomLevelRef.current;

            if (event.deltaY < 0) zoomLevelRef.current /= zoomFactor;
            else zoomLevelRef.current *= zoomFactor;

            zoomLevelRef.current = Math.max(MIN_ZOOM_LEVEL, Math.min(zoomLevelRef.current, MAX_ZOOM_LEVEL));

            const centerTimestamp = leftEdgeTimestampRef.current + (FULL_VIEW_DURATION_MS * previousZoomLevel * mouseXRatio);

            leftEdgeTimestampRef.current = centerTimestamp - (FULL_VIEW_DURATION_MS * zoomLevelRef.current * mouseXRatio);
            rightEdgeTimestampRef.current = leftEdgeTimestampRef.current + (FULL_VIEW_DURATION_MS * zoomLevelRef.current);

            if (leftEdgeTimestampRef.current < initialTimestamp) {
                leftEdgeTimestampRef.current = initialTimestamp;
                rightEdgeTimestampRef.current = initialTimestamp + (FULL_VIEW_DURATION_MS * zoomLevelRef.current);
            }

            if (rightEdgeTimestampRef.current >= maximumTimestampRef.current) {
                rightEdgeTimestampRef.current = maximumTimestampRef.current;
                leftEdgeTimestampRef.current = rightEdgeTimestampRef.current - (FULL_VIEW_DURATION_MS * zoomLevelRef.current);
                autoFollowRef.current = true;
            }

            resetScrollPosition(scrollableElement, childRef.current!);
        };

        const scrollableElement = scrollableRef.current;
        if (!scrollableElement) return;

        scrollableElement.addEventListener("wheel", handleWheel, { passive: false });

        return () => {
            scrollableElement.removeEventListener("wheel", handleWheel);
        };
    }, []);

    useEffect(() => {
        const scrollableElement = scrollableRef.current;
        const childElement = childRef.current;
        if (!scrollableElement || !childElement) return;

        resetScrollPosition(scrollableElement, childElement);
    }, []);

    return (
        <ScrollContext.Provider value={value}>{children}</ScrollContext.Provider>
    );
}

export function useScrollableRef() {
    const context = useContext(ScrollContext);

    if (!context) {
        throw new Error("useScrollableRef must be used within a ScrollProvider");
    }

    return {
        scrollableRef: context.scrollableRef,
        childRef: context.childRef,
    };
}

export function useScrollState() {
    const context = useContext(ScrollContext);

    if (!context) {
        throw new Error("useScrollState must be used within a ScrollProvider");
    }

    return {
        leftEdgeTimestampRef: context.leftEdgeTimestampRef,
        rightEdgeTimestampRef: context.rightEdgeTimestampRef,
    };
}
