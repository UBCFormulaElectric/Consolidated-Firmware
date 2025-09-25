"use client";

import React, { useEffect, useMemo, useRef } from "react";
import "uplot/dist/uPlot.min.css";

type SeriesMeta = { label: string; color?: string };
type AlignedData = [number[], ...(Array<(number | null)[]>)];

type Props = {
  data: AlignedData;
  series: SeriesMeta[]; // one per Y series
  width: number;
  height: number;
  spanGaps?: boolean; // connect across nulls
  options?: any; // keep flexible; uPlot's Options type is large,
  version?: number
};

export default function UPlotChart({ data, series, width, height, spanGaps = true, options, version }: Props) {
  const containerRef = useRef<HTMLDivElement | null>(null);
  const plotRef = useRef<any>(null);
  const uCtorRef = useRef<any>(null);
  const pendingRAF = useRef<number | null>(null);

  // Build uPlot series config once per meta/options change
  const uPlotSeries = useMemo(() => {
    return [
      { label: "time" },
      ...series.map((s) => ({
        label: s.label,
        stroke: s.color,
        spanGaps,
        width: 1,
      }))
    ];
  }, [series, spanGaps]);

  // Instantiate once on mount (SSR-safe via dynamic import)
  useEffect(() => {
    let mounted = true;
    (async () => {
      if (!containerRef.current) return;
      if (!uCtorRef.current) {
        const mod = await import("uplot");
        // default export is the constructor
        uCtorRef.current = (mod as any).default || (mod as any);
      }
      if (!mounted || !containerRef.current) return;

      const baseOptions = {
        width,
        height,
        series: uPlotSeries,
        scales: { x: { time: true } },
        axes: [{}, {}],
        ...options,
      };

      const u = new uCtorRef.current(baseOptions, data, containerRef.current);
      plotRef.current = u;
    })();
    return () => {
      mounted = false;
      if (pendingRAF.current) cancelAnimationFrame(pendingRAF.current);
      if (plotRef.current) {
        plotRef.current.destroy();
        plotRef.current = null;
      }
    };
  }, []); // init once

  // Throttled data updates
  useEffect(() => {
    const u = plotRef.current;
    if (!u) return;
    if (pendingRAF.current) cancelAnimationFrame(pendingRAF.current);
    pendingRAF.current = requestAnimationFrame(() => {
      pendingRAF.current = null;
      u.setData(data);
    });
  }, [data, version]);

  // Size updates
  useEffect(() => {
    const u = plotRef.current;
    if (!u) return;
    u.setSize({ width, height });
  }, [width, height]);

  // Update series/options if metadata changes (recreate chart)
  useEffect(() => {
    if (!plotRef.current) return;
    const el = containerRef.current;
    if (!el) return;
    const Ctor = uCtorRef.current;
    if (!Ctor) return;

    const currentData = data;
    plotRef.current.destroy();
    const u = new Ctor(
      {
        width,
        height,
        series: uPlotSeries,
        scales: { x: { time: true } },
        axes: [{}, {}],
        hooks: {
          init: [
            () => {
              console.warn("[WARNING]: Re-rendered full chart, ensure this isn't unnecesary")
            }
          ]
        },
        ...options,
      },
      currentData,
      el
    );
    plotRef.current = u;
  }, [uPlotSeries, options]);

  return <div ref={containerRef} style={{ width, height }} />;
}
