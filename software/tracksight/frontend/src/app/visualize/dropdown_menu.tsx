import { Dispatch, SetStateAction } from 'react'; 
import { Select } from 'antd';
import React from 'react';

export default function DropdownMenu(props: {
    options: string[],
    setSelectedOptions: Dispatch<SetStateAction<string>> | Dispatch<SetStateAction<string[]>>,
    placeholder?: string,
    single?: boolean,
    loading?: boolean,
    disabled?: boolean,
}) {
    const filterOption = (input: string, option?: { label: string; value: string }) =>
      (option?.label ?? '').toLowerCase().includes(input.toLowerCase());

    return (
        <Select
            className="w-full"
            showSearch
            mode={props.single ? undefined : "multiple"}
            placeholder={props.placeholder ?? "Placeholder Text..."}
            filterOption={filterOption}
            onChange={(v) => props.setSelectedOptions(v)}
            options={props.options.map(n => ({ value: n, label: n, }))}
            maxTagCount="responsive"
            disabled={props.disabled ?? false}
            loading={props.loading ?? false}
        />
    );
};
