import React from 'react';
import './App.css';
import Content from './components/Content';
import Footer from './components/Footer';
import Commersial from './components/Commersial';
import { Layout} from 'antd';
import Header from './components/Header';
export const ThemeContext = React.createContext<'dark'|'light'>('dark');
export const LanguageContext = React.createContext('en');

const App = () => (
  <ThemeContext.Provider value='dark'>
    <Layout style={{minHeight: '100%'}} className="layout">
      <Layout>
        <Header/>
      </Layout >
      <Layout className={'contentCommersial'}>
        <Content />
        <Commersial />
      </Layout>
      <Layout style={{ textAlign: 'center' }}>Grad-z Team ©2020</Layout>
    </Layout>,
</ThemeContext.Provider>
);

export default App;
