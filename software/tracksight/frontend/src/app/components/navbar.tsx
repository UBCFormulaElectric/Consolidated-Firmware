import * as React from 'react';

import { Menu } from 'antd';

const NavBar = (props) => (
    <Menu 
      theme="light" 
      mode="horizontal" 
      defaultSelectedKeys={['visualize']}
      onClick={(e) => props.updateFunction(e.key)}
    >
        <Menu.Item key="visualize">Visualize</Menu.Item>
        <Menu.Item key="dashboards">Dashboards</Menu.Item>
    </Menu>
);

export default NavBar;
