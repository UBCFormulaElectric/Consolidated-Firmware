"use client";

// import LabelLegend from "@/components/legends/LabelLegend";
import { WidgetDataEnum } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { useMemo, useEffect } from "react";
import { EnumSignalMetadata, SignalType } from "@/lib/types/Signal";
import { usePausePlay } from "@/components/PausePlayControl";
import { useSignals, useDataVersion } from "@/lib/contexts/SignalContext";
import { formatWithMs } from "@/lib/dateformat";

const NA_COLOR = "#E5E7EB";

function EnumWidgetBody({ signalName, setSignalName, signalMetadata, colorPalette, onDelete }: {
  setSignalName: (name: string) => void;
  colorPalette: string[];
  onDelete: () => void;
} & ({
  signalName: null;
  signalMetadata: null;
} | {
  signalName: string;
  signalMetadata: EnumSignalMetadata;
})) {
  const { horizontalScale, setHorizontalScale } = usePausePlay();
  const {
    subscribeToSignal,
    unsubscribeFromSignal,
    getEnumData,
  } = useSignals() as any;
  const dataVersion = useDataVersion();

  const signals = useMemo(() => signalMetadata ? Object.values(signalMetadata.enum.items) : [], [signalMetadata]);

  const enumData: any[] = useMemo(
    () => getEnumData(),
    [getEnumData, dataVersion]
  );

  useEffect(() => {
    if (signalName) {
      subscribeToSignal(signalName, "enumeration");
    }
    return () => {
      if (signalName) {
        unsubscribeFromSignal(signalName);
      }
    };
  }, [signalName, subscribeToSignal, unsubscribeFromSignal]);

  const chartData = useMemo(() => {
    if (!signalName) return [];
    const filteredData = enumData.filter(
      (d) => d.name === signalName && d.value != null
    );
    if (filteredData.length === 0) return [];
    // sort only this signal's points
    return filteredData
      .map((d) => ({
        time:
          typeof d.time === "number" ? d.time : new Date(d.time).getTime(),
        state: String(d.value),
      }))
      .sort((a, b) => a.time - b.time) as { time: number; state: string }[];
  }, [enumData, signalName]);

  const pixelPerPoint = 50;
  const width =
    Math.max(chartData.length * pixelPerPoint, 100) * (horizontalScale / 100);

  const bars = useMemo(() => {
    return chartData.map((dataPoint, index) => ({
      state: dataPoint.state,
      startTime: dataPoint.time,
      width: pixelPerPoint * (horizontalScale / 100),
      xOffset: index * pixelPerPoint * (horizontalScale / 100),
    }));
  }, [chartData, horizontalScale]);

  const labelBars = useMemo(() => {
    const maxLabels = 10;
    return bars.filter((_, idx) => idx % maxLabels === 0);
  }, [bars]);

  // helper to find color for a state
  const getColorForState = (state: string) => {
    if (state === "N/A") return NA_COLOR;
    if (!signalMetadata) return colorPalette[0];

    const index = Object.values(signalMetadata.enum.items).indexOf(state);
    if (index !== -1) {
      return colorPalette[index % colorPalette.length];
    }
    return colorPalette[0];
  };

  return (
    <div className="flex flex-col gap-5 w-full">
      <div className="px-8">
        <EnumSignalSelector currentSignal={signalName} onSignalChange={setSignalName} />
        <button onClick={onDelete} title="Remove mock graph"
          className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
        >
          ×
        </button>
      </div>
      {/* <LabelLegend signals={signals} colorPalette={colorPalette} /> */}

      <div className="p-4 w-full overflow-x-auto">
        {/* Controls */}
        <div className="mb-2">
          <label className="text-sm mr-2">
            Horizontal Scale: {horizontalScale}%
          </label>
          <input
            type="range"
            min={1}
            max={1000}
            value={horizontalScale}
            onChange={(e) => setHorizontalScale(+e.target.value)}
          />
        </div>

        {/* Chart */}
        {chartData.length === 0 ? (
          <div className="w-full h-6 flex items-center justify-center text-gray-500">
            No data
          </div>
        ) : (
          <div
            className="relative inline-block"
            style={{
              width,
              transition: "width 100ms ease-out",
              //   marginLeft: "60px", 
            }}
          >
            <div className="h-6 flex flex-row flex-nowrap">
              {bars.map((bar, idx) => {
                const label = bar.state;
                const iColor = getColorForState(label);

                const endTime =
                  idx < bars.length - 1
                    ? bars[idx + 1].startTime
                    : bar.startTime + 100;

                const startTimeStr = formatWithMs(new Date(bar.startTime));
                const endTimeStr = formatWithMs(new Date(endTime));

                return (
                  <div
                    key={idx}
                    className="h-6 shrink-0"
                    style={{ width: `${bar.width}px`, backgroundColor: iColor }}
                    title={`${label} from ${startTimeStr} to ${endTimeStr}`}
                  />
                );
              })}
            </div>
            <div className="absolute top-full left-0 h-4 mt-1">
              {labelBars.map((bar, idx) => (
                <div
                  key={idx}
                  className="absolute whitespace-nowrap text-xs text-gray-500"
                  style={{ left: `${bar.xOffset}px` }}
                >
                  {formatWithMs(new Date(bar.startTime))}
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    </div>
  );
}

export default function EnumWidget({ widgetData: { signal, options: { colorPalette } }, setEnumSignal, onDelete }: {
  widgetData: WidgetDataEnum
  setEnumSignal: (newSignal: string) => void;
  onDelete: () => void;
}) {
  const { isPending: signalMetadataLoading, error: signalMetadataError, data: signalMetadata } = useSignalMetadata(signal);

  if (signalMetadataLoading) {
    return (
      <div className="flex flex-col gap-5">
        <div className="px-8">
          Loading...
        </div>
        <div className="relative flex h-6 w-full">
          <div className="w-full" style={{ backgroundColor: "lightgray", }} />
        </div>
      </div>
    );
  }
  if (signalMetadataError) {
    return (
      <div className="flex flex-col items-center">
        Error loading signal metadata.
        <p className="p-4 text-center text-sm">
          Signal metadata not found for signal "{signal}". Please select a valid enum signal.
        </p>
      </div>
    )
  }
  if (signal === null || signalMetadata === null) {
    return <EnumWidgetBody
      onDelete={onDelete}
      signalName={null}
      setSignalName={setEnumSignal}
      signalMetadata={null}
      colorPalette={colorPalette} />
  }
  if (signalMetadata.type !== SignalType.ENUM) {
    return (
      <div className="flex flex-col items-center">
        Error: Signal metadata type is not Enum. THIS SHOULD NEVER HAPPEN
      </div>
    );
  }
  return <EnumWidgetBody
    onDelete={onDelete}
    signalName={signal}
    setSignalName={setEnumSignal}
    signalMetadata={signalMetadata}
    colorPalette={colorPalette} />
}
