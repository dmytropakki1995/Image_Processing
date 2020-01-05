import React from 'react';
import {Menu, Icon, Layout} from 'antd';
import router from '../routing/Router';
import { routeNames } from '../routing/Routes';
const {Sider} = Layout;
const Nav: React.FC = () => {
    return (
        <Sider
        breakpoint="lg"
        collapsedWidth="0"
        onBreakpoint={broken => {
          console.log(broken);
        }}
        onCollapse={(collapsed, type) => {
          console.log(collapsed, type);
        }}
      >
        <div className="logo" />
        <Menu theme="dark" mode="inline" defaultSelectedKeys={['1']}>
          <Menu.Item key="1" onClick={() => router.navigate(routeNames.Upload)}>
            <Icon type="upload" />
            <span className="nav-text">Upload image</span>
          </Menu.Item>
          <Menu.Item key="2" onClick={() => router.navigate(routeNames.About)}>
            <Icon type="user" />
            <span className="nav-text">About Us</span>
          </Menu.Item>
        </Menu>
      </Sider>
    );
}

export default Nav;