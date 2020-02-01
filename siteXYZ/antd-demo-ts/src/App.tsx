import React from 'react';
import './App.css';
import Content from './components/Content';
import { Provider } from 'react-redux';
import Commersial from './components/Commersial';
import { Layout} from 'antd';
import Header from './components/Header';
import { store } from './redusers';
export const ThemeContext = React.createContext<'dark'|'light'>('dark');
export const LanguageContext = React.createContext('en');

const App = () => (
  <Provider store={store}>
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
</Provider>
);

export default App;
