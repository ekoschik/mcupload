'use strict';

var React = require('react')
  , ScreenshotList = require('./ScreenshotList.jsx')
  ;

var initialState = null;

React.renderComponent(
    <ScreenshotList images={initialState} />, 
    document.getElementById('react-app')
);