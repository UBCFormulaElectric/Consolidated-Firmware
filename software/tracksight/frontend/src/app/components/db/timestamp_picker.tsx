import { useState, useEffect } from 'react'; 
import type { Dayjs } from 'dayjs';
import { DatePicker, Space, TimePicker } from 'antd';

const { RangePicker } = DatePicker;

const TimeStampPicker = (props) => {
    const [dayValue, setDayValue] = useState<Dayjs | null>(null);

    const changeDate = (date_pair, _) => {
        const startEpoch = date_pair[0].valueOf() + "ms";
        const endEpoch = date_pair[1].valueOf() + "ms";
        props.setStart(startEpoch);
        props.setEnd(endEpoch);
    };

    return (
        <Space>
            <RangePicker showTime onChange={changeDate}/>
        </Space>
    );
};

export default TimeStampPicker;
