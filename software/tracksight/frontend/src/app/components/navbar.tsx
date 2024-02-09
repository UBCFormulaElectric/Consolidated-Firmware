import React from 'react';
import { Menu, Switch, Space } from 'antd';

export interface NavBarProps {
  updateFunction: (key: string) => void;
  theme: boolean;
  setTheme: (theme: boolean) => void;
}

const NavBar = (props: NavBarProps) => {
  const changeTheme = (checked: boolean) => {
    props.setTheme(checked);
  };

  return (
      <Menu
        theme={props.theme ? 'dark' : 'light'}
        mode="horizontal"
        defaultSelectedKeys={['visualize']}
        onClick={(e) => props.updateFunction(e.key)}
        style={{ flexGrow: 1 }} 
      >
        <Menu.Item key="visualize">Visualize</Menu.Item>
        <Menu.Item key="dashboards">Dashboards</Menu.Item>
        <Menu.Item key="theme">
        <Switch
        checked={props.theme}
        onChange={changeTheme}
        checkedChildren="Dark"
        unCheckedChildren="Light"
      />
        </Menu.Item>
      </Menu>
  );
};

export default NavBar;
