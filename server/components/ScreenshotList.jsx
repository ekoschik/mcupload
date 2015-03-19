'use strict';

var React = require('react/addons')
  , Screenshot = require('./Screenshot.jsx')
  , ReactCSSTransitionGroup = React.addons.CSSTransitionGroup
  ;

var ScreenshotList = module.exports = React.createClass({
    render: function() {
        var screenshots = this.props.images.map(function(image) {
            return (
                <li key={image.hash}>
                    <Screenshot image={image} />
                </li>
            );
        });

        return (
            <div class="image-list">
                <ul>
                <ReactCSSTransitionGroup transitionName="float-in">
                    {screenshots}
                </ReactCSSTransitionGroup>
                </ul>
            </div>
        );

    },

    addScreenshot: function(screenshot) {
        var screenshots = this.state.screenshots;
        screenshots.unshift(screenshot);
        this.setState({ screenshots: screenshots });
    },

    getInitialState: function(props) {
        props = this.props || props;

        return {
            screenshots: props.screenshots
        };
    },
    
    componentWillReceiveProps: function(newProps, oldProps){
        this.setState(this.getInitialState(newProps));
    },

    componentDidMount: function() {
        var self = this;

        // This feels like a hack, but I couldn't get it to work with just /screenshots
        var host = window.document.location.href.replace(/\/$/, '');
        var socket = io.connect(host);

        socket.on('screenshot', function(screenshot) {
            self.addScreenshot(screenshot);
        });
    }
});