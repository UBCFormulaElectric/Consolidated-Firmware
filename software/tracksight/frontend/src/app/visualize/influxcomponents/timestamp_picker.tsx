import { useState, useEffect, SetStateAction, Dispatch } from "react";
import type { Dayjs } from "dayjs";
import { DatePicker, Space, TimePicker } from "antd";
const { RangePicker } = DatePicker;

export interface TimeStampPickerProps {
  setStart: Dispatch<SetStateAction<number | null>>;
  setEnd: Dispatch<SetStateAction<number | null>>;
}

declare type EventValue<DateType> = DateType | null;
declare type RangeValue<DateType> =
  | [EventValue<DateType>, EventValue<DateType>]
  | null;

const TimeStampPicker = (props: TimeStampPickerProps) => {
  const [dayValue, setDayValue] = useState<Dayjs | null>(null);

  const changeDate = (
    date_pair: RangeValue<Dayjs>,
    formatted_dates: [string, string]
  ) => {
    if (date_pair) {
      const startEpoch = date_pair[0]!.unix();
      const endEpoch = date_pair[1]!.unix();
      props.setStart(startEpoch);
      props.setEnd(endEpoch);
    }
  };

  return (
    <Space>
      <RangePicker showTime onChange={changeDate} />
    </Space>
  );
};

export default TimeStampPicker;
