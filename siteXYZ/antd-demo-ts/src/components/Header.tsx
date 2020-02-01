import React from 'react';
import { useContext } from 'react';
import { Menu, Icon } from 'antd';
import '../styles/header.css'
import router from '../routing/Router';
import { routeNames } from '../routing/Routes';
import { ThemeContext } from '../App';
const Header: React.FC = () => {
    return (
      <>
      <Menu
      theme={useContext(ThemeContext)}
      mode="horizontal"
      defaultSelectedKeys={['1']}
      style={{ lineHeight: '64px' }}
    >
      <Menu.Item key="1" onClick={() => router.navigate(routeNames.Upload)}>
        <Icon type="upload" />
        <span className="nav-text">Image Scan</span>
      </Menu.Item>
      <Menu.Item key="2" onClick={() => router.navigate(routeNames.About)}>
        <Icon type="user" />
        <span className="nav-text">About Us</span>
      </Menu.Item>
      <Menu.Item key="3" onClick={() => router.navigate(routeNames.Faq)}>
        <Icon type="question" />
        <span className="nav-text">FAQ</span>
      </Menu.Item>
      <Menu.Item key="4" onClick={() => router.navigate(routeNames.Donation)}>
        <Icon type="fund" />
        <span className="nav-text">Donation</span>
      </Menu.Item>
    </Menu>
    </>
    );
}

export default Header;