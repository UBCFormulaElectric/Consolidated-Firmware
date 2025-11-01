import React, { useState, useRef, useCallback } from "react";

const MIN_VALUE = 1;
const MAX_VALUE = 100;
const WINDOW_SIZE = 10; // How many numbers to show at once
const TOTAL_DATA_ITEMS = MAX_VALUE - MIN_VALUE + 1;

function ScrollController({
  onScrollChange,
  style,
}: {
  onScrollChange: (newStart: number) => void;
  style?: React.CSSProperties;
}) {
  // use 3000px as an arbitrary large width for the inner div to create a long scroll range
  const innerScrollWidth = 3000;

  const handleScroll = (e: React.UIEvent<HTMLDivElement>) => {
    const { scrollLeft, scrollWidth, clientWidth } = e.currentTarget;

    console.log(scrollLeft, scrollWidth, clientWidth);

    const maxScrollLeft = scrollWidth - clientWidth;

    if (maxScrollLeft === 0) {
      onScrollChange(0);
      return;
    }

    const scrollPercentage = scrollLeft / maxScrollLeft;

    onScrollChange(scrollPercentage);
    console.log("Scroll Percentage:", scrollPercentage);
  };

  return (
    <div
      //className="invisible-scrollbar" // For the WebKit CSS rule
      onScroll={handleScroll}
      style={{
        ...style, // receives position: 'absolute', zIndex, etc.
        overflowX: "scroll",
        overflowY: "hidden",
        cursor: "ew-resize",
        background: "transparent",
        //scrollbarWidth: 'none',
        //msOverflowStyle: 'none',
      }}
    >
      {/* This is the invisible, wide child that creates the scrollbar */}
      <div style={{ width: `${innerScrollWidth}px`, height: "1px" }}></div>
    </div>
  );
}

function DataVisualizer({
  title,
  range,
  windowSize,
  progress,
}: {
  title: string;
  range: [number, number];
  windowSize: number;
  progress: number;
}) {
  const [min, max] = range;
  const totalDataItems = max - min + 1;

  const actualWindowSize = Math.min(windowSize, totalDataItems);
  const possibleStartPositions = totalDataItems - actualWindowSize + 1;

  const startIndex = Math.round(progress * possibleStartPositions);
  const visibleStart = min + startIndex;

  const numbers = [];
  const end = visibleStart + actualWindowSize;

  for (let i = visibleStart; i < end; i++) {
    numbers.push(i);
  }

  return (
    <div
      style={{
        borderRadius: "8px",
        padding: "16px",
        margin: "16px 0",
        background: "#000000ff",
        overflow: "hidden",
      }}
      className="pointer-events-none select-none"
    >
      <h3>{title}</h3>
      <p>
        Showing range:{" "}
        <strong>
          {visibleStart} - {end - 1}
        </strong>
      </p>
      <div style={{ display: "flex", gap: "5px" }}>
        {numbers.map((num) => (
          <div
            key={num}
            style={{
              flex: "1 0 50px", // Grow, don't shrink, base 50px
              textAlign: "center",
              padding: "10px",
              background: "#e7f3ff",
              border: "1px solid #b3d7ff",
              borderRadius: "4px",
              fontWeight: "bold",
              color: "black",
            }}
          >
            {num}
          </div>
        ))}
      </div>
    </div>
  );
}

export default function SyncedGraphContainer() {
  const [scrollProgress, setScrollProgress] = useState(0);

  const handleProgressUpdate = (newProgress: number) => {
    setScrollProgress(newProgress);
  };

  return (
    <>
      <DataVisualizer
        title="Graph 1"
        range={[1, 100]}
        windowSize={10}
        progress={scrollProgress}
      />
      <DataVisualizer
        title="Graph 2"
        range={[50, 200]}
        windowSize={20}
        progress={scrollProgress}
      />
      <DataVisualizer
        title="Graph 3"
        range={[1, 30]}
        windowSize={5}
        progress={scrollProgress}
      />
    </>
  );
}
