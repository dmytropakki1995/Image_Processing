import React from 'react';
import './App.css';
import Content from './components/Content';
import Footer from './components/Footer';
import Commersial from './components/Commersial';
import { Layout} from 'antd';
import Header from './components/Header';
export const ThemeContext = React.createContext('dark');
export const LanguageContext = React.createContext('en');

const App = () => (
  <ThemeContext.Provider value="dark">
    <Layout style={{height: '100%'}} className="layout">
      <Layout>
        <Header/>
      </Layout >
      <Layout className={'contentCommersial'}>
        <Content />
        <Commersial />
      </Layout>
      <Footer />
    </Layout>,
</ThemeContext.Provider>
);

export default App;
