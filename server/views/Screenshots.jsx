'use strict';

var React = require('react')
  , ImageList = require('./components/ImageList.jsx')
  , ScreenshotModal = require('./components/ScreenshotModal.jsx')
  ;

module.exports = React.createClass({

    getInitialState: function(props) {
        props = this.props || props;
        var screenshots = props.screenshots || [];
        this.calculateIndexes(screenshots);

        return {
            screenshots: screenshots,
            modalOpen: false,
            activeScreenshot: {}
        };
    },

    render: function() {
        return (
            <div id="screenshots">
                <ImageList images={this.state.screenshots} onImageClick={this.showImage} />
                <ScreenshotModal hidden={!this.state.modalOpen} image={this.state.activeScreenshot.url} handleClick={this.hideImage} />
            </div>
        );
    },

    showImage: function(image) {
        this.setState({
            modalOpen: true,
            activeScreenshot: image
        })
    },

    hideImage: function() {
        this.setState({
            modalOpen: false
        });
    },

    addScreenshot: function(screenshot) {
        var screenshots = this.state.screenshots;
        screenshots.unshift(screenshots);
        this.calculateIndexes(screenshots);
        this.setState({ screenshots: screenshots });
    },

    handleKeypress: function(e) {
        console.log(e);

        if (!this.state.modalOpen) return;

        var screenshots = this.state.screenshots
          , index = this.state.activeScreenshot.index
          ;

        // left
        if (e.keyCode === 37) {
            console.log('left');
            --index;
            if (index < 0) {
                index = screenshots.length - 1;
            }
        // right
        } else if (e.keyCode === 39) {
            console.log('right');
            ++index;
            if (index >= screenshots.length) {
                index = 0;
            }
        }

        this.setState({
            activeScreenshot: screenshots[index]
        });
    },

    calculateIndexes: function(screenshots) {
        var i = 0;
        screenshots.forEach(function(screenshot) {
            screenshot.index = i;
            ++i;
        });
    },

    componentWillReceiveProps: function(newProps, oldProps){
        this.setState(this.getInitialState(newProps));
    },

    componentDidMount: function() {
        var self = this;

        window.addEventListener('keydown', this.handleKeypress, true);

        // This feels like a hack, but I couldn't get it to work with just /screenshots
        var host = window.document.location.href.replace(/\/$/, '');
        this.socket = io.connect(host);

        socket.on('screenshot', function(screenshot) {
            self.addScreenshot(screenshot);
        });
    },

    componentWillUnmount: function() {
        // turn off the socket
        if (this.socket) {
            this.socket.disconnect();
        }

        window.removeEventListener('keydown', this.handleKeypress);
    }

});
