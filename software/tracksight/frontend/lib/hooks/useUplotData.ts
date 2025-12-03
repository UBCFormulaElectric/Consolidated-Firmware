import {
  ParsedSignalPayload,
  SignalDataReducer,
  useSignalDataRefWithReducers,
} from "@/lib/signalData";

// TODO(evan): Add better documentation to the functions here

type UPlotData = [
  number[], // x values (time)
  ...(number[] | null)[], // y values for each signal
];

const dataPointsReducer = (
  dataPoint: ParsedSignalPayload,
  prev: UPlotData,
  signalIndexMap: Map<string, number>
) => {
  let signalIndex = signalIndexMap.get(dataPoint.signal);

  if (!signalIndex) {
    const newSignalIndex = signalIndexMap.size + 1;

    signalIndexMap.set(dataPoint.signal, newSignalIndex);

    signalIndex = newSignalIndex;
  }

  prev[signalIndex] = prev[signalIndex] || new Array(prev[0].length).fill(null);

  // NOTE(evan): Assume our new data will probably be to the more recent end of our previous data
  //             hence we can search backwards to find if we need to insert it at a previous index
  //             or create a new one
  let j = prev[0].length;

  do {
    j--;

    const prevDataPoint = prev[signalIndex]?.[j + 1] || -1;

    if (prevDataPoint > (dataPoint?.time.getTime() || 0)) continue;

    // NOTE(evan): If we reach here we know that the time slot is new so we need to insert it at index
    //             `j+1` and then go through all previous y-values to insert the new data points at the
    //             same index.
    if (prevDataPoint !== dataPoint.time.getTime()) {
      prev[0].splice(j + 1, 0, dataPoint.time.getTime());

      for (let k = 1; k < prev.length; k++) {
        // NOTE(evan): cast as number because we know signalIndexMap starts from 1
        prev[k]?.splice(j + 1, 0, null as unknown as number);
      }
    }

    // NOTE(evan): If we reach here we can assume that the other rows have populated or undefined
    //             values in this time slot so we can just overwrite it.
    prev[signalIndex]![j + 1] = dataPoint.value;

    break;
  } while (j > 0);

  return prev;
};

const createDataPointsReducer = (signals: string[]): SignalDataReducer<UPlotData> => {
  const signalIndexMap = new Map<string, number>();

  signals.forEach((signal, index) => {
    signalIndexMap.set(signal, index + 1);
  });

  return (newData: ParsedSignalPayload, prev: UPlotData) => {
    return dataPointsReducer(newData, prev, signalIndexMap);
  };
};

const useUPlotData = (signals: string[]) => {
  const reducer = createDataPointsReducer(signals);

  const data = useSignalDataRefWithReducers(signals, reducer, [[]] as UPlotData);

  return data;
};

export default useUPlotData;
