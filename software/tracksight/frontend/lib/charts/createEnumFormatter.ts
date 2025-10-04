import { DataPoint } from "@/hooks/SignalConfig";
import UPlotFormatter, { UPlotData } from "../types/UPlotFormatter";

/**
 * Creates a data point formatter for the given subscribed signals.
 */
const createEnumFormatter = (subscribedSignals: Set<string>): UPlotFormatter => {
  let currentMaxIndex = 0;
  let currentData: UPlotData = [[]];

  return (dataPoints: DataPoint[]) => {
    const updatedFormattedData = formatNewDataPoints(
      dataPoints,
      subscribedSignals,
      currentMaxIndex,
      currentData
    );

    currentMaxIndex = dataPoints.length;
    currentData = updatedFormattedData;

    return currentData;
  }
}

const formatNewDataPoints = (
  numbericalData: DataPoint[],
  subscribedSignals: Set<string>,
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

    if (!data[dataPoint.value]) {
        for (let j = data.length; j < dataPoint.value+1; j++) {
            data[j] = new Array(data[0].length).fill(null);
        }
    }

    // NOTE(evan): Assume our new data will probably be to the more recent end of our previous data
    //             hence we can search backwards to find if we need to insert it at a previous index
    //             or create a new one
    let j = data[0].length;

    do {
      j--;

      const prevDataPoint = data[dataPoint.value][j + 1] || -1;

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
      data[dataPoint.value][j + 1] = 1;

      break;
    } while (j > 0);
  }

  return data;
}

export default createEnumFormatter;
