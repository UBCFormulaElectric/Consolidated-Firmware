import { Menu } from 'antd';
import { Dispatch, SetStateAction } from 'react';

export interface NavBarProps {
  updateFunction: Dispatch<SetStateAction<string>>,
}

const NavBar = (props: NavBarProps) => (
    <Menu 
      theme="dark" 
      mode="horizontal" 
      defaultSelectedKeys={['visualize']}
      onClick={(e) => props.updateFunction(e.key)}
    >
        <Menu.Item key="visualize">Visualize</Menu.Item>
        <Menu.Item key="dashboards">Dashboards</Menu.Item>
    </Menu>
);

export default NavBar;
