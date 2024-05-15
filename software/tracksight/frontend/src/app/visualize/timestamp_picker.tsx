import { SetStateAction, Dispatch } from 'react';
import type { Dayjs } from 'dayjs';
import { DatePicker } from 'antd';
const { RangePicker } = DatePicker;

export interface TimeStampPickerProps {
    setStart: Dispatch<SetStateAction<string>>,
    setEnd: Dispatch<SetStateAction<string>>,
}

declare type EventValue<DateType> = DateType | null;
declare type RangeValue<DateType> = [EventValue<DateType>, EventValue<DateType>] | null;

export default function TimeStampPicker(props: TimeStampPickerProps) {
    return (
        <RangePicker showTime onChange={(date_pair: RangeValue<Dayjs>, formatted_dates: [string, string]) => {
            if (!date_pair) return;
            const startEpoch = date_pair[0]!.valueOf() + "ms";
            const endEpoch = date_pair[1]!.valueOf() + "ms";
            props.setStart(startEpoch);
            props.setEnd(endEpoch);
        }}
        />
    );
}