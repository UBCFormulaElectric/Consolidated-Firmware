'use client'

import { usePathname, useRouter } from 'next/navigation'
import { useContext } from 'react';
import { Menu, Switch } from 'antd';
import { Theme, ThemeContext } from './Theme';

export default function NavBar() {
  const { theme, setTheme } = useContext(ThemeContext);
  const router = useRouter();
  const path = usePathname();

  return (
    <div className="p-0 fixed top-0 left-0 right-0">
      <Menu
        theme={theme ? 'dark' : 'light'}
        mode="horizontal"
        defaultSelectedKeys={[path.split('/')[1] || "home"]}
        onClick={(e) => {
          switch (e.key) {
            case "home":
              router.push("/");
              break;
            case "visualize":
              router.push("/visualize");
              break;
            case "dashboard":
              router.push("/dashboard");
              break;
            case "theme":
              break;
          }
        }}
        className="flex-grow"
      >
        <Menu.Item key="home">Home</Menu.Item>
        <Menu.Item key="visualize">Visualize</Menu.Item>
        <Menu.Item key="dashboard">Dashboards</Menu.Item>
        <div className="order-10">
          <Switch
            checked={theme === Theme.DARK}
            onChange={(checked: boolean) => setTheme(checked ? Theme.DARK : Theme.LIGHT)}
            checkedChildren="Dark"
            unCheckedChildren="Light"
          />
        </div>
      </Menu>
    </div>
  );
};
