import { DataPoint } from "@/hooks/SignalConfig";

type UPlotData = [number[], ...(Array<(number | null)[]>)];
type SignalIndexMap = Map<string | number, number>;

/**
 * Creates a data point formatter for the given subscribed signals.
 */
const createDataPointFormatter = (subscribedSignals: Set<string>) => {
  let signalIndexMap: SignalIndexMap = new Map();
  let currentMaxIndex = 0;
  let currentData: UPlotData = [[]];

  return (dataPoints: DataPoint[]) => {
    const updatedFormattedData = formatNewDataPoints(
      dataPoints,
      subscribedSignals,
      signalIndexMap,
      currentMaxIndex,
      currentData
    );

    currentMaxIndex = dataPoints.length;
    currentData = updatedFormattedData;

    return currentData;
  }
}

/**
 * Formats new data points for uPlot. **Only numerical data** is supported, read the notes if you need to extend this.
 * for enums.
 */
const formatNewDataPoints = (
  numbericalData: DataPoint[],
  subscribedSignals: Set<string>,
  signalIndexMap: SignalIndexMap,
  startAfter: number,
  startingData?: UPlotData,
): UPlotData => {
  const data = startingData || [[]];

  for (let i = startAfter; i < numbericalData.length; i++) {
    const dataPoint = numbericalData[i];

    // NOTE(evan): Assume this function is only used for numerical data for now a few spots will need
    //             to be refactored if this function needs to be used for the enums too.
    if (
      !dataPoint
      || typeof dataPoint.name !== 'string'
      || !subscribedSignals.has(dataPoint.name)
      || typeof dataPoint.time !== 'number'
      || typeof dataPoint.value !== 'number'
    ) {
      console.log("Invalid data point:", numbericalData, i);
      continue;
    }

    let signalIndex = signalIndexMap.get(dataPoint.name);

    if (!signalIndex) {
      const newSignalIndex = signalIndexMap.size + 1;

      signalIndexMap.set(dataPoint.name, newSignalIndex);
      signalIndex = newSignalIndex;
    }

    if (!data[signalIndex]) {
      data[signalIndex] = new Array(data[0].length).fill(null);
    }

    // NOTE(evan): Assume our new data will probably be to the more recent end of our previous data
    //             hence we can search backwards to find if we need to insert it at a previous index
    //             or create a new one
    let j = data[0].length;

    do {
      j--;

      const prevDataPoint = data[signalIndex][j + 1] || -1;

      if (prevDataPoint > dataPoint.time) continue;

      // NOTE(evan): If we reach here we know that the time slot is new so we need to insert it at index
      //             `j+1` and then go through all previous y-values to insert the new data points at the
      //             same index.
      if (prevDataPoint !== dataPoint.time) {
        data[0].splice(j + 1, 0, dataPoint.time);

        for (let k = 1; k < data.length; k++) {
          data[k].splice(j + 1, 0, null);
        }
      }

      // NOTE(evan): If we reach here we can assume that the other rows have populated or undefined
      //             values in this time slot so we can just overwrite it.
      data[signalIndex][j + 1] = dataPoint.value;

      break;
    } while (j > 0);
  }

  return data;
}

export default createDataPointFormatter;
