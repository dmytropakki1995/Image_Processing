import React from 'react';
import './App.css';
import Header from './components/Header';
import Content from './components/Content';
import Footer from './components/Footer';
import Nav from './components/Nav';
import Commersial from './components/Commersial';
import { Layout , Menu, Icon} from 'antd';

const App = () => (
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
  );

export default App;
