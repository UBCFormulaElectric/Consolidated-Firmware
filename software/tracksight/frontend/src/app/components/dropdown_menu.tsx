import { useState, useEffect } from 'react'; 
import { Select } from 'antd';

const DropdownMenu = (props) => {
    const [items, setItems] = useState([]);

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

DropdownMenu.defaultProps = {
    single: false
};
export default DropdownMenu;
