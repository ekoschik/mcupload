'use strict';

var React = require('react')
  , Layout = require('./Layout.jsx')
  , ImageList = require('./components/ImageList.jsx')
  , ScreenshotModal = require('./components/ScreenshotModal.jsx')
  ;

module.exports = React.createClass({

    getInitialState: function(props) {
        props = this.props || props;

        return {
            screenshots: props.screenshots || [],
            modalOpen: false,
            activeScreenshot: null,
            mapUrl: null
        };
    },

    render: function() {

        var scripts = ['/js/app.js'];

        return (
            <Layout title="Screenshots"> scripts={scripts}>
                <ImageList images={this.state.screenshots} onImageClick={this.onImageClick} />
                <ScreenshotModal hidden={!this.state.modalOpen} />
            </Layout>
        );
    },

    onImageClick: function() {
        console.log('is this working?');
        console.log('the object at hand: ', this);
    },

    addScreenshot: function(screenshot) {
        var screenshots = this.state.screenshots;
        screenshots.unshift(screenshots);
        this.setState({ screenshots: screenshots });
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
