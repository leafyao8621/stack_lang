import React from 'react';
import { Routes, Route } from 'react-router-dom';

import LoginPage from './LoginPage';
import LandingPage from './LandingPage';

const Main = () => {
  return (
    <Routes>
        <Route path='/' element={<LoginPage/>}></Route>
        <Route path='/landing' element={<LandingPage/>}></Route>
    </Routes>
  );
}

export default Main;
