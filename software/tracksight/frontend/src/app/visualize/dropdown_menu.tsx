import React, {Dispatch, SetStateAction} from 'react';
import {Select} from 'antd';


export default function DropdownMenu<T>(props: {
    options: string[],
    selectedOptions: T,
    setSelectedOptions: Dispatch<SetStateAction<T>>,
    placeholder?: string,
    single?: boolean,
    loading?: boolean,
    disabled?: boolean,
}) {
    return (
        <Select
            className="w-full"
            showSearch
            mode={props.single ? undefined : "multiple"}
            placeholder={props.placeholder ?? "Placeholder Text..."}
            filterOption={(input: string, option?: { label: string; value: string }) =>
                (option?.label ?? '').toLowerCase().includes(input.toLowerCase())}
            value={props.selectedOptions}
            onChange={(v) => props.setSelectedOptions(v)}
            options={props.options.map(n => ({ value: n, label: n, }))}
            maxTagCount="responsive"
            disabled={props.disabled ?? false}
            loading={props.loading ?? false}
        />
    );
};
