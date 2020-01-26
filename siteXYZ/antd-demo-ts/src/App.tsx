import React from 'react';
import './App.css';
import Header from './components/Header';
import Content from './components/Content';
import Footer from './components/Footer';
import Nav from './components/Nav';
import Commersial from './components/Commersial';
import { Layout , Menu, Icon} from 'antd';
export const ThemeContext = React.createContext('dark');
export const LanguageContext = React.createContext('en');

const App = () => (
  <ThemeContext.Provider value="dark">
  <div className='App'>
    <Layout>
      <Nav/>
      <Layout>
        <Header />
        <Layout>
          <div className={'contentCommersial'}>
             <Content/>
             <Commersial/>
          </div>
        </Layout>
        <Footer />
      </Layout>
    </Layout>
  </div>
  </ThemeContext.Provider>
  );

export default App;
