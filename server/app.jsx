var React = require('react')
  , Screenshots = require('./views/Screenshots.jsx')
  ;

React.render(
    <Screenshots {...window.PROPS} />,
    document.getElementById('content')
);
