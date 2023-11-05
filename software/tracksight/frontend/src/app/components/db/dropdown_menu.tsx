import { useState, useEffect, Dispatch, SetStateAction } from 'react'; 
import { Select } from 'antd';

export interface DropdownMenuProps {
    setOption: Dispatch<SetStateAction<string[]>>,
    selectedOptions: string[],
    options: string[],
    single: boolean,
    name: string,
}

const DropdownMenu = (props: DropdownMenuProps) => {
    const [items, setItems] = useState<Array<{value: string, label: string}>>([]);

    useEffect(() => {
        const updatedItems = props.options.map((name, index) => ({
            value: name,
            label: name,
        }));
        setItems(updatedItems);
    }, [props.options, props.selectedOptions]);
    
    const handleChange = (value: string[]) => {
        props.setOption(value);
    };

    const filterOption = (input: string, option?: { label: string; value: string }) =>
      (option?.label ?? '').toLowerCase().includes(input.toLowerCase());

    return (
        <Select
            showSearch
            mode="multiple"
            placeholder={props.name}
            filterOption={filterOption}
            onChange={handleChange}
            style={{width: "100%"}}
            options={items}
            maxTagCount="responsive"
        />
    );
};

export default DropdownMenu;
