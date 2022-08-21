import React from 'react';
import { Routes, Route } from 'react-router-dom';

import LoginPage from './LoginPage';
import LandingPage from './LandingPage';
import Editor from './Editor';

const Main = () => {
  return (
    <Routes>
        <Route path='/' element={<LoginPage/>}></Route>
        <Route path='/landing' element={<LandingPage/>}></Route>
        <Route path='/editor' element={<Editor/>}></Route>
    </Routes>
  );
}

export default Main;
